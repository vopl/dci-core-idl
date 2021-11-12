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
    class Idl
        : public Text
    {
    public:
        Idl();
        ~Idl() override;

        static std::string sname();
        std::string name() override;
        std::string description() override;

        boost::program_options::options_description options() override;

        bool run(const im::Storage& ims, const boost::program_options::variables_map& vars) override;

    private:
        void walk(const im::ast::Primitive& v) override;

        void walk(const im::ast::Array& v) override;
        void walk(const im::ast::Tuple& v) override;
        void walk(const im::ast::Ptr& v) override;

        void walk(const im::ast::List& v) override;
        void walk(const im::ast::Map& v) override;
        void walk(const im::ast::Set& v) override;

        void walk(const im::ast::Alias& v) override;

        void walk(const im::ast::Enum& v) override;
        void walk(const im::ast::EnumField& v) override;

        void walk(const im::ast::Flags& v) override;
        void walk(const im::ast::FlagsField& v) override;

        void walk(const im::ast::Scope& v) override;

        void walk(const im::ast::Variant& v) override;

        void walk(const im::ast::Exception& v) override;
        void walk(const im::ast::ExceptionBase& v) override;
        void walk(const im::ast::ExceptionField& v) override;

        void walk(const im::ast::Struct& v) override;
        void walk(const im::ast::StructBase& v) override;
        void walk(const im::ast::StructField& v) override;

        void walk(const im::ast::Interface& v) override;
        void walk(const im::ast::InterfaceBase& v) override;
        void walk(const im::ast::Method& v) override;
        void walk(const im::ast::MethodParam& v) override;

        void walk(const im::ast::TypeUse& v) override;
        void walk(const im::ast::ScopedName& v) override;

    private:
        template <class T>
        void writeSign(const T& v);

    private:
        bool _writeSigns{false};
    };





    template <class T>
    void Idl::writeSign(const T& v)
    {
        if(!_writeSigns)
        {
            return;
        }

        _out<<"//"<<v->sign.toHex()<<el;
    }

}

