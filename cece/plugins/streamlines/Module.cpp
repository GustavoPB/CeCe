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
#include "cece/plugins/streamlines/Module.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/constants.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/core/constants.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/BinaryInput.hpp"
#include "cece/core/BinaryOutput.hpp"
#include "cece/core/ShapeToGrid.hpp"
#include "cece/object/Object.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Parse layout description.
 *
 * @param is     Input stream.
 * @param layout Output layout.
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Module::Layout& layout)
{
    return is >> std::skipws >>
        layout[Module::LayoutPosTop] >>
        layout[Module::LayoutPosRight] >>
        layout[Module::LayoutPosBottom] >>
        layout[Module::LayoutPosLeft]
    ;
}

/* ************************************************************************ */

/**
 * @brief Read inlet velocities.
 *
 * @param is         Input stream.
 * @param velocities
 *
 * @return is.
 */
InStream& operator>>(InStream& is, Module::InletVelocities& velocities)
{
    return is >> std::skipws >>
        velocities[Module::LayoutPosTop] >>
        velocities[Module::LayoutPosRight] >>
        velocities[Module::LayoutPosBottom] >>
        velocities[Module::LayoutPosLeft]
    ;
}

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

constexpr StaticArray<char, 5> FILE_GUARD{{'C', 'E', 'S', 'L', '\0'}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<Vector<int>, 2>, Module::LayoutPosCount> EDGES{{
    {{{ 1, -1}, { 1,  1}}},
    {{{-1, -1}, {-1,  1}}},
    {{{-1,  1}, { 1,  1}}},
    {{{ 1, -1}, {-1, -1}}}
}};

/* ************************************************************************ */

/**
 * @brief Calculate Poiseuille flow profile.
 *
 * @param max  Max speed.
 * @param pos  Position from range.
 * @param size Range size.
 *
 * @return
 */
template<typename T>
units::Velocity calcPoiseuilleFlow(units::Velocity max, T pos, T size) noexcept
{
    Assert(pos >= 0);
    Assert(pos < size);
    Assert(size > 1);

    const RealType posF = static_cast<RealType>(pos);
    const RealType sizeF = static_cast<RealType>(size);
    return 4.f * max / (sizeF * sizeF) * (sizeF * posF - posF * posF);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Module::Module()
{
    m_layout[LayoutPosTop]    = LayoutType::Barrier;
    m_layout[LayoutPosBottom] = LayoutType::Barrier;
    m_layout[LayoutPosLeft]   = LayoutType::Inlet;
    m_layout[LayoutPosRight]  = LayoutType::Outlet;
}

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::init(simulator::Simulation& simulation)
{
    initBarriers(simulation);

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

    // Calculate values
    const auto vMax = calculateMaxVelocity(dl);
    const auto omega = 1.0 / getTau();

    Log::info("[streamlines] Omega: ", omega);
    Log::info("[streamlines] Tau: ", getTau());
    Log::info("[streamlines] Viscosity: ", calculateViscosity());
    Log::info("[streamlines] Cell size: (", dl.getX(), " um; ", dl.getY(), " um)");
    Log::info("[streamlines] Max velocity: (", vMax.getX(), " um/s; ", vMax.getY(), " um/s)");
    Log::info("[streamlines] Max object speed: ", simulation.getMaxObjectTranslation(), " um/it");
    Log::info("[streamlines] Viscosity: ", getKinematicViscosity(), " um2/s");

    // Obstacles
    updateObstacleMap(simulation, vMax);

    Log::info("[streamlines] Initialization...");

    bool initialized = false;

    if (!m_initFile.empty() && fileExists(m_initFile))
    {
        try
        {
            Log::info("[streamlines] Loading from external file: ", m_initFile);

            loadFromFile(m_initFile);
            initialized = true;
        }
        catch (const Exception& e)
        {
            Log::warning("[streamlines] ", e.what());
        }
    }

    if (!initialized)
    {
        // Initialization iterations
        for (simulator::IterationNumber it = 1; it <= getInitIterations(); it++)
        {
            if ((it % 100) == 0)
                Log::info("[streamlines] Initialization ", it, "/", getInitIterations());

            m_lattice.collideAndStream(omega);

            // Apply boundary conditions
            applyBoundaryConditions(simulation, vMax);
        }

        // Store initialization
        if (!m_initFile.empty())
        {
            storeToFile(m_initFile);
        }
    }

    Log::info("[streamlines] Initialization done.");
}

/* ************************************************************************ */

void Module::initBarriers(simulator::Simulation& simulation)
{
    // Create barriers
    for (unsigned int pos = 0; pos < LayoutPosCount; ++pos)
    {
        if (m_layoutBarriers[pos])
        {
            // Barrier recreation is not needed
            if (m_layout[pos] == LayoutType::Barrier)
                continue;

            simulation.deleteObject(m_layoutBarriers[pos].get());
            m_layoutBarriers[pos] = nullptr;
        }
        else if (m_layout[pos] == LayoutType::Barrier)
        {
            initBorderBarrier(simulation, static_cast<LayoutPosition>(pos));
        }
    }
}

/* ************************************************************************ */

void Module::update(simulator::Simulation& simulation, units::Time dt)
{
    Assert(m_lattice.getSize() != Zero);

    auto _ = measure_time("streamlines", simulator::TimeMeasurementIterationOutput(simulation));

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();

    // Calculate maximum flow velocity
    const auto vMax = calculateMaxVelocity(dl);

    // Relaxation parameter
    const auto omega = 1.0 / getTau();

    // Calculate conversion coefficient
    setCoefficient(calculateCoefficient(dt, dl));

    // Obstacles
    updateObstacleMap(simulation, vMax);

    // Store streamlines data
    if (m_dataOut)
    {
        for (auto&& c : range(m_lattice.getSize()))
        {
            const auto& data = m_lattice[c];
            const auto vel = vMax * data.calcVelocityNormalized();

            *m_dataOut <<
                // iteration
                simulation.getIteration() - 1 << ";" <<
                // totalTime
                simulation.getTotalTime().value() << ";" <<
                // x
                c.getX() << ";" <<
                // y
                c.getY() << ";" <<
                // velX
                vel.getX().value() << ";" <<
                // velY
                vel.getY().value()
            ;

            if (m_dataOutDensity)
            {
                *m_dataOut << ";" <<
                    // rho
                    data.calcRho()
                ;
            }

            if (m_dataOutPopulations)
            {
                *m_dataOut << ";" <<
                    // d0
                    data[0] << ";" <<
                    // d1
                    data[1] << ";" <<
                    // d2
                    data[2] << ";" <<
                    // d3
                    data[3] << ";" <<
                    // d4
                    data[4] << ";" <<
                    // d5
                    data[5] << ";" <<
                    // d6
                    data[6] << ";" <<
                    // d7
                    data[7] << ";" <<
                    // d8
                    data[8]
                ;
            }

            *m_dataOut << "\n";
        }

        m_dataOut->flush();
    }

#ifdef CECE_THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Collide and propagate
    for (auto it = getInnerIterations(); it--; )
    {
        m_lattice.collideAndStream(omega);

        // Apply boundary conditions
        applyBoundaryConditions(simulation, vMax);
    }

    // Apply streamlines to world objects
    applyToObjects(simulation, dt, vMax);
}

/* ************************************************************************ */

void Module::loadConfig(simulator::Simulation& simulation, const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(simulation, config);

    // Number of init iterations
    setInitIterations(config.get("init-iterations", getInitIterations()));

    // Number of inner iterations
    setInnerIterations(config.get("iterations", getInnerIterations()));

    if (getInnerIterations() == 0)
        throw InvalidArgumentException("Number of inner iterations cannot be zero");

    // Convert relaxation time
    setTau(config.get("tau", getTau()));

    if (getTau() == 0)
        throw InvalidArgumentException("Relaxation time cannot be zero");

    if (config.has("inlet-velocity"))
    {
        auto vel = config.get<units::Velocity>("inlet-velocity");
        m_inletVelocities[LayoutPosTop]    = vel;
        m_inletVelocities[LayoutPosBottom] = vel;
        m_inletVelocities[LayoutPosLeft]   = vel;
        m_inletVelocities[LayoutPosRight]  = vel;
    }
    else
    {
        // Inlet velocities
        setInletVelocities(config.get("inlet-velocities", getInletVelocities()));
    }

    // Viscosity
    setKinematicViscosity(config.get("kinematic-viscosity", getKinematicViscosity()));

    if (getKinematicViscosity() == Zero)
        throw InvalidArgumentException("Kinematic viscosity cannot be zero");

    // Grid size
    m_lattice.setSize(config.get<Vector<Lattice::SizeType>>("grid"));

    if (m_lattice.getSize() == Zero)
        throw InvalidArgumentException("Lattice size cannot be zero");

    // Layout
    setLayout(config.get("layout", getLayout()));

    // Get layout type
    m_layoutType = config.get("layout-type", String{});

    // Enable dynamic object obstacles
    setDynamicObjectsObstacles(config.get("dynamic-object-obstacles", isDynamicObjectsObstacles()));

    if (config.has("data-out-filename"))
    {
        m_dataOut = makeUnique<OutFileStream>(config.get("data-out-filename"));
        m_dataOutDensity = config.get<bool>("data-out-density", false);

        *m_dataOut << "iteration;totalTime;x;y;velX;velY";

        if (m_dataOutDensity)
            *m_dataOut << ";rho";

        if (m_dataOutPopulations)
            *m_dataOut << ";d0;d1;d2;d3;d4;d5;d6;d7;d8";

        *m_dataOut << "\n";
    }

#if ENABLE_RENDER && DEV_PLUGIN_streamlines_RENDER
    setDebugMagnitudeScale(config.get("debug-magnitude-scale", getDebugMagnitudeScale()));
#endif

    // Get initialization file
    if (config.has("init-file"))
    {
        // Get file name
        auto file = config.get("init-file");

        // In case of %temp%
        if (file.substr(0, 6) == "%temp%")
        {
            m_initFile = tempDirectory() / file.substr(6);
        }
        else
        {
            m_initFile = config.buildFilePath(file);
        }
    }

    // Initialize lattice
    init(simulation);
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(const simulator::Simulation& simulation, render::Context& context)
{
#if DEV_PLUGIN_streamlines_RENDER
    if (getDrawFlags() == 0)
        return;

    const auto size = m_lattice.getSize();

    if (!m_drawableDebug)
        m_drawableDebug.create(context, size);

    // Temporary for velocities
    Grid<Vector<LatticeCell::ValueType>> velocities(size);

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / m_lattice.getSize();
    // Calculate maximum flow velocity
    const auto vMax = calculateMaxVelocity(dl);

    // Calculate grid max velocity
    Assert(!m_inletVelocities.empty());
    const auto maxInlet = *std::max_element(m_inletVelocities.begin(), m_inletVelocities.end());
    const auto maxVel = m_debugMagnitudeScale * maxInlet / vMax.getLength();

    {
#ifdef CECE_THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif

        // Update texture
        for (auto&& c : range(size))
        {
            // Cell alias
            const auto& cell = m_lattice[c];
            render::Color color = render::colors::BLACK;

            if (cell.hasObstacleDynamics() || cell.hasNoDynamics())
            {
                velocities[c] = Zero;

                if (checkDebugDraw(DRAW_DEBUG_OBSTACLES))
                {
                    switch (cell.getDynamics())
                    {
                    case LatticeCell::Dynamics::None:
                        color = render::colors::WHITE;
                        break;

                    case LatticeCell::Dynamics::BGK:
                        color = render::colors::BLACK;
                        break;

                    case LatticeCell::Dynamics::StaticObstacle:
                        color = render::colors::GREEN;
                        break;

                    case LatticeCell::Dynamics::DynamicObstacle:
                        color = render::colors::RED;
                        break;

                    default:
                        break;
                    }

                    // Set alpha channel
                    color.setAlpha(0);
                }
            }
            else
            {
                // Cell velocity
                velocities[c] = cell.calcVelocityNormalized();

                if (checkDebugDraw(DRAW_DEBUG_MAGNITUDE))
                    color = render::Color::fromGray(velocities[c].getLength() / maxVel);
            }

            m_drawableDebug->set(c, color);
        }
    }

    if (!m_drawableDirections)
    {
        m_drawableDirections.create(context, size, velocities.getData(), maxVel);
    }
    else
    {
        m_drawableDirections->setMax(maxVel);
        m_drawableDirections->update(velocities.getData());
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));

    if (checkDebugDraw(DRAW_DEBUG_MAGNITUDE) || checkDebugDraw(DRAW_DEBUG_OBSTACLES))
        m_drawableDebug->draw(context);

    if (checkDebugDraw(DRAW_DEBUG_DIRECTION))
        m_drawableDirections->draw(context);

    context.matrixPop();
#endif
}
#endif

/* ************************************************************************ */

VelocityVector Module::calculateMaxVelocity(PositionVector dl) const noexcept
{
    return (LatticeCell::MAX_SPEED / calculateViscosity()) * getKinematicViscosity() / dl;
}

/* ************************************************************************ */

void Module::updateObstacleMap(const simulator::Simulation& simulation, const VelocityVector& vMax)
{
    // Clear previous flag
    m_lattice.setDynamics(LatticeCell::Dynamics::BGK);

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        const bool isDynamic = obj->getType() == object::Object::Type::Dynamic;

        // Ignore dynamic objects
        if (!isDynamicObjectsObstacles() && isDynamic)
            continue;

        // Get object position
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(Zero, simulation.getWorldSize()))
            continue;

        // Get grid position
        const auto coord = Coordinate(pos / step);

        // Calculate object velocity in LB
        const auto velocity = obj->getVelocity() / vMax;

        // In this case duplicate coordinates doesn't harm and calling
        // operation multiple times on same coordinate is faster than
        // sorting and erasing non-unique coordinates.

        // Map shapes to grid
        for (const auto& shape : obj->getShapes())
        {
            mapShapeToGrid(
                [this, &velocity, isDynamic] (Coordinate&& coord) {
                    Assert(m_lattice.inRange(coord));
                    if (isDynamic)
                        m_lattice[coord].setDynamicObstacleDynamics(velocity);
                    else
                        m_lattice[coord].setStaticObstacleDynamics();
                },
                [] (Coordinate&& coord) {},
                shape, step, coord, obj->getRotation(), m_lattice.getSize()
            );
        }
    }

    m_lattice.fixupObstacles(LatticeCell::Dynamics::StaticObstacle);

    if (isDynamicObjectsObstacles())
        m_lattice.fixupObstacles(LatticeCell::Dynamics::DynamicObstacle);
}

