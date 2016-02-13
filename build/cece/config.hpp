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

/**
 * This file contains CeCe simulator configuration.
 *
 * As a part of distribution this file ensures custom plugins are built
 * against simulator libraries with same configuration.
 */

namespace cece {
namespace config {

/* ************************************************************************ */

/**
 * @brief If CeCe is built with rendering support.
 */
#define CECE_ENABLE_RENDER

/* ************************************************************************ */

/**
 * @brief If Box2D physics is enabled.
 */
#define CECE_ENABLE_BOX2D_PHYSICS

/* ************************************************************************ */

/**
 * @brief If Box2D physics engine debug is enabled.
 */
/* #undef CECE_ENABLE_BOX2D_PHYSICS_DEBUG */

/* ************************************************************************ */

/**
 * @brief If CeCe is built with thread support.
 */
/* #undef CECE_THREAD_SAFE */

/* ************************************************************************ */

/**
 * @brief Simulator real type.
 */
using RealType = float;

/* ************************************************************************ */

/**
 * @brief CeCe version string.
 */
constexpr auto VERSION_STRING = "0.4.3";

/* ************************************************************************ */

/**
 * @brief CeCe major version.
 */
constexpr int VERSION_MAJOR = 0;

/* ************************************************************************ */

/**
 * @brief CeCe minor version.
 */
constexpr int VERSION_MINOR = 4;

/* ************************************************************************ */

/**
 * @brief CeCe patch version.
 */
constexpr int VERSION_PATCH = 3;

/* ************************************************************************ */

/**
 * @brief Simulator space dimension - only 2D is supported now.
 */
constexpr unsigned int DIMENSION = 2;

/* ************************************************************************ */

}
}

/* ************************************************************************ */
