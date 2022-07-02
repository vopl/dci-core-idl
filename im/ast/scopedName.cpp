/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "scopedName.hpp"
#include "scope.hpp"
#include "interface.hpp"
#include "interfaceOpposite.hpp"
#include <dci/utils/dbg.hpp>
#include <numeric>

namespace dci::idl::im::ast
{
    std::string SScopedName::toString(const std::string& delim) const
    {
        return std::accumulate(
                    values.begin(),
                    values.end(),
                    std::string(),
                    [&](const std::string& state, const Name& v){return state.empty() ? v->value : state+delim+v->value;});
    }

    ScopedName SScopedName::toFullScopedName() const
    {
        ScopedName res (new SScopedName);
        res->pos = this->pos;
        res->root = true;

        SScopeEntry* se = this->asScopedEntry;
        if(!se && this->asInterfaceOpposite)
        {
//            Name n(new SName);
//            n->pos = this->pos;
//            n->value = "Opposite";
//            res->values.insert(res->values.begin(), n);
            se = this->asInterfaceOpposite->primary;
        }
        while(se && se->owner)
        {
            dbgAssert(se->name);
            res->values.insert(res->values.begin(), se->name);
            se = se->owner;
        }

        res->asDecl = this->asDecl;
        res->asScopedEntry = this->asScopedEntry;
        res->asInterfaceOpposite = this->asInterfaceOpposite;

        return res;
    }
}
