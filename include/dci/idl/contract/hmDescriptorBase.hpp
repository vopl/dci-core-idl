/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "hmbDescriptor.hpp"

namespace dci::idl::contract
{
    struct alignas(HmbDescriptor) HmDescriptorBase
    {
        constexpr HmDescriptorBase(bool mdiSideSame)
            : _mdiSideSame(mdiSideSame)
        {
        }

        constexpr const HmbDescriptor& hmbd(uint32 index) const
        {
            return _hmbdFake[1+index];
        }

        union
        {
            const HmbDescriptor _hmbdFake[1];
            const bool _mdiSideSame;
        };

        //const HmbDescriptor _hmbd[N];
    };
}
