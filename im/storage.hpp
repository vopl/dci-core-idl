/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "ast.hpp"
#include "errorInfo.hpp"

namespace dci::idl::im
{
    class Storage
    {
    public:
        void add(const ast::Scope& root);
        void addSource(const std::string& source);
        bool commit(std::vector<ErrorInfo>& errors);

        const ast::Scope& root() const;
        const std::vector<std::string> sources() const;

    private:
        ast::Scope                  _root;
        std::vector<std::string>    _sources;
    };
}
