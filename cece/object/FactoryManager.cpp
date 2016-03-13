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

// Declaration
#include "cece/object/FactoryManager.hpp"

// CeCe
#include "cece/object/Object.hpp"

/* ************************************************************************ */

namespace cece {
namespace object {

/* ************************************************************************ */

ViewPtr<Factory> FactoryManager::get(StringView name) const noexcept
{
    // FIXME: C++14
    auto it = m_factories.find(String(name));

    return it != m_factories.end() ? it->second.get() : nullptr;
}

/* ************************************************************************ */

DynamicArray<String> FactoryManager::getNames() const noexcept
{
    DynamicArray<String> names;

    for (const auto& pair : m_factories)
        names.push_back(pair.first);

    return names;
}

/* ************************************************************************ */

UniquePtr<Object> FactoryManager::createObject(StringView name, simulator::Simulation& simulation, Object::Type type) const
{
    auto factory = get(name);

    if (factory)
        return factory->create(simulation, String(name), type);

    throw FactoryNotFoundException("Object factory not found: " + String(name));
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
