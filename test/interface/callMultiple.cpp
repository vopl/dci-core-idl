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
TEST(idl, interface_callMultiple)
{
    {
        h3::Rectangle<> r;
        r.init();
        h3::Rectangle<>::Opposite r2(r);

        EXPECT_EQ(0u, r->width<dci::sbs::wire::Agg::all>().size());

        r2->width() += []
        {
            return dci::cmt::readyFuture(uint32(10));
        };

        EXPECT_EQ(1u, r->width<dci::sbs::wire::Agg::all>().size());

        r2->width() += []
        {
            return dci::cmt::readyFuture(uint32(20));
        };

        EXPECT_EQ(2u, r->width<dci::sbs::wire::Agg::all>().size());
        EXPECT_EQ(2u, r->width<dci::sbs::wire::Agg::all>().size());
    }
}
