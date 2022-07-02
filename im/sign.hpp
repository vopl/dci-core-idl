/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <string>
#include <cstdint>

namespace dci::idl::im
{
    class Sign final
    {
    public:
        static constexpr std::size_t _size = 16;

    public:
        Sign();
        Sign(const std::uint8_t (&data)[_size]);
        Sign(const Sign& from);
        Sign(Sign&& from);
        ~Sign();

        Sign& operator=(const Sign& from);
        Sign& operator=(Sign&& from);

        char* data();
        const char* data() const;

        std::string toHex(std::size_t charStart=0, std::size_t chars=_size*2) const;
        bool fromHex(const std::string& txt);
        void fromRnd();

        bool operator<(const Sign& with) const;
        bool operator>(const Sign& with) const;
        bool operator<=(const Sign& with) const;
        bool operator>=(const Sign& with) const;
        bool operator==(const Sign& with) const;
        bool operator!=(const Sign& with) const;

    private:
        std::uint8_t _data[_size];
    };
}
