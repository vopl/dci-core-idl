/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <dci/exception.hpp>

namespace dci::idl::interface::exception
{
    class MethodNotImplemented
        : public dci::exception::Skeleton<MethodNotImplemented, dci::Exception>
    {
    public:
        using dci::exception::Skeleton<MethodNotImplemented, dci::Exception>::Skeleton;

    public:
        static constexpr Eid _eid {0xb2,0x2d,0x12,0xe1,0xf1,0xfb,0x43,0x43,0x8c,0x51,0x08,0xbb,0x05,0x96,0x90,0xbc};
    };
}
