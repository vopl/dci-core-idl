/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "../sign.hpp"

#include <memory>
#include <vector>
#include <set>

namespace dci::idl::im::ast
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SPrimitive;
    using Primitive = std::shared_ptr<SPrimitive>;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SArray;
    using Array = std::shared_ptr<SArray>;

    struct STuple;
    using Tuple = std::shared_ptr<STuple>;

    struct SPtr;
    using Ptr = std::shared_ptr<SPtr>;

    struct SOpt;
    using Opt = std::shared_ptr<SOpt>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SList;
    using List = std::shared_ptr<SList>;

    struct SMap;
    using Map = std::shared_ptr<SMap>;

    struct SSet;
    using Set = std::shared_ptr<SSet>;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SAlias;
    using Alias = std::shared_ptr<SAlias>;

    struct SEnum;
    using Enum = std::shared_ptr<SEnum>;

    struct SEnumField;
    using EnumField = std::shared_ptr<SEnumField>;

    struct SFlags;
    using Flags = std::shared_ptr<SFlags>;

    struct SFlagsField;
    using FlagsField = std::shared_ptr<SFlagsField>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SScope;
    using Scope = std::shared_ptr<SScope>;

    struct SScopeEntry;
    using ScopeEntry = std::shared_ptr<SScopeEntry>;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SVariant;
    using Variant = std::shared_ptr<SVariant>;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SException;
    using Exception = std::shared_ptr<SException>;

    struct SExceptionBase;
    using ExceptionBase = std::shared_ptr<SExceptionBase>;

    struct SExceptionField;
    using ExceptionField = std::shared_ptr<SExceptionField>;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SStruct;
    using Struct = std::shared_ptr<SStruct>;

    struct SStructBase;
    using StructBase = std::shared_ptr<SStructBase>;

    struct SStructField;
    using StructField = std::shared_ptr<SStructField>;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SInterface;
    using Interface = std::shared_ptr<SInterface>;

    struct SInterfaceBase;
    using InterfaceBase = std::shared_ptr<SInterfaceBase>;

    struct SMethod;
    using Method = std::shared_ptr<SMethod>;

    struct SMethodParam;
    using MethodParam = std::shared_ptr<SMethodParam>;

    struct SInterfaceOpposite;
    using InterfaceOpposite = std::shared_ptr<SInterfaceOpposite>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct SScopedName;
    using ScopedName = std::shared_ptr<SScopedName>;

    struct SName;
    using Name = std::shared_ptr<SName>;
}
