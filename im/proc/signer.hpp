/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "../ast.hpp"
#include "../signBuilder.hpp"

#include <dci/utils/dbg.hpp>
#include <set>
#include <typeindex>

namespace dci::idl::im::proc
{
    using namespace ast;

    class Signer
        : public boost::static_visitor<>
    {
    public:
        void exec(Scope& s)
        {
            _processed.clear();
            (*this)(s.get());
        }

    private:
        std::set<std::pair<std::type_index, const void*>> _processed;

        template <class T>
        bool processed(const T* v)
        {
            return !_processed.insert(std::make_pair(std::type_index(typeid(v)), v)).second;
        }

    private:
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SAlias* v)
        {
            if(processed(v)) return;

            (*this)(static_cast<SScopeEntry*>(v));

            SignBuilder sb;
            sb.add("alias");
            sb.add(v->sign);

            sb.add(static_cast<std::uint32_t>(v->type.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());
                sb.add("type");
                sb.add(sptr->sign);
            }, v->type);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SStruct* v)
        {
            if(processed(v)) return;

            (*this)(static_cast<SScopeEntry*>(v));

            SignBuilder sb;
            sb.add("struct");
            sb.add(v->sign);

            auto processor = [&](const auto& vec, const char* tag)
            {
                sb.add(static_cast<std::uint32_t>(vec.size()));
                for(auto& i : vec)
                {
                    (*this)(i.get());
                    sb.add(tag);
                    sb.add(i->sign);
                }
            };

            processor(v->bases,     "base");
            processor(v->fields,    "field");

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SVariant* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("variant");

            sb.add(static_cast<std::uint32_t>(v->elementTypes.size()));
            for(const TypeUse& tu : v->elementTypes)
            {
                sb.add(static_cast<std::uint32_t>(tu.which()));
                boost::apply_visitor([&](const auto& sptr)
                {
                    (*this)(sptr.get());

                    sb.add("elementType");
                    sb.add(sptr->sign);
                }, tu);
            }

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SEnum* v)
        {
            if(processed(v)) return;

            (*this)(static_cast<SScopeEntry*>(v));

            SignBuilder sb;
            sb.add("enum");
            sb.add(v->sign);

            auto processor = [&](const auto& vec, const char* tag)
            {
                sb.add(static_cast<std::uint32_t>(vec.size()));
                for(auto& i : vec)
                {
                    (*this)(i.get());
                    sb.add(tag);
                    sb.add(i->sign);
                }
            };

            processor(v->fields,    "field");

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SFlags* v)
        {
            if(processed(v)) return;

            (*this)(static_cast<SScopeEntry*>(v));

            SignBuilder sb;
            sb.add("flags");
            sb.add(v->sign);

            auto processor = [&](const auto& vec, const char* tag)
            {
                sb.add(static_cast<std::uint32_t>(vec.size()));
                for(auto& i : vec)
                {
                    (*this)(i.get());
                    sb.add(tag);
                    sb.add(i->sign);
                }
            };

            processor(v->fields,    "field");

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SException* v)
        {
            if(processed(v)) return;

            (*this)(static_cast<SScopeEntry*>(v));

            SignBuilder sb;
            sb.add("exception");
            sb.add(v->sign);

            auto processor = [&](const auto& vec, const char* tag)
            {
                sb.add(static_cast<std::uint32_t>(vec.size()));
                for(auto& i : vec)
                {
                    (*this)(i.get());
                    sb.add(tag);
                    sb.add(i->sign);
                }
            };

            if(v->base)
            {
                sb.add("base");
                (*this)(v->base.get());
                sb.add(v->base->sign);
            }
            processor(v->fields, "field");

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SInterface* v)
        {
            if(processed(v)) return;

            (*this)(static_cast<SScopeEntry*>(v));

            SignBuilder sb;
            sb.add("interface");
            sb.add(v->sign);

            auto processor = [&](const auto& vec, const char* tag)
            {
                sb.add(static_cast<std::uint32_t>(vec.size()));
                for(auto& i : vec)
                {
                    (*this)(i.get());
                    sb.add(tag);
                    sb.add(i->sign);
                }
            };

            processor(v->bases,     "base");
            processor(v->methods,   "method");

            v->sign = sb.finish();

            (*this)(v->opposite.get());
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SScope* v)
        {
            if(processed(v)) return;

            (*this)(static_cast<SScopeEntry*>(v));

            SignBuilder sb;
            sb.add("scope");
            sb.add(v->sign);

            auto processor = [&](const auto& vec, const char* tag)
            {
                sb.add(static_cast<std::uint32_t>(vec.size()));
                for(auto& v : vec)
                {
                    (*this)(v);

                    //пусть скоуп не зависит от своего содержимого, он всего лишь пространство имен
                    //sb.add(tag);
                    //sb.add(v->sign);

                    (void)tag;
                }
            };

            processor(v->aliases,       "alias");
            processor(v->structs,       "struct");
            processor(v->enums,         "enum");
            processor(v->flagses,       "flags");
            processor(v->exceptions,    "exception");
            processor(v->interfaces,    "interface");
            processor(v->scopes,        "scope");

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SScopeEntry* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("scopeEntry");

            std::uint32_t deep = 0;
            SScopeEntry* vIter = v;
            while(vIter)
            {
                deep++;
                vIter = vIter->owner;
            }
            sb.add(deep);

            vIter = v;
            while(vIter)
            {
                if(vIter->name)
                {
                    sb.add("name");
                    sb.add(vIter->name->value);
                }

                vIter = vIter->owner;
            }

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SInterfaceBase* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("interfaceBase");

            (*this)(v->scopedName.get());
            sb.add(v->scopedName->sign);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SMethod* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("method");

            sb.add("direction");
            sb.add(static_cast<std::uint64_t>(v->direction));

            sb.add("name");
            sb.add(v->name->value);

            sb.add(static_cast<std::uint32_t>(v->query.size()));
            for(const MethodParam& mp : v->query)
            {
                (*this)(mp.get());

                sb.add("param");
                sb.add(mp->sign);
            }

            sb.add("noreply");
            sb.add(v->noreply);

            if(!v->noreply)
            {
                sb.add(static_cast<std::uint32_t>(v->reply.which()));
                boost::apply_visitor([&](const auto& sptr)
                {
                    (*this)(sptr.get());

                    sb.add("reply");
                    sb.add(sptr->sign);
                }, v->reply);
            }

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SMethodParam* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("methodParam");

            //без имени, оно не влияет на сигнатуру
            //if(v->name)
            //{
            //    sb.add("name");
            //    sb.add(v->name->value);
            //}

            sb.add(static_cast<std::uint32_t>(v->type.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("type");
                sb.add(sptr->sign);
            }, v->type);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SInterfaceOpposite* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("interfaceOpposite");

            (*this)(v->primary);
            sb.add(v->primary->sign);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SStructBase* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("structBase");

            (*this)(v->scopedName.get());
            sb.add(v->scopedName->sign);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SStructField* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("structField");

            sb.add("name");
            sb.add(v->name->value);

            sb.add(static_cast<std::uint32_t>(v->type.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("type");
                sb.add(sptr->sign);
            }, v->type);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SExceptionBase* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("exceptionBase");

            (*this)(v->scopedName.get());
            sb.add(v->scopedName->sign);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SExceptionField* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("exceptionField");

            sb.add("name");
            sb.add(v->name->value);

            sb.add(static_cast<std::uint32_t>(v->type.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("type");
                sb.add(sptr->sign);
            }, v->type);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SEnumField* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("enumField");

            sb.add("name");
            sb.add(v->name->value);

            if(!v->value.empty())
            {
                sb.add("value");

                try
                {
                    std::int64_t i64 = std::stoll(v->value, nullptr, 0);
                    sb.add(i64);
                }
                catch(...)
                {
                    std::uint64_t ui64 = std::stoull(v->value, nullptr, 0);
                    sb.add(ui64);
                }
            }

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SFlagsField* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("flagsField");

            sb.add("name");
            sb.add(v->name->value);

            if(!v->value.empty())
            {
                sb.add("value");

                try
                {
                    std::int64_t i64 = std::stoll(v->value, nullptr, 0);
                    sb.add(i64);
                }
                catch(...)
                {
                    std::uint64_t ui64 = std::stoull(v->value, nullptr, 0);
                    sb.add(ui64);
                }
            }

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SPrimitive* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("primitive");

            sb.add("kind");
            sb.add(static_cast<std::uint64_t>(v->kind));

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(STuple* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("tuple");

            sb.add(static_cast<std::uint32_t>(v->elementTypes.size()));
            for(const TypeUse& tu : v->elementTypes)
            {
                boost::apply_visitor([&](const auto& sptr)
                {
                    (*this)(sptr.get());

                    sb.add("elementType");
                    sb.add(sptr->sign);
                }, tu);
            }

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SList* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("list");

            sb.add(static_cast<std::uint32_t>(v->elementType.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("elementType");
                sb.add(sptr->sign);
            }, v->elementType);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SSet* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("set");

            sb.add(static_cast<std::uint32_t>(v->elementType.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("elementType");
                sb.add(sptr->sign);
            }, v->elementType);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SMap* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("map");

            sb.add(static_cast<std::uint32_t>(v->keyType.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("keyType");
                sb.add(sptr->sign);
            }, v->keyType);

            sb.add(static_cast<std::uint32_t>(v->valueType.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("valueType");
                sb.add(sptr->sign);
            }, v->valueType);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SPtr* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("ptr");

            sb.add(static_cast<std::uint32_t>(v->valueType.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("valueType");
                sb.add(sptr->sign);
            }, v->valueType);

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SArray* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("array");

            sb.add(static_cast<std::uint32_t>(v->elementType.which()));
            boost::apply_visitor([&](const auto& sptr)
            {
                (*this)(sptr.get());

                sb.add("elementType");
                sb.add(sptr->sign);
            }, v->elementType);

            sb.add("size");
            sb.add(std::int64_t(std::stoll(v->size, nullptr, 0)));

            v->sign = sb.finish();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void operator()(SScopedName* v)
        {
            if(processed(v)) return;

            SignBuilder sb;
            sb.add("scopedName");

            sb.add(static_cast<std::uint32_t>(v->asDecl.which()));
            boost::apply_visitor([&](auto* v)
            {
                dbgAssert(v);
                (*this)(v);

                sb.add("decl");
                sb.add(v->sign);
            }, v->asDecl);

            v->sign = sb.finish();
        }
    };
}
