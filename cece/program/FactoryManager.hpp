/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Exception.hpp"
#include "cece/program/Program.hpp"
#include "cece/program/Factory.hpp"

/* ************************************************************************ */

namespace cece {
namespace program {

/* ************************************************************************ */

/**
 * @brief Exception for access to unregistred program factory.
 */
class FactoryNotFoundException : public InvalidArgumentException
{
    using InvalidArgumentException::InvalidArgumentException;
};

/* ************************************************************************ */

/**
 * @brief Program factory manager.
 */
class FactoryManager
{

// Public Accessors
public:


    /**
     * @brief Find program factory by name.
     *
     * @param name Factory name.
     *
     * @return
     */
    ViewPtr<Factory> get(StringView name) const noexcept;


// Public Mutators
public:


    /**
     * @brief Register a new factory.
     *
     * @param name    Factory name.
     * @param factory Factory pointer.
     */
    void add(String name, UniquePtr<Factory> factory) noexcept
    {
        m_factories.emplace(std::move(name), std::move(factory));
    }


    /**
     * @brief Unregister factory.
     *
     * @param name Factory name.
     */
    void remove(StringView name) noexcept
    {
        m_factories.erase(String(name));
    }


    /**
     * @brief Register a new factory.
     *
     * @tparam FactoryType
     *
     * @param name Factory name.
     */
    template<typename FactoryType>
    void create(String name) noexcept
    {
        add(std::move(name), makeUnique<FactoryType>());
    }


    /**
     * @brief Register a new factory for specified module.
     *
     * @param name Factory name.
     */
    template<typename ProgramType>
    void createForProgram(String name) noexcept
    {
        create<FactoryTyped<ProgramType>>(std::move(name));
    }


    /**
     * @brief Register a new factory for specified module.
     *
     * @param name Factory name.
     */
    template<typename Callable>
    void createFromCallback(Callable callable) noexcept
    {
        create<FactoryCallable<Callable>>(std::move(callable));
    }


// Public Operations
public:


    /**
     * @brief Create a program by name.
     *
     * @param name Factory name.
     *
     * @return Created program.
     *
     * @throw ProgramFactoryNotFoundException In case of factory with given name doesn't exists.
     */
    UniquePtr<Program> createProgram(StringView name) const;


// Private Data Members
private:

    /// Registered module factories.
    Map<String, UniquePtr<Factory>> m_factories;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
