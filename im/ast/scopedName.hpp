/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "predecl.hpp"
#include "../posInSources.hpp"
#include "name.hpp"
#include <boost/variant.hpp>

namespace dci::idl::im::ast
{
    struct SScopeEntry;

    struct SScopedName
    {
        PosInSources        pos;
        bool                root{false};
        std::vector<Name>   values;

        boost::variant<
              SAlias *
            , SVariant *
            , SStruct *
            , SEnum *
            , SFlags *
            , SException *
            , SInterface *
            , SInterfaceOpposite *
        >                   asDecl;
        SScopeEntry *       asScopedEntry {nullptr};
        SInterfaceOpposite* asInterfaceOpposite {nullptr};
        Sign                sign;//не для самого имени а для контента, который лежит в asDecl

        std::string toString(const std::string& delim = "::") const;
        ScopedName toFullScopedName() const;
    };
}
