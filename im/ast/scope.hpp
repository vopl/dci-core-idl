/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "predecl.hpp"
#include "scopeEntry.hpp"
#include "decl.hpp"

#include <map>

namespace dci::idl::im::ast
{
    struct SScope : SScopeEntry
    {
        std::vector<Name> nestedNames;

        std::vector<Decl> decls;

        std::vector<SAlias *>       aliases;
        std::vector<SStruct *>      structs;
        std::vector<SEnum *>        enums;
        std::vector<SFlags *>       flagses;
        std::vector<SException *>   exceptions;
        std::vector<SInterface *>   interfaces;
        std::vector<SScope *>       scopes;

        std::map<std::string, SAlias *>     mergedAliases;
        std::map<std::string, SStruct *>    mergedStructs;
        std::map<std::string, SEnum *>      mergedEnums;
        std::map<std::string, SFlags *>     mergedFlagses;
        std::map<std::string, SException *> mergedExceptions;
        std::map<std::string, SInterface *> mergedInterfaces;
        std::map<std::string, SScope *>     mergedScopes;
    };
}