/* ************************************************************************ */

void Module::applyToObjects(const simulator::Simulation& simulation, units::Time dt, const VelocityVector& vMax)
{
    // Foreach objects
    for (auto& obj : simulation.getObjects())
    {
        applyToObject(*obj, simulation, dt, vMax);
    }
}

/* ************************************************************************ */

void Module::applyToObject(object::Object& object, const simulator::Simulation& simulation,
    units::Time dt, const VelocityVector& vMax)
{
    // Ignore static objects
    if (object.getType() != object::Object::Type::Dynamic)
        return;

    // Maximum object speed that is allowed by physical engine
    const auto maxSpeed = simulation.getMaxObjectTranslation() / dt;
    const auto maxSpeedSq = maxSpeed * maxSpeed;

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / m_lattice.getSize();

    // Get mass local center
    const auto center = object.getMassCenterOffset();

    // Coefficient used in force calculation
    const auto forceCoefficient = 6 * constants::PI * getKinematicViscosity() * object.getDensity();

    auto force = ForceVector{Zero};
    auto velocityObjEnv = VelocityVector{Zero};

    // Map shapes border to grid
    for (const auto& shape : object.getShapes())
    {
        // Only circle shapes are supported
        if (shape.getType() != ShapeType::Circle)
            continue;

        // Shape alias
        const auto& circle = shape.getCircle();

        // Transform from [-size / 2, size / 2] to [0, size] space
        const auto pos = object.getWorldPosition(circle.center) - start;

        // Check if position is in range
        if (!pos.inRange(Zero, simulation.getWorldSize()))
            continue;

        // Get coordinate to lattice
        const auto coord = Coordinate(pos / step);

        Vector<float> velocityLB = Zero;
        unsigned long count = 0;

        // Store velocity for each coordinate
        mapShapeBorderToGrid(
            [this, &velocityLB, &vMax, &count] (Coordinate&& coord) {
                if (!m_lattice[coord].hasObstacleDynamics())
                {
                    velocityLB += m_lattice[coord].calcVelocity();
                    ++count;
                }
            },
            [] (Coordinate&& coord) { },
            shape, step, coord, m_lattice.getSize(), {}, 1
        );

        if (count == 0)
            continue;

        // Average
        velocityLB /= count;
        const VelocityVector velocityEnv = velocityLB * vMax;
        //const VelocityVector velocityEnv{m_inletVelocities[0], Zero};

        if (velocityEnv.getLengthSquared() > maxSpeedSq)
        {
            OutStringStream oss;
            oss <<
                "[streamlines] Physical engine can't handle environment "
                "velocity (" << velocityEnv.getLength() << " um/s < " << maxSpeed << " um/s). "
                "Decrease inlet velocities or change topology."
            ;

            Log::warning(oss.str());
            //throw RuntimeException(oss.str());
        }

        velocityObjEnv += velocityEnv;

        // Shape radius
        const auto radius = circle.radius;
        // Distance from mass center
        const auto offset = circle.center - center;

        // Angular velocity
        const auto omega = object.getAngularVelocity();

        // Calculate shape global velocity
        const auto velocity = object.getVelocity() + cross(omega, offset);

        // Difference between environment velocity and shape velocity
        const auto dv = velocityEnv - velocity;

        // Same velocity
        if (dv == Zero)
            continue;

        // Add force from shape
        force += forceCoefficient * radius * dv;
    }

    Assert(object.getShapes().size() > 0);
    velocityObjEnv /= object.getShapes().size();

    // Difference between velocities
    const auto dv = velocityObjEnv - object.getVelocity();

    // Same velocity
    if (dv == Zero)
        return;

    // Maximum impulse
    const auto impulseMax = object.getMass() * dv;

    // Calculate linear impulse from shapes
    auto impulse = force * dt;

    // Impulse is to big
    if (impulse.getLengthSquared() > impulseMax.getLengthSquared())
    {
        const RealType ratio = impulseMax.getLength() / impulse.getLength();
        impulse *= ratio;
    }

    // Apply impulse
    object.applyLinearImpulse(impulse);
}

