/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/idl/interface/lid.hpp>
#include <dci/idl/interface/id.hpp>
#include <dci/idl/contract/lidRegistry.hpp>

namespace dci::idl::interface
{
    bool Lid::fromText(const String& text)
    {
        Id id;
        if(!id.fromText(text))
        {
            return false;
        }

        _clid = contract::lidRegistry.get(id._cid);
        _side = id._side;

        return true;
    }

    String Lid::toText() const
    {
        Id id {contract::lidRegistry.get(_clid), _side};

        return id.toText();
    }
}
