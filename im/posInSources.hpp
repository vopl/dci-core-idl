/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <string>
#include <deque>

namespace dci::idl::im
{
    struct PosInSources
    {
    public:
        void add(const std::string& file, int line, int column);
        std::string toString() const;

    private:
        struct Entry
        {
            std::string     _file;
            int             _line;
            int             _column;

            Entry(auto&& file, auto&& line, auto&& column)
                : _file{std::forward<decltype(file)>(file)}
                , _line{static_cast<int>(line)}
                , _column{static_cast<int>(column)}
            {}
        };

        std::deque<Entry> _entries;
    };
}
