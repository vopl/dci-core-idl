/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "sign.hpp"
#include <cstring>
#include <dci/crypto/rnd.hpp>
#include <dci/utils/h2b.hpp>
#include <dci/utils/b2h.hpp>

namespace dci::idl::im
{
    Sign::Sign()
        : _data{}
    {

    }

    Sign::Sign(const std::uint8_t (&data)[_size])
    {
        memcpy(this->data(), data, _size);
    }

    Sign::Sign(const Sign& from)
    {
        memcpy(data(), from.data(), _size);
    }

    Sign::Sign(Sign&& from)
    {
        memcpy(data(), from.data(), _size);
    }

    Sign::~Sign()
    {

    }

    Sign& Sign::operator=(const Sign& from)
    {
        memcpy(data(), from.data(), _size);
        return *this;
    }

    Sign& Sign::operator=(Sign&& from)
    {
        memcpy(data(), from.data(), _size);
        return *this;
    }

    char* Sign::data()
    {
        return reinterpret_cast<char *>(_data);
    }

    const char* Sign::data() const
    {
        return reinterpret_cast<const char *>(_data);
    }

    std::string Sign::toHex(std::size_t charStart, std::size_t chars) const
    {
        if(charStart >= _size*2)
        {
            return std::string();
        }
        else if(charStart+chars > _size*2)
        {
            chars = _size*2 - charStart;
        }

        std::string res;
        res.resize(_size*2);

        dci::utils::b2h(_data, _size, res.data());

        return res.substr(charStart, chars);
    }

    bool Sign::fromHex(const std::string& txt)
    {
        if(txt.size() < _size*2)
        {
            return false;
        }

        return dci::utils::h2b(txt.data(), _size*2, _data);
    }

    void Sign::fromRnd()
    {
        dci::crypto::rnd::generate(_data, _size);
    }

    bool Sign::operator<(const Sign& with) const
    {
        return memcmp(data(), with.data(), _size) < 0;
    }

    bool Sign::operator>(const Sign& with) const
    {
        return memcmp(data(), with.data(), _size) > 0;
    }

    bool Sign::operator<=(const Sign& with) const
    {
        return memcmp(data(), with.data(), _size) <= 0;
    }

    bool Sign::operator>=(const Sign& with) const
    {
        return memcmp(data(), with.data(), _size) >= 0;
    }

    bool Sign::operator==(const Sign& with) const
    {
        return memcmp(data(), with.data(), _size) == 0;
    }

    bool Sign::operator!=(const Sign& with) const
    {
        return memcmp(data(), with.data(), _size) != 0;
    }
}
