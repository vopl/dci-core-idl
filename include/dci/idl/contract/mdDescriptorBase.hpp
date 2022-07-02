/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "hmDescriptorBase.hpp"
#include "id.hpp"
#include "lid.hpp"

namespace dci::idl::contract
{
    struct MdState;

    struct MdDescriptorBase
    {
        using FCreateMds        = MdState *(*)();
        using FDisconnectWires  = void (*)(MdState* mds);
        using FDestroyMds       = void (*)(MdState* mds);
        using FResolveHmd       = const HmDescriptorBase *(*)(const Id& id, const Lid& lid);

        const FCreateMds        _createMds;
        const FDisconnectWires  _disconnectWires;
        const FDestroyMds       _destroyMds;
        const FResolveHmd       _resolveHmd;

        const Id&   _id;
        const Lid   _lid;

        constexpr MdDescriptorBase(
                const FCreateMds  createMds,
                const FDisconnectWires disconnectWires,
                const FDestroyMds destroyMds,
                const FResolveHmd resolveHmd,
                const Id& id,
                const Lid& lid)
            : _createMds{createMds}
            , _disconnectWires{disconnectWires}
            , _destroyMds{destroyMds}
            , _resolveHmd{resolveHmd}
            , _id{id}
            , _lid{lid}
        {
        }
    };
}