/* ************************************************************************ */

void Module::applyBoundaryConditions(const simulator::Simulation& simulation, const VelocityVector& vMax)
{
    // Init boundaries
    for (unsigned int pos = 0; pos < LayoutPosCount; ++pos)
        initBorderInletOutlet(simulation, static_cast<LayoutPosition>(pos), vMax);
}

/* ************************************************************************ */

VelocityVector Module::inletVelocityProfile(
    Lattice::CoordinateType coord, LayoutPosition pos,
    DynamicArray<StaticArray<Lattice::CoordinateType, 2>> inlets
) const noexcept
{
    //return {getInletVelocities()[pos], Zero};

    StaticArray<Lattice::CoordinateType, 2> inletRange{{Zero, Zero}};
    bool found = false;

    switch (pos)
    {
    case LayoutPosTop:
    case LayoutPosBottom:
        // Find inlet range
        for (const auto& inlet : inlets)
        {
            if (inlet[0].getX() <= coord.getX() && coord.getX() <= inlet[1].getX())
            {
                inletRange = inlet;
                found = true;
                break;
            }
        }
        break;

    case LayoutPosLeft:
    case LayoutPosRight:
        // Find inlet range
        for (const auto& inlet : inlets)
        {
            if (inlet[0].getY() <= coord.getY() && coord.getY() <= inlet[1].getY())
            {
                inletRange = inlet;
                found = true;
                break;
            }
        }
        break;

    default:
        Assert(false);
        break;
    }

    if (!found)
        return Zero;

    switch (pos)
    {
    case LayoutPosTop:
        return {
            Zero,
            -calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getX() - inletRange[0].getX(),
                (inletRange[1] - inletRange[0]).getWidth() + 1
            )
        };
        break;

    case LayoutPosBottom:
        return {
            Zero,
            calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getX() - inletRange[0].getX(),
                (inletRange[1] - inletRange[0]).getWidth() + 1
            )
        };
        break;

    case LayoutPosRight:
        return {
            -calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getY() - inletRange[0].getY(),
                (inletRange[1] - inletRange[0]).getHeight() + 1
            ),
            Zero
        };
        break;

    case LayoutPosLeft:
        return {
            calcPoiseuilleFlow(
                getInletVelocities()[pos],
                coord.getY() - inletRange[0].getY(),
                (inletRange[1] - inletRange[0]).getHeight() + 1
            ),
            Zero
        };
        break;

    default:
        Assert(false);
        break;
    }

    return Zero;
}

