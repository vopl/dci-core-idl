/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "lidRegistry.hpp"

namespace dci::idl::contract::impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    LidRegistry::LidRegistry()
        : _lidGen{1}
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    LidRegistry::~LidRegistry()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Lid LidRegistry::emplace(const Id& id)
    {
        //dbgAssert(_fwd.end() == _fwd.find(id));

        Lid lid = _lidGen;
        auto p = _fwd.emplace(id, lid);

        if(p.second)
        {
            ++_lidGen._value;
            _bwd.emplace(lid, id);
            return lid;
        }

        return p.first->second;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Lid LidRegistry::get(const Id& id) const
    {
        //dbgAssert(_fwd.end() != _fwd.find(id));

        auto iter = _fwd.find(id);

        if(_fwd.end() == iter)
        {
            return Lid{};
        }

        return iter->second;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const Id& LidRegistry::get(Lid lid) const
    {
        //dbgAssert(_bwd.end() != _bwd.find(lid));

        auto iter = _bwd.find(lid);

        if(_bwd.end() == iter)
        {
            static const Id badId {};
            return badId;
        }

        return iter->second;
    }

}
