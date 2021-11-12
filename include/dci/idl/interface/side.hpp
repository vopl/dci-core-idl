/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <dci/primitives.hpp>
#include "../introspection.hpp"

namespace dci::idl::interface
{
    enum class Side : uint8
    {
        primary     = 1,
        opposite    = 2,
    };

    constexpr Side invert(Side s)
    {
        switch(s)
        {
        default:
        case Side::primary:  return Side::opposite;
        case Side::opposite: return Side::primary;
        }
    }
}

namespace dci::idl::introspection
{
    template <> inline constexpr Kind kind<dci::idl::interface::Side> = Kind::enum_;
    template <> inline constexpr uint32 fieldsCount<dci::idl::interface::Side> = 2;
    template <> struct FieldType<dci::idl::interface::Side, 0> {using result = dci::idl::interface::Side; };
    template <> struct FieldType<dci::idl::interface::Side, 1> {using result = dci::idl::interface::Side; };
    template <> inline constexpr auto fieldValue<dci::idl::interface::Side, 0> = dci::idl::interface::Side::primary;
    template <> inline constexpr auto fieldValue<dci::idl::interface::Side, 1> = dci::idl::interface::Side::opposite;
}