/* ************************************************************************ */

RealType Module::calculateCoefficient(units::Time dt, PositionVector dl) const noexcept
{
    return calculateViscosity() * (getInnerIterations() * dl.getX() * dl.getY()) / getKinematicViscosity() / dt;
}

/* ************************************************************************ */

void Module::initBorderBarrier(simulator::Simulation& simulation, LayoutPosition pos)
{
    const auto& size = m_lattice.getSize();
    const auto worldSizeHalf = simulation.getWorldSize() * 0.5;

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / size;

    // Create barrier
    auto obstacle = simulation.buildObject("obstacle.Polygon");
    auto& shapes = obstacle->getMutableShapes();
    shapes.resize(1);

    // Init shape
    shapes[0] = Shape::makeEdges({
        EDGES[pos][0] * (worldSizeHalf - 0.5 * dl),
        EDGES[pos][1] * (worldSizeHalf - 0.5 * dl)
    });
    obstacle->initShapes();

    // Store obstacle view
    m_layoutBarriers[pos] = obstacle;

    Vector<Lattice::SizeType> rngMin;
    Vector<Lattice::SizeType> rngMax;

    switch (pos)
    {
    case LayoutPosTop:
        rngMin = {0, size.getHeight() - 1};
        rngMax = {size.getWidth(), size.getHeight()};
        break;

    case LayoutPosBottom:
        rngMin = {0, 0};
        rngMax = {size.getWidth(), 1};
        break;

    case LayoutPosRight:
        rngMin = {size.getWidth() - 1, 0};
        rngMax = {size.getWidth(), size.getHeight()};
        break;

    case LayoutPosLeft:
        rngMin = {0, 0};
        rngMax = {1, size.getHeight()};
        break;

    default:
        break;
    }

    // Set obstacles
    for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
            m_lattice[{x, y}].setStaticObstacleDynamics();
}

