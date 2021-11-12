/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/test.hpp>
#include "interface/call.hpp"

#include <vector>
#include <string>

using namespace dci;
using namespace dci::idl;

/////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
TEST(idl, interface_callMultiarg)
{
    {
        h3::Rectangle<> r;
        r.init();

        h3::Object<>::Opposite o(r);

        int callMarker = 0;

        o->multiarg() += [&](
            bool                       a3,
            int8                       a4,
            int16                      a5,
            int32                      a6,
            int64                      a7,
            uint8                      a8,
            uint16                     a9,
            uint32                     a10,
            uint64                     a11,
            real32                     a12,
            real64                     a13,
            String                     a14,
            Bytes                      a15,
            IId                        a16,
            Array<String, 2>           a17,
            Set<String>                a19,
            Map<String, Bytes>         a21,
            List<String>               a23,
            Ptr<String>                a25,
            Tuple<bool_, int8, String> a26,
            Interface                  a27,
            h3::Object<>               a28)
        {
            callMarker=42;

            EXPECT_EQ(a3, true);
            EXPECT_EQ(a4, int8(4));
            EXPECT_EQ(a5, int16(5));
            EXPECT_EQ(a6, int32(6));
            EXPECT_EQ(a7, int64(7));
            EXPECT_EQ(a8, uint8(8));
            EXPECT_EQ(a9, uint16(9));
            EXPECT_EQ(a10, uint32(10));
            EXPECT_EQ(a11, uint64(11));
            EXPECT_FLOAT_EQ(a12, real32(12));
            EXPECT_FLOAT_EQ(a13, real64(13));
            EXPECT_EQ(a14, String("14"));
            EXPECT_EQ(a15, Bytes());
            EXPECT_EQ(a16, IId());
            EXPECT_EQ(a17, (Array<String, 2>({"17", "18"})));
            EXPECT_EQ(a19, (Set<String>({"19", "20"})));
            EXPECT_EQ(a21, (Map<String, Bytes>({{"21", Bytes()}, {"22", Bytes()}})));
            EXPECT_EQ(a23, (List<String>({"23", "24"})));
            EXPECT_EQ(*a25,(*Ptr<String>(new String("25"))));
            EXPECT_EQ(a26, (Tuple<bool_, int8, String>{true, 26, "27"}));
            EXPECT_EQ(a27, Interface());
            EXPECT_EQ(a28, h3::Object<>());
        };


        for(std::size_t k(0); k<3; ++k)
        {
            r->multiarg(
                true,
                int8(4),
                int16(5),
                int32(6),
                int64(7),
                uint8(8),
                uint16(9),
                uint32(10),
                uint64(11),
                real32(12),
                real64(13),
                String("14"),
                Bytes(),
                IId(),
                Array<String, 2>({"17", "18"}),
                Set<String>({"19", "20"}),
                Map<String, Bytes>({{"21", Bytes()}, {"22", Bytes()}}),
                List<String>({"23", "24"}),
                Ptr<String>(new String("25")),
                Tuple<bool_, int8, String>{true, 26, "27"},
                Interface(),
                h3::Object<>()
            );

            EXPECT_EQ(callMarker, 42);
        }
    }
}
