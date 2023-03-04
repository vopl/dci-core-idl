/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "../ast.hpp"

namespace dci::idl::im::proc
{
    using namespace ast;

    class NestedScopeExpander
        : public boost::static_visitor<void>
    {
    public:
        void exec(const Scope& s)
        {
            operator()(s);
        }

    public:
        void operator()(const Scope& v)
        {
            if(0 == v->nestedNames.size())
            {
                //nothing
            }
            else if(1 == v->nestedNames.size())
            {
                v->name = std::move(v->nestedNames[0]);
                v->nestedNames.clear();
            }
            else
            {
                v->name = std::move(v->nestedNames[0]);
                v->nestedNames.erase(v->nestedNames.begin());

                Scope nested(new SScope);
                nested->nestedNames = std::move(v->nestedNames);
                nested->decls = std::move(v->decls);
                v->decls.clear();
                v->decls.push_back(nested);
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
