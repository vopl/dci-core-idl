/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/test.hpp>

#include "../im/signBuilder.hpp"
#include "../im/sign.hpp"

/////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
TEST(idl, signBuilder)
{
    using namespace dci::idl::im;

    {
        SignBuilder sb;
        EXPECT_EQ("ac6e96149dfedb04e4d48857e86a6707", sb.finish().toHex());
    }

    {
        SignBuilder sb;
        sb.add("The quick brown fox jumps over the lazy dog");
        EXPECT_EQ("afae443d5f0425e57c1ea3be8b364f71", sb.finish().toHex());
    }

    {
        SignBuilder sb;
        sb.add("8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c928d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c9");
        EXPECT_EQ("dceba10f5b2b0192f52a19ed6006d7ed", sb.finish().toHex());
    }

    {
        SignBuilder sb;

        sb.add(SignBuilder().finish());
        sb.add(std::string("std::string"));
        sb.add("csz");

        sb.add(true);

        sb.add(std::uint8_t(1));
        sb.add(std::uint16_t(2));
        sb.add(std::uint32_t(3));
        sb.add(std::uint64_t(4));

        sb.add(std::int8_t(5));
        sb.add(std::int16_t(6));
        sb.add(std::int32_t(7));
        sb.add(std::int64_t(8));

        EXPECT_EQ("5f7d82d917bfae7d35a31476c4131985", sb.finish().toHex());
    }
}
