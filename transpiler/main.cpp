/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/integration/info.hpp>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <filesystem>

#include "im/storage.hpp"
#include "im/errorInfo.hpp"

#include "prs/config.hpp"
#include "prs/exec.hpp"

#include "gen/executor.hpp"

#include <iostream>

namespace po = boost::program_options;
using namespace dci::idl;

int main(int argc, const char **argv)
{
    ////////////////////////////////////////////////////////////////////////////////
    po::options_description desc("general options");
    desc.add_options()
            ("help",            "produce help message")
            ("version",         "print version info")
            ;

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description descInput("input options");
    descInput.add_options()
            (
                "in,i",
                po::value<std::vector<std::string>>(),
                "input file name"
            )
            (
                "include,I",
                po::value<std::vector<std::string>>(),
                "include directories"
            );


    desc.add(descInput);

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description descOutput("output options");
    descOutput.add_options()
            (
                "list-generators",
                "show generators available"
            )
            (
                "generate,g",
                po::value<std::vector<std::string>>(),
                "specify generator for use"
            );

    desc.add(descOutput);

    ////////////////////////////////////////////////////////////////////////////////
    po::positional_options_description p;
    p.add("in", -1);

    ////////////////////////////////////////////////////////////////////////////////
    po::variables_map vars;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).allow_unregistered().run(), vars);
    }
    catch(std::exception& e)
    {
        std::cerr<<"exception: "<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cerr<<"unknown exception"<<std::endl;
        return EXIT_FAILURE;
    }
    po::notify(vars);

    ////////////////////////////////////////////////////////////////////////////////
    if(vars.count("list-generators"))
    {
        std::cout << "generators available:" << std::endl;

        for(const auto& gen : gen::Executor::getAll())
        {
            std::cout << gen.first;

            gen::ExecutorPtr executor = gen.second();
            if(!executor->description().empty())
            {
                std::cout << ": " << executor->description();
            }
            std::cout << std::endl;
        }

        return EXIT_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////////////
    std::vector<gen::ExecutorPtr> executors;
    if(vars.count("generate"))
    {
        for(const std::string& gen: vars["generate"].as<std::vector<std::string>>())
        {
            gen::ExecutorPtr executor;
            try
            {
                executor = gen::Executor::getAll().at(gen)();
            }
            catch(const std::out_of_range&)
            {
                std::cerr << "generator " << gen << " is not found" << std::endl;
                return EXIT_FAILURE;
            }

            executors.push_back(executor);

            desc.add(executor->options());
        }
    }

    {
        vars.clear();
        try
        {
            po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vars);
        }
        catch(std::exception& e)
        {
            std::cerr<<"exception: "<<e.what()<<std::endl;
            return EXIT_FAILURE;
        }
        catch(...)
        {
            std::cerr<<"unknown exception"<<std::endl;
            return EXIT_FAILURE;
        }
        po::notify(vars);
    }

    ////////////////////////////////////////////////////////////////////////////////
    if(vars.empty() || vars.count("version"))
    {
        std::cout << dci::integration::info::version() << std::endl;
        return EXIT_SUCCESS;
    }

    if(vars.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////////////
    im::Storage ims;

    if(vars.count("in"))
    {
        prs::Config cfg;
        if(vars.count("include"))
        {
            cfg._includeDirectories = vars["include"].as<std::vector<std::string>>();
        }

        std::vector<std::string> idlFiles = vars["in"].as<std::vector<std::string>>();

        {
            std::vector<std::string> sourceFilesParsed;
            std::vector<im::ErrorInfo> errors;

            for(const im::ast::Scope& rootScope : prs::exec(idlFiles, cfg, errors, sourceFilesParsed))
            {
                if(!errors.empty())
                {
                    //std::cerr << "parse failed" << std::endl;
                    for(const im::ErrorInfo& error : errors)
                    {
                        std::cerr << error.toString() << std::endl;
                    }
                    return EXIT_FAILURE;
                }

                if(rootScope)
                {
                    ims.add(rootScope);
                }
            }

            for(const std::string& sourceFileParsed : sourceFilesParsed)
            {
                ims.addSource(sourceFileParsed);
            }
        }
    }

    {
        std::vector<im::ErrorInfo> errors;
        if(!ims.commit(errors))
        {
            for(const im::ErrorInfo& error : errors)
            {
                std::cerr << error.toString() << std::endl;
            }
            return EXIT_FAILURE;
        }
    }

    for(gen::ExecutorPtr& executor : executors)
    {
        if(executor->run(ims, vars))
        {
            //std::cout << executor->name() << " generation done" << std::endl;
        }
        else
        {
            std::cerr << executor->name() << " generation failed" << std::endl;
            return EXIT_FAILURE;
        }
        executor.reset();
    }
    executors.clear();

    return EXIT_SUCCESS;
}
