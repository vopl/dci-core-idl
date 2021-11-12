/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "text.hpp"

namespace dci::idl::gen::executor
{
    class Deps
        : public Text
    {
    public:
        Deps();
        ~Deps() override;

        static std::string sname();
        std::string name() override;
        std::string description() override;

        boost::program_options::options_description options() override;

        bool run(const im::Storage& ims, const boost::program_options::variables_map& vars) override;

    private:
        enum class Target
        {
            unknown,
            cpp,
            ninja,
        } _target = Target::unknown;

        std::string _artifact;
    };
}

