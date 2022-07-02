/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "../errorInfo.hpp"
#include "../ast.hpp"
#include <algorithm>
#include <map>

namespace dci::idl::im::proc
{
    using namespace ast;

    class ScopeMerger
        : public boost::static_visitor<void>
    {
        enum class Mode
        {
            initiate,
            merge
        } _mode = Mode::initiate;

        std::map<std::string, SScope *> _scopesByName;

    public:
        void exec(const Scope& s)
        {
            _mode = Mode::initiate;
            operator()(s);

            _mode = Mode::merge;
            operator()(s);
        }

    public:
        void operator()(const Scope& v)
        {
            std::string key = v->prepareFullScopedName()->toString();

            if(Mode::initiate == _mode)
            {
                SScope* one = _scopesByName.try_emplace(key, v.get()).first->second;

                auto emplacer = [](auto& resMap, const auto& srcVec)
                {
                    for(const auto& v : srcVec)
                    {
                        resMap.emplace(std::make_pair(v->name->value, v));
                    }
                };
                emplacer(one->mergedAliases     ,v->aliases);
                emplacer(one->mergedStructs     ,v->structs);
                emplacer(one->mergedEnums       ,v->enums);
                emplacer(one->mergedFlagses     ,v->flagses);
                emplacer(one->mergedExceptions  ,v->exceptions);
                emplacer(one->mergedInterfaces  ,v->interfaces);
                emplacer(one->mergedScopes      ,v->scopes);
            }
            else
            {
                SScope* one = _scopesByName[key];

                v->mergedAliases    = one->mergedAliases;
                v->mergedStructs    = one->mergedStructs;
                v->mergedEnums      = one->mergedEnums;
                v->mergedFlagses    = one->mergedFlagses;
                v->mergedExceptions = one->mergedExceptions;
                v->mergedInterfaces = one->mergedInterfaces;
                v->mergedScopes     = one->mergedScopes;
            }

            for(const Decl& decl : v->decls)
            {
                boost::apply_visitor(*this, decl);
            }
        }

        template <class TOther>
        void operator()(const TOther&)
        {
        }
    };
}
