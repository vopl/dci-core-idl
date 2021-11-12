/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "text.hpp"
#include <regex>

namespace dci::idl::gen::executor
{
    using namespace dci::idl::im;

    class Cpp
        : public Text
    {
    public:
        Cpp();
        ~Cpp() override;

        static std::string sname();
        std::string name() override;
        std::string description() override;

        boost::program_options::options_description options() override;

        bool run(const Storage& ims, const boost::program_options::variables_map& vars) override;

    private:
        void walk(const ast::Primitive& v) override;

        void walk(const ast::Array& v) override;
        void walk(const ast::Tuple& v) override;
        void walk(const ast::Ptr& v) override;

        void walk(const ast::List& v) override;
        void walk(const ast::Map& v) override;
        void walk(const ast::Set& v) override;

        void walk(const ast::Alias& v) override;

        void walk(const ast::Enum& v) override;
        void walk(const ast::EnumField& v) override;

        void walk(const ast::Flags& v) override;
        void walk(const ast::FlagsField& v) override;

        void walk(const ast::Scope& v) override;

        void walk(const ast::Variant& v) override;

        void walk(const ast::Exception& v) override;
        void walk(const ast::ExceptionBase& v) override;
        void walk(const ast::ExceptionField& v) override;

        void walk(const ast::Struct& v) override;
        void walk(const ast::StructBase& v) override;
        void walk(const ast::StructField& v) override;

        void walk(const ast::Interface& v) override;
        //void walk(const ast::InterfaceBase& v) override;
        //void walk(const ast::Method& v) override;
        //void walk(const ast::MethodParam& v) override;

        void walk(const ast::TypeUse& v) override;
        void walk(const ast::ScopedName& v) override;

    private:
        void writeFlagsSupport();

        void writeIntrospection(const ast::Scope& v);

    private:
        void writeBundles(const ast::Scope& v);
        void writeBundleContainers(const ast::Scope& v);

        void writeHmDescriptors(const ast::Scope& v);
        void writeHmDescriptorMethods(const ast::Scope& v);

        void writeMdDescriptors(const ast::Scope& v);
        void writeMdDescriptorMethods(const ast::Scope& v);

        void writeInterfaceMethodBodies(const ast::Scope& v);
        void writeMethods(const ast::Scope& v);
        void writeStructAndExceptionOperators(const ast::Scope& v);

        void writeStiacLinks(const ast::Scope& v);
        void writeStiacLinkMethodBodies(const ast::Scope& v);
        void writeStiacExceptionSerializers(const ast::Scope& v);

    private:
        std::string nameInRoot(const std::string& name);
        std::string nameInGen(const std::string& name);
        std::string nameInSparesI(const std::string& name);
        std::string nameInSparesC(const std::string& name);
        std::string nameInPrimitives(const std::string& name);
        std::string nameInSbs(const std::string& name);
        std::string nameInDci(const std::string& name);

        std::string bundleInContainerName(const ast::SInterface* i);

        std::string idInitializer(const Sign& sign);

    private:
        struct MethodFromBaseOrSelf
        {
            ast::Method             _method;
            ast::MethodDirection    _calculatedDirection;//с учетом переворотов в базах
            bool                    _overrided;
        };

        std::vector<MethodFromBaseOrSelf> collectMethodsFromBasesOrSelf(const ast::SInterface* v);

        struct BaseOrSelf
        {
            const ast::SInterface*  _interface;
            bool                    _sameDirection;//с учетом переворотов в базах
        };

        std::vector<BaseOrSelf> collectBasesOrSelf(const ast::SInterface* v);

    private:
        void writeMethodParamType(const ast::MethodParam& param, std::size_t index);
        void writeMethodParamName(const ast::MethodParam& param, std::size_t index);
        void writeMethodSignature(const MethodFromBaseOrSelf& m, bool primary);

    private:
        const std::string _rootScopeName        = "dci::idl";
        const std::string _genScopeName         = "dci::idl::gen";
        const std::string _sparesIScopeName     = "dci::idl::interface";
        const std::string _sparesCScopeName     = "dci::idl::contract";
        const std::string _primitivesScopeName  = "dci::primitives";
        const std::string _sbsScopeName         = "dci::sbs";
        const std::string _dciScopeName         = "dci";

        bool _writeEntities = true;

        bool _writeStiacSupport = false;
        std::regex _stiacSupportFilter;
    };
}

