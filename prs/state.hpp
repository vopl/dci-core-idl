/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "config.hpp"
#include "im/errorInfo.hpp"
#include "im/ast.hpp"
#include "iterator.hpp"

namespace dci::idl::prs
{
    class State
    {
    public:
        State(const Config& cfg, std::vector<im::ErrorInfo>& errors, std::vector<std::string>& sourceFilesParsed);
        ~State();

        im::ast::Scope process(const std::string& fileName, bool once = true);
        void storePos(Iterator pos);
        im::PosInSources pos2Im(Iterator pos);

        void pushError(const std::string& msg, const Iterator& pos);

    private:
        std::string resolveFileName(const std::string& in, std::string& errorMessage);

    private:
        const Config&                   _cfg;
        std::vector<im::ErrorInfo>&     _errors;
        std::vector<std::string>&       _sourceFilesParsed;

        struct Source
        {
            std::string         _file;
            std::vector<char>   _content{};
        };
        using SourcePtr = std::shared_ptr<Source>;

        struct SourcePos
        {
            SourcePtr   _source;
            Iterator    _pos;
        };

        std::deque<SourcePos> _sourcesStack;
    };
}
