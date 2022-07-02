/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "im/ast.hpp"
#include "error.hpp"
#include "state.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/auxiliary/eps.hpp>
#include <boost/spirit/home/x3/core/call.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <cctype>

//https://github.com/boostorg/spirit/issues/675
// в 1.76 стреляет, по идее в 1.77 вольют фикс
namespace boost::spirit::x3
{
    struct classify_workaround: char_encoding::standard
    {
        typedef unsigned char char_type;
    };

    template <>
    struct char_class_base<char_encoding::standard>: char_class_base<classify_workaround>
    {};
}
//https://github.com/boostorg/spirit/issues/675

namespace dci::idl::prs
{
    namespace x3 = boost::spirit::x3;
}

namespace dci::idl::prs::grammar
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    using namespace im::ast;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto skipper =
        x3::omit[
            +(
                x3::space |
                (x3::lit("//") >> *(x3::char_-x3::eol) >> (x3::eol|x3::eoi)) |
                (x3::lit("/*") >> *(x3::char_-x3::lit("*/")) >> x3::lit("*/"))
            )
        ];

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr x3::rule<struct quotedString_tag, std::string> quotedString {""};
    constexpr auto quotedString_def = x3::lexeme[x3::lit('"') >> *((x3::char_-'"') | (x3::lit('\\') >> (x3::char_('"') | x3::char_('\\')))) >> x3::lit('"')];
    BOOST_SPIRIT_DEFINE(quotedString)

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr x3::rule<struct identifier_tag, std::string> identifier {""};
    constexpr auto identifier_def = x3::lexeme[(x3::alpha|x3::char_('_')) >> *(x3::alnum|x3::digit|x3::char_('_'))];
    BOOST_SPIRIT_DEFINE(identifier)

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct KwParser : x3::parser<KwParser>
    {
        using attribute_type = x3::unused_type;

        constexpr KwParser(const char* sample)
          : _sample{sample}
        {}

        template <typename Iterator, typename Context>
        bool parse(Iterator& first, const Iterator& last, const Context& context, x3::unused_type, x3::unused_type) const
        {
            x3::skip_over(first, last, context);

            const char* sample = _sample;
            Iterator iter = first;

            while(*sample)
            {
                if(iter == last || *sample != *iter)
                {
                    return false;
                }

                ++sample;
                ++iter;
            }

            if(iter != last && (std::isalnum(*iter) || std::isdigit(*iter) || '_' == *iter))
            {
                return false;
            }

            first = iter;
            return true;
        }

        const char* _sample;
    };

    constexpr auto kw(const char *value)
    {
        return KwParser{value};
        //return identifier[([value](auto& ctx) { if(x3::_attr(ctx) != value) x3::_pass(ctx) = false; })];
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr x3::rule<struct integerString_tag, std::string> integerString {""};
    constexpr auto integerString_def = x3::lexeme[(x3::string("0x") >> *x3::xdigit) | (-x3::char_('-') >> *x3::digit)];
    BOOST_SPIRIT_DEFINE(integerString)

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto error(const char *msg)
    {
        return x3::eps[([=](auto& ctx){ throw Error{msg, x3::_where(ctx).begin()}; })];
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
#define ALL                                     \
        ONE(primitive,      Primitive)          \
        ONE(tuple,          Tuple)              \
        ONE(list,           List)               \
        ONE(set,            Set)                \
        ONE(map,            Map)                \
        ONE(ptr,            Ptr)                \
        ONE(opt,            Opt)                \
        ONE(array,          Array)              \
        ONE(name,           Name)               \
        ONE(scopedName,     ScopedName)         \
        ONE(typeUse,        TypeUse)            \
        ONE(alias,          Alias)              \
        ONE(variant,        Variant)            \
        ONE(structField,    StructField)        \
        ONE(struct_,        Struct)             \
        ONE(structBase,     StructBase)         \
        ONE(enumField,      EnumField)          \
        ONE(enum_,          Enum)               \
        ONE(flagsField,     FlagsField)         \
        ONE(flags,          Flags)              \
        ONE(exceptionField, ExceptionField)     \
        ONE(exception,      Exception)          \
        ONE(exceptionBase,  ExceptionBase)      \
        ONE(methodParam,    MethodParam)        \
        ONE(method,         Method)             \
        ONE(interface,      Interface)          \
        ONE(interfaceBase,  InterfaceBase)      \
        ONE(scope,          Scope)              \
        ONE(include,        std::vector<Decl>)  \
        ONE(require,        std::vector<Decl>)  \
        ONE(decl,           Decl)               \
        ONE(decls,          std::vector<Decl>)  \
        ONE(file,           Scope)

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
#define ONE(name, res) constexpr x3::rule<struct name##_tag, res> name {""};
        ALL
#undef ONE

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto primitive_def =
        x3::eps[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SPrimitive>(); })] >>
        (
            kw("void")     [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::void_;       })] |
            kw("bool")     [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::bool_;       })] |
            kw("int8")     [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::int8;        })] |
            kw("int16")    [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::int16;       })] |
            kw("int32")    [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::int32;       })] |
            kw("int64")    [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::int64;       })] |
            kw("uint8")    [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::uint8;       })] |
            kw("uint16")   [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::uint16;      })] |
            kw("uint32")   [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::uint32;      })] |
            kw("uint64")   [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::uint64;      })] |
            kw("real32")   [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::real32;      })] |
            kw("real64")   [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::real64;      })] |
            kw("string")   [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::string;      })] |
            kw("bytes")    [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::bytes;       })] |
            kw("interface")[([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::interface;   })] |
            kw("iid")      [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::iid;         })] |
            kw("ilid")     [([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::ilid;        })] |
            kw("exception")[([](auto& ctx) { x3::_val(ctx)->kind = PrimitiveKind::exception;   })]
        );

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto tuple_def =
        kw("tuple")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<STuple>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        -(typeUse[([](auto& ctx){ x3::_val(ctx)->elementTypes.push_back(x3::_attr(ctx)); })] % x3::lit(",")) >>
        (x3::lit('>') | error("'>' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto list_def =
        kw("list")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SList>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->elementType = x3::_attr(ctx); })] |
            error("list element type expected")
        ) >>
        (x3::lit('>') | error("'>' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto set_def =
        kw("set")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SSet>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->elementType = x3::_attr(ctx); })] |
            error("set element type expected")
        ) >>
        (x3::lit('>') | error("'>' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto map_def =
        kw("map")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SMap>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->keyType = x3::_attr(ctx); })] |
            error("map key type expected")
        ) >>
        (x3::lit(',') | error("',' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->valueType = x3::_attr(ctx); })] |
            error("map value type expected")
        ) >>
        (x3::lit('>') | error("'>' expected"));


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto ptr_def =
        kw("ptr")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SPtr>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->valueType = x3::_attr(ctx); })] |
            error("pointer value type expected")
        ) >>
        (x3::lit('>') | error("'>' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto opt_def =
        kw("opt")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SOpt>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->valueType = x3::_attr(ctx); })] |
            error("optional value type expected")
        ) >>
        (x3::lit('>') | error("'>' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto array_def =
        kw("array")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SArray>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->elementType = x3::_attr(ctx); })] |
            error("array element type expected")
        ) >>
        (x3::lit(',') | error("',' expected")) >>
        (
            integerString[([](auto& ctx){ x3::_val(ctx)->size = x3::_attr(ctx); })] |
            error("array size expected")
        ) >>
        (x3::lit('>') | error("'>' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto name_def =
        x3::eps[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SName>(); x3::_val(ctx)->pos = x3::get<State>(ctx).pos2Im(_where(ctx).begin()); })] >>
        identifier[([](auto& ctx) { x3::_val(ctx)->value = _attr(ctx); })];

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto scopedName_def =
        (
            x3::eps[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SScopedName>(); x3::_val(ctx)->pos = x3::get<State>(ctx).pos2Im(_where(ctx).begin()); })] >>
            (
                x3::lit("::")[([](auto& ctx){ x3::_val(ctx)->root = true; })] >>
                (
                    name[([](auto& ctx){ x3::_val(ctx)->values.push_back(x3::_attr(ctx)); })] |
                    error("type name expected")
                )
            ) |
            name[([](auto& ctx){ x3::_val(ctx)->values.push_back(x3::_attr(ctx)); })]
        ) >>
        *(
            x3::lit("::") >>
            (
                name[([](auto& ctx){ x3::_val(ctx)->values.push_back(x3::_attr(ctx)); })] |
                error("type name expected")
            )
        );

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto typeUse_def =
        primitive | tuple | list | set | map | ptr | opt | array | variant | scopedName;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto alias_def =
        kw("alias")[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SAlias>(); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("alias name expected")
        ) >>
        (x3::lit('=') | error("'=' expected")) >>
        (
            typeUse[([](auto& ctx){ x3::_val(ctx)->type = x3::_attr(ctx); })] |
            error("alias value expected")
        ) >>
        (x3::lit(';') | error("';' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto variant_def =
        kw("variant")[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SVariant>(); })] >>
        (x3::lit('<') | error("'<' expected")) >>
        -(typeUse[([](auto& ctx){ x3::_val(ctx)->elementTypes.push_back(x3::_attr(ctx)); })] % x3::lit(',')) >>
        (x3::lit('>') | error("'>' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto structField_def =
        typeUse[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SStructField>(); x3::_val(ctx)->type = x3::_attr(ctx); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("field name expected")
        ) >>
        (x3::lit(';') | error("';' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto struct__def =
        kw("struct")[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SStruct>(); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("struct name expected")
        ) >>
        -(
            x3::lit(':') >>
            (
                (
                    structBase[([](auto& ctx) { x3::_val(ctx)->bases.push_back(x3::_attr(ctx)); })] |
                    error("base struct expected")
                ) % x3::lit(',')
            )
        ) >>
        (x3::lit('{') | error("'{' or struct base expected")) >>
        *(
            structField[([](auto& ctx) { x3::_val(ctx)->fields.push_back(x3::_attr(ctx)); })]
        ) >>
        (x3::lit('}') | error("'}' or struct field expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto structBase_def =
        scopedName[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SStructBase>(); x3::_val(ctx)->scopedName = x3::_attr(ctx); })];

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto enumField_def =
        name[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SEnumField>(); x3::_val(ctx)->name = x3::_attr(ctx); })] >>
        -(
            x3::lit('=') >>
            (
                integerString[([](auto& ctx) { x3::_val(ctx)->value = x3::_attr(ctx); })] |
                error("enum value expected")
            )
        );

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto enum__def =
        kw("enum")[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SEnum>(); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("enum name expected")
        ) >>
        (x3::lit('{') | error("'{' expected")) >>
        -(
            (enumField[([](auto& ctx) { x3::_val(ctx)->fields.push_back(x3::_attr(ctx)); })] % x3::lit(',')) >> -x3::lit(',')
        ) >>
        (x3::lit('}') | error("'}' or enum field expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto flagsField_def =
        name[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SFlagsField>(); x3::_val(ctx)->name = x3::_attr(ctx); })] >>
        -(
            x3::lit('=') >>
            (
                integerString[([](auto& ctx) { x3::_val(ctx)->value = x3::_attr(ctx); })] |
                error("flags value expected")
            )
        );

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto flags_def =
        kw("flags")[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SFlags>(); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("flags name expected")
        ) >>
        (x3::lit('{') | error("'{' expected")) >>
        -(
            (flagsField[([](auto& ctx) { x3::_val(ctx)->fields.push_back(x3::_attr(ctx)); })] % x3::lit(',')) >> -x3::lit(',')
        ) >>
        (x3::lit('}') | error("'}' or flag field expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto exceptionField_def =
        typeUse[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SExceptionField>(); x3::_val(ctx)->type = x3::_attr(ctx); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("field name expected")
        ) >>
        (x3::lit(';') | error("';' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto exception_def =
        kw("exception")[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SException>(); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("exception name expected")
        ) >>
        -(
            x3::lit(':') >>
            (
                exceptionBase[([](auto& ctx) { x3::_val(ctx)->base = x3::_attr(ctx); })] |
                error("base exception expected")
            )
        ) >>
        (x3::lit('{') | error("'{' or exception base expected")) >>
        *(
            exceptionField[([](auto& ctx) { x3::_val(ctx)->fields.push_back(x3::_attr(ctx)); })]
        ) >>
        (x3::lit('}') | error("'}' or exception field expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto exceptionBase_def =
        scopedName[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SExceptionBase>(); x3::_val(ctx)->scopedName = x3::_attr(ctx); })];

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto methodParam_def =
        typeUse[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SMethodParam>(); x3::_val(ctx)->type = x3::_attr(ctx); })] >>
        (-name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })]);

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto method_def =
        (
            kw("in") [([](auto& ctx){ x3::_val(ctx) = std::make_shared<SMethod>(); x3::_val(ctx)->direction = MethodDirection::in;  })] |
            kw("out")[([](auto& ctx){ x3::_val(ctx) = std::make_shared<SMethod>(); x3::_val(ctx)->direction = MethodDirection::out; })]
        ) >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("method name expected")
        ) >>
        //(x3::lit(':') | error("':' expected")) >>
        (
            (x3::lit('(') | error("'(' expected")) >>
            (
                     -(methodParam[([](auto& ctx){ x3::_val(ctx)->query.push_back(x3::_attr(ctx)); })] % x3::lit(','))
            ) >>
            (x3::lit(')') | error("')' or method param expected"))
        ) >>
        (
            (
                x3::lit("->") >>
                (
                    typeUse [([](auto& ctx){ x3::_val(ctx)->reply = x3::_attr(ctx); x3::_val(ctx)->noreply = false; })] |
                    error("return type expected")
                )
            ) |
            x3::eps [([](auto& ctx){ x3::_val(ctx)->noreply = true; })]
        ) >>
        (x3::lit(';') | error("';' expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto interface_def =
        kw("interface")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SInterface>(); })] >>
        (
            name[([](auto& ctx){ x3::_val(ctx)->name = x3::_attr(ctx); })] |
            error("interface name expected")
        ) >>

        -(
            x3::lit(':') >>
            (
                (
                    interfaceBase[([](auto& ctx) { x3::_val(ctx)->bases.push_back(x3::_attr(ctx)); })] |
                    error("base interface expected")
                ) % x3::lit(',')
            )
        ) >>

        (x3::lit('{') | error("'{' or interface base expected")) >>
        *(
            method[([](auto& ctx) { x3::_val(ctx)->methods.push_back(x3::_attr(ctx)); })]
        ) >>
        (x3::lit('}') | error("'}' or method expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto interfaceBase_def =
        scopedName[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SInterfaceBase>(); x3::_val(ctx)->scopedName = x3::_attr(ctx); })];

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto scope_def =
        kw("scope")[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SScope>(); })] >>
        (
            (
                name[([](auto& ctx) { x3::_val(ctx)->nestedNames.push_back(x3::_attr(ctx)); })] |
                error("scope name expected")
            ) >>
            *(
                x3::lit("::") >>
                (
                    name[([](auto& ctx) { x3::_val(ctx)->nestedNames.push_back(x3::_attr(ctx)); })] |
                    error("nested scope name expected")
                )
            )
        ) >>
        (x3::lit('{') | error("'{' expected")) >>
        -decls[([](auto& ctx){ x3::_val(ctx)->decls = x3::_attr(ctx); })] >>
        (x3::lit('}') | error("'}' or decl expected"));

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto include_def =
        kw("include") >>
        x3::eps[([](auto& ctx){ x3::get<State>(ctx).storePos(_where(ctx).begin()); })] >>
        (
            quotedString[([](auto& ctx){ const Scope res = x3::get<State>(ctx).process(_attr(ctx), false); if(res) x3::_val(ctx) = res->decls; })] |
            error("file name expected")
        );

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto require_def =
        kw("require") >>
        x3::eps[([](auto& ctx){ x3::get<State>(ctx).storePos(_where(ctx).begin()); })] >>
        (
            quotedString[([](auto& ctx){ const Scope res = x3::get<State>(ctx).process(_attr(ctx), true); if(res) x3::_val(ctx) = res->decls; })] |
            error("file name expected")
        );

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto decl_def =
        (alias | struct_ | enum_ | flags | exception | interface | scope)
            [([](auto& ctx){ x3::_val(ctx) = x3::_attr(ctx); })];

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto decls_def =
        +(
            decl[([](auto& ctx){ x3::_val(ctx).push_back(x3::_attr(ctx)); })] |
            include[([](auto& ctx){ auto& dst = x3::_val(ctx); const auto& src = x3::_attr(ctx); dst.insert(dst.end(), src.begin(), src.end()); })] |
            require[([](auto& ctx){ auto& dst = x3::_val(ctx); const auto& src = x3::_attr(ctx); dst.insert(dst.end(), src.begin(), src.end()); })]
        );

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    constexpr auto file_def =
        x3::eps[([](auto& ctx) { x3::_val(ctx) = std::make_shared<SScope>(); })] >>
        -decls[([](auto& ctx){ x3::_val(ctx)->decls = x3::_attr(ctx); })];

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
#define ONE(name, res) BOOST_SPIRIT_DEFINE(name)
        ALL
#undef ONE
}
