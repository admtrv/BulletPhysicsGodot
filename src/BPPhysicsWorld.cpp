/*
 * BPPhysicsWorld.cpp
 */

#include "BPPhysicsWorld.h"
#include "forces/BPForce.h"
#include "environments/BPEnvironment.h"
#include "BPProjectile.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

using namespace godot;

BPPhysicsWorld::BPPhysicsWorld()
{
    m_integrator = createIntegrator();
}

BPPhysicsWorld::~BPPhysicsWorld() {}

void BPPhysicsWorld::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_integrator", "type"), &BPPhysicsWorld::set_integrator);
    ClassDB::bind_method(D_METHOD("get_integrator"), &BPPhysicsWorld::get_integrator);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "integrator", PROPERTY_HINT_ENUM, "Euler,Midpoint,RK4"), "set_integrator", "get_integrator");

    ClassDB::bind_method(D_METHOD("rebuild"), &BPPhysicsWorld::rebuild);
}

void BPPhysicsWorld::_ready()
{
    rebuild();
}

void BPPhysicsWorld::rebuild()
{
    m_world.clear();
    m_integrator = createIntegrator();

    for (int i = 0; i < get_child_count(); i++)
    {
        Node* child = get_child(i);

        auto* force = Object::cast_to<BPForce>(child);
        if (force)
        {
            auto f = force->createForce();
            if (f)
            {
                m_world.addForce(std::move(f));
            }
            continue;
        }

        auto* env = Object::cast_to<BPEnvironment>(child);
        if (env)
        {
            auto e = env->createEnvironment();
            if (e)
            {
                m_world.addEnvironment(std::move(e));
            }
        }
    }
}

void BPPhysicsWorld::_physics_process(double delta)
{
    if (!m_integrator)
    {
        return;
    }

    // find all BPProjectile in scene tree
    findAndStepProjectiles(static_cast<Node*>(get_tree()->get_root()), delta);
}

void BPPhysicsWorld::findAndStepProjectiles(Node* node, double delta)
{
    if (!node)
    {
        return;
    }

    auto* proj = Object::cast_to<BPProjectile>(node);
    if (proj && proj->is_active())
    {
        proj->step(*m_integrator, m_world, delta);
    }

    for (int i = 0; i < node->get_child_count(); i++)
    {
        findAndStepProjectiles(node->get_child(i), delta);
    }
}

std::unique_ptr<BulletPhysics::math::IIntegrator> BPPhysicsWorld::createIntegrator() const
{
    switch (m_integratorType)
    {
        case 0: return std::make_unique<BulletPhysics::math::EulerIntegrator>();
        case 1: return std::make_unique<BulletPhysics::math::MidpointIntegrator>();
        default: return std::make_unique<BulletPhysics::math::RK4Integrator>();
    }
}

void BPPhysicsWorld::set_integrator(int type) { m_integratorType = type; }
int BPPhysicsWorld::get_integrator() const { return m_integratorType; }