/* ************************************************************************ */

void Module::initBorderInletOutlet(const simulator::Simulation& simulation,
    LayoutPosition pos, const VelocityVector& vMax)
{
    const auto& size = m_lattice.getSize();

    Vector<Lattice::SizeType> rngMin;
    Vector<Lattice::SizeType> rngMax;
    LatticeCell::Position dir;

    // Detect multiple inlets at the layout position
    DynamicArray<StaticArray<Lattice::CoordinateType, 2>> inlets;

    switch (pos)
    {
    case LayoutPosTop:
        rngMin = {0, size.getHeight() - 1};
        rngMax = {size.getWidth(), size.getHeight()};
        dir = LatticeCell::PositionBottom;

        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
        {
            const Lattice::CoordinateType c1 = {x, size.getHeight() - 1};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++x;
            for (; x < rngMax.getX(); ++x)
            {
                const Lattice::CoordinateType cNext = {x, size.getHeight() - 1};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getX() - c2.getX() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    case LayoutPosBottom:
        rngMin = {0, 0};
        rngMax = {size.getWidth(), 1};
        dir = LatticeCell::PositionTop;

        for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
        {
            const Lattice::CoordinateType c1 = {x, 0};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++x;
            for (; x < rngMax.getX(); ++x)
            {
                const Lattice::CoordinateType cNext = {x, 0};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getX() - c2.getX() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    case LayoutPosRight:
        rngMin = {size.getWidth() - 1, 0};
        rngMax = {size.getWidth(), size.getHeight()};
        dir = LatticeCell::PositionLeft;

        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        {
            const Lattice::CoordinateType c1 = {size.getWidth() - 1, y};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++y;
            for (; y < rngMax.getY(); ++y)
            {
                const Lattice::CoordinateType cNext = {size.getWidth() - 1, y};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getY() - c2.getY() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    case LayoutPosLeft:
        rngMin = {0, 0};
        rngMax = {1, size.getHeight()};
        dir = LatticeCell::PositionRight;

        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        {
            const Lattice::CoordinateType c1 = {0, y};
            if (m_lattice[c1].hasStaticObstacleDynamics())
                continue;

            auto c2 = c1;

            ++y;
            for (; y < rngMax.getY(); ++y)
            {
                const Lattice::CoordinateType cNext = {0, y};
                if (m_lattice[cNext].hasStaticObstacleDynamics())
                    break;

                c2 = cNext;
            }

            if (c1.getY() - c2.getY() > 1)
                inlets.push_back({{c1, c2}});
        }

        break;

    default:
        throw RuntimeException("Invalid layout position");
    }

    if (m_layout[pos] == LayoutType::Outlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
                if (!m_lattice[{x, y}].hasObstacleDynamics())
                    m_lattice[{x, y}].initOutlet(dir, 1.0);
    }
    else if (m_layout[pos] == LayoutType::Inlet)
    {
        for (auto y = rngMin.getY(); y < rngMax.getY(); ++y)
        {
            for (auto x = rngMin.getX(); x < rngMax.getX(); ++x)
            {
                if (!m_lattice[{x, y}].hasObstacleDynamics())
                    m_lattice[{x, y}].initInlet(dir, inletVelocityProfile({x, y}, pos, inlets) / vMax);
            }
        }
    }
}

/* ************************************************************************ */

void Module::storeToFile(const FilePath& filename)
{
    OutFileStream ofs(filename.string(), OutFileStream::binary);
    BinaryOutput out(ofs);

    // Write file guard
    out.write(FILE_GUARD);

    // Write lattice size
    out.write(m_lattice.getSize());

    // Number of init iterations
    out.write(m_initIterations);

    for (auto&& c : range(m_lattice.getSize()))
    {
        const LatticeCell& cell = m_lattice[c];

        // Write cell populations
        out.write(cell.getValues());
    }
}

/* ************************************************************************ */

void Module::loadFromFile(const FilePath& filename)
{
    InFileStream ifs(filename.string(), OutFileStream::binary);

    if (!ifs.is_open())
        throw InvalidArgumentException("[streamlines] Cannot load from file: File not found '" + filename.string() + "'");

    BinaryInput in(ifs);

    // Read guard
    StaticArray<char, FILE_GUARD.size()> guard;
    in.read(guard);

    if (guard != FILE_GUARD)
        throw InvalidArgumentException("[streamlines] Cannot load from file: File is not valid");

    // Read lattice size
    Lattice::Size size;
    in.read(size);

    if (size != m_lattice.getSize())
        throw InvalidArgumentException("[streamlines] Cannot load from file: different lattice sizes");

    decltype(m_initIterations) iterations;
    in.read(iterations);

    if (iterations != m_initIterations)
        throw InvalidArgumentException("[streamlines] Cannot load from file: different init iterations");

    for (auto&& c : range(m_lattice.getSize()))
    {
        LatticeCell& cell = m_lattice[c];

        // Read cell populations
        in.read(cell.getValues());
    }
}

/* ************************************************************************ */

InStream& operator>>(InStream& is, Module::LayoutType& type)
{
    String desc;
    is >> desc;

    if (desc == "none")
        type = Module::LayoutType::None;
    else if (desc == "barrier")
        type = Module::LayoutType::Barrier;
    else if (desc == "inlet")
        type = Module::LayoutType::Inlet;
    else if (desc == "outlet")
        type = Module::LayoutType::Outlet;
    else
        throw InvalidArgumentException("Unknown layout type");

    return is;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
