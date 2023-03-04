/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <dci/primitives.hpp>
#include "../introspection.hpp"

namespace dci::idl::contract
{
    struct Lid
    {
        uint32 _value {};

        explicit operator bool() const;
        bool operator !() const;

        friend auto operator<=>(const Lid&, const Lid&) = default;
    };

    inline Lid::operator bool() const
    {
        return _value ? true : false;
    }

    inline bool Lid::operator !() const
    {
        return !_value;
    }
}

namespace dci::idl::introspection
{
    template <> inline constexpr Kind kind<dci::idl::contract::Lid> = Kind::struct_;
    template <> inline constexpr uint32 basesCount<dci::idl::contract::Lid> = 0;
    template <> inline constexpr uint32 fieldsCount<dci::idl::contract::Lid> = 1;
    template <> struct FieldType<dci::idl::contract::Lid, 0> {using result = uint32; };
    template <> inline constexpr auto fieldValue<dci::idl::contract::Lid, 0> = memberValue<dci::idl::contract::Lid, &dci::idl::contract::Lid::_value>;
}
