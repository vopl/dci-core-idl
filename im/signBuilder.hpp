/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "sign.hpp"
#include <dci/crypto/blake2b.hpp>

#include <string>
#include <cstdint>

namespace dci::idl::im
{
    //SHA-256, свернутый пополам через xor, итого 128 бит
    class SignBuilder final
    {
    public:
        SignBuilder();
        ~SignBuilder();

        void add(const Sign& v);
        void add(const std::string& v);
        void add(const char* csz);

        void add(bool v);

        void add(std::uint8_t v);
        void add(std::uint16_t v);
        void add(std::uint32_t v);
        void add(std::uint64_t v);

        void add(std::int8_t v);
        void add(std::int16_t v);
        void add(std::int32_t v);
        void add(std::int64_t v);

        Sign finish();

    private:
        void addImpl(const void* data, std::size_t size, const std::string_view tag);

    private:
        crypto::Blake2b _hashier{Sign::_size};
    };
}
