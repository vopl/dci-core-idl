/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "out.hpp"
#include <iostream>
#include <dci/utils/dbg.hpp>
#include <filesystem>

namespace dci::idl::gen
{
    Out& el(Out& out)
    {
        out._currentLining->_isNewLine = true;
        return out << std::endl;
    }

    Out& indent(Out& out)
    {
        ++out._currentLining->_indentLevel;
        return out;
    }

    Out& undent(Out& out)
    {
        dbgAssert(out._currentLining->_indentLevel);
        --out._currentLining->_indentLevel;
        return out;
    }

    Out::Out()
    {
    }

    Out::~Out()
    {
        close();
    }

    void Out::open()
    {
        close();

        _fname = "stdout";

        _s = &std::cout;
        _currentStream = _s;
        _currentLining = &_lining;
    }

    void Out::open(const std::string& fname)
    {
        close();

        _fname = fname;

        std::filesystem::create_directories(std::filesystem::path{_fname}.parent_path());

        _fs.open(_fname);
        if(!_fs)
        {
            throw std::system_error(std::error_code{errno, std::generic_category()}, "unable to open file "+_fname);
        }

        _s = &_fs;
        _currentStream = _s;
        _currentLining = &_lining;
    }

    void Out::close()
    {
        _fs.close();
        _fs.clear();
        _s = nullptr;

        while(!_levels.empty())
        {
            _levels.pop();
        }

        _currentStream = nullptr;
        _currentLining = nullptr;

    }

    void Out::setIndent(const std::string& v)
    {
        _currentLining->_indentPad = v;
    }

    void Out::push()
    {
        _levels.emplace();
        Level& top = _levels.top();
        _currentStream = &top._stringstream;
        _currentLining = &top;
    }

    std::string Out::pop(bool commit)
    {
        dbgAssert(!_levels.empty());

        auto content = _levels.top()._stringstream.str();
        _levels.pop();

        if(_levels.empty())
        {
            _currentStream = _s;
            _currentLining = &_lining;
        }
        else
        {
            Level& top = _levels.top();
            _currentStream = &top._stringstream;
            _currentLining = &top;
        }

        if(commit)
        {
            _currentStream->write(content.data(), static_cast<std::streamsize>(content.size()));
            _currentStream->flush();
        }

        return content;
    }

    Out::operator bool()
    {
        return static_cast<bool>(_s && *_s);
    }

    Out& Out::operator<<(Out&(*v)(Out&))
    {
        return v(*this);
    }

    Out& Out::operator<<(std::ostream&(*v)(std::ostream&))
    {
        (*_currentStream) << v;
        if(!*_currentStream)
        {
            throw std::system_error(std::error_code{errno, std::generic_category()}, "unable to write file "+_fname);
        }
        _currentStream->flush();
        return *this;
    }

    Out::ScopedIndentRaii::ScopedIndentRaii()
    {
    }

    Out::ScopedIndentRaii::ScopedIndentRaii(Out* target, const std::string& prefix, const std::string& suffix)
        : _target(target)
        , _suffix(suffix)
    {
        if(_target)
        {
            if(prefix.empty())
            {
                (*_target)<<indent;
            }
            else
            {
                (*_target)<<prefix<<indent<<el;
            }
        }
    }

    Out::ScopedIndentRaii::ScopedIndentRaii(ScopedIndentRaii&& from)
        : _target(std::exchange(from._target, {}))
        , _suffix(std::move(from._suffix))
    {
    }

    Out::ScopedIndentRaii::~ScopedIndentRaii()
    {
        if(_target)
        {
            if(_suffix.empty())
            {
                (*_target)<<undent;
            }
            else
            {
                (*_target)<<undent<<_suffix<<el;
            }
        }
    }

    void Out::ScopedIndentRaii::operator=(ScopedIndentRaii&& from)
    {
        dbgAssert(!_target);
        _target = std::exchange(from._target, {});
        _suffix = std::exchange(from._suffix, {});
    }

    Out::ContentPrefixRaii::ContentPrefixRaii()
    {
    }

    Out::ContentPrefixRaii::ContentPrefixRaii(Out* target, const std::string& prefix)
        : _target(target)
    {
        if(_target)
        {
            _target->pushContentPrefix(prefix);
        }
    }

    Out::ContentPrefixRaii::ContentPrefixRaii(ContentPrefixRaii&& from)
        : _target(std::exchange(from._target, {}))
    {
    }

    Out::ContentPrefixRaii::~ContentPrefixRaii()
    {
        if(_target)
        {
            _target->popContentPrefix();
        }
    }

    void Out::ContentPrefixRaii::operator=(ContentPrefixRaii&& from)
    {
        dbgAssert(!_target);
        _target = std::exchange(from._target, {});
    }

    Out::ScopedIndentRaii Out::scope(const std::string& prefix, const std::string& suffix)
    {
        return ScopedIndentRaii(this, prefix, suffix);
    }

    Out::ContentPrefixRaii Out::contentPrefix(const std::string& prefix)
    {
        return ContentPrefixRaii(this, prefix);
    }

    void Out::pushContentPrefix(const std::string& prefix)
    {
        _currentLining->_contentPrefix.push(prefix);
    }

    void Out::popContentPrefix()
    {
        _currentLining->_contentPrefix.pop();
    }

    void Out::flushNewLine()
    {
        if(_currentLining->_isNewLine)
        {
            if(!_currentLining->_contentPrefix.empty())
            {
                (*_currentStream) << _currentLining->_contentPrefix.top();
                if(!*_currentStream)
                {
                    throw std::system_error(std::error_code{errno, std::generic_category()}, "unable to write file "+_fname);
                }
            }

            for(std::size_t i(0); i<_currentLining->_indentLevel; ++i)
            {
                (*_currentStream) << _currentLining->_indentPad;
                if(!*_currentStream)
                {
                    throw std::system_error(std::error_code{errno, std::generic_category()}, "unable to write file "+_fname);
                }
            }
            _currentStream->flush();
            _currentLining->_isNewLine = false;
        }
    }

}
