/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "../api.hpp"
#include <dci/himpl.hpp>
#include <dci/idl/implMetaInfo.hpp>
#include "id.hpp"
#include "lid.hpp"

namespace dci::idl::contract
{
    class API_DCI_IDL LidRegistry
        : public himpl::FaceLayout<LidRegistry, impl::LidRegistry>
    {
    protected:
        LidRegistry();
        ~LidRegistry();

    public:
        Lid emplace(const Id& id);
        Lid get(const Id& id) const;
        const Id& get(Lid lid) const;
    };

    extern LidRegistry& lidRegistry API_DCI_IDL;
}
