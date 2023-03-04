/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "signBuilder.hpp"
#include <dci/utils/endian.hpp>
#include <dci/utils/dbg.hpp>
#include <cstring>
#include <string_view>

namespace dci::idl::im
{
    using namespace dci::utils::endian;
    using namespace std::literals;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    SignBuilder::SignBuilder()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    SignBuilder::~SignBuilder()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(const Sign& v)
    {
        addImpl(v.data(), v._size, "sign"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(const std::string& v)
    {
        addImpl(v.data(), v.size(), "string"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(const char* csz)
    {
        addImpl(csz, strlen(csz), "csz"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(bool v)
    {
        v = !!v;
        addImpl(&v, sizeof(v), "bool"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::uint8_t v)
    {
        addImpl(&v, sizeof(v), "ui8"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::uint16_t v)
    {
        auto buf = n2b(v);
        addImpl(&buf, sizeof(buf), "ui16"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::uint32_t v)
    {
        auto buf = n2b(v);
        addImpl(&buf, sizeof(buf), "ui32"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::uint64_t v)
    {
        auto buf = n2b(v);
        addImpl(&buf, sizeof(buf), "ui64"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::int8_t v)
    {
        addImpl(&v, sizeof(v), "i8"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::int16_t v)
    {
        auto buf = n2b(v);
        addImpl(&buf, sizeof(buf), "i16"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::int32_t v)
    {
        auto buf = n2b(v);
        addImpl(&buf, sizeof(buf), "i32"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::add(std::int64_t v)
    {
        auto buf = n2b(v);
        addImpl(&buf, sizeof(buf), "i64"sv);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Sign SignBuilder::finish()
    {
        Sign res {};

        dbgAssert(Sign::_size == _hashier.digestSize());
        _hashier.finish(res.data());

        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void SignBuilder::addImpl(const void* data, std::size_t len, const std::string_view tag)
    {
        _hashier.add(data, len);
        _hashier.barrier();
        if(!tag.empty())
        {
            _hashier.add(tag.data(), tag.size());
        }
    }
}
