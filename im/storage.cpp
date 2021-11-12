/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "storage.hpp"

#include "proc/nestedScopeExpander.hpp"
#include "proc/ownerIndexer.hpp"
#include "proc/scopeMerger.hpp"
#include "proc/nameChecker.hpp"
#include "proc/scopedNameResolver.hpp"
#include "proc/scopedNameOrderChecker.hpp"
#include "proc/signer.hpp"

namespace dci::idl::im
{
    void Storage::add(const ast::Scope& root)
    {
        dbgAssert(root);

        if(!_root)
        {
            _root = root;
        }
        else
        {
            _root->decls.insert(_root->decls.end(), root->decls.begin(), root->decls.end());
        }
    }

    void Storage::addSource(const std::string& source)
    {
        _sources.emplace_back(source);
    }

    bool Storage::commit(std::vector<ErrorInfo>& errors)
    {
        dbgAssert(errors.empty());
        if(!errors.empty())
        {
            return false;
        }

        if(!_root)
        {
            errors.emplace_back(ErrorInfo {
                                  "empty input",
                                  PosInSources{}});
            return false;
        }

        //expand nested scopes
        proc::NestedScopeExpander().exec(_root);

        //index owners
        proc::OwnerIndexer().exec(_root);

        //merge scopes
        proc::ScopeMerger().exec(_root);

        //check names uniqueness
        proc::NameChecker(errors).exec(_root);
        if(!errors.empty())
        {
            return false;
        }

        //resolve typeUse.scopedName
        proc::ScopedNameResolver(errors).exec(_root);
        if(!errors.empty())
        {
            return false;
        }

        //check scoped names vs declarations ordering
        proc::ScopedNameOrderChecker(errors).exec(_root);
        if(!errors.empty())
        {
            return false;
        }

        //sign
        proc::Signer().exec(_root);

        return true;
    }

    const ast::Scope& Storage::root() const
    {
        return _root;
    }

    const std::vector<std::string> Storage::sources() const
    {
        return _sources;
    }
}
