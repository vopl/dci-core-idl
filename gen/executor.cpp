/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "executor.hpp"
#include "executor/idl.hpp"
#include "executor/cpp.hpp"
#include "executor/deps.hpp"

namespace dci::idl::gen
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace
    {
        class ExecutorsMap
            : public std::map<std::string, std::function<ExecutorPtr()>>
        {
        public:
            ExecutorsMap()
            {
                insert(std::make_pair(executor::Idl::sname(), [](){return ExecutorPtr(new executor::Idl);}));
                insert(std::make_pair(executor::Cpp::sname(), [](){return ExecutorPtr(new executor::Cpp);}));
                insert(std::make_pair(executor::Deps::sname(), [](){return ExecutorPtr(new executor::Deps);}));
            }
        } executorsMap;
    }

    const std::map<std::string, std::function<ExecutorPtr()>>& Executor::getAll()
    {
        return executorsMap;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Executor::Executor()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Executor::~Executor()
    {

    }
}
