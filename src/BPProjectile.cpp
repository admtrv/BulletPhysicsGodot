/*
 * BPProjectile.cpp
 */

#include "BPProjectile.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;
using namespace BulletPhysics;

BPProjectile::BPProjectile() {}
BPProjectile::~BPProjectile() {}

void BPProjectile::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("fire", "elevation_deg", "azimuth_deg"), &BPProjectile::fire);

    // projectile group
    ADD_GROUP("Projectile", "projectile_");

    ClassDB::bind_method(D_METHOD("set_projectile_mass", "mass"), &BPProjectile::set_projectile_mass);
    ClassDB::bind_method(D_METHOD("get_projectile_mass"), &BPProjectile::get_projectile_mass);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_mass", PROPERTY_HINT_RANGE, "0.001,1.0,0.001"), "set_projectile_mass", "get_projectile_mass");

    ClassDB::bind_method(D_METHOD("set_diameter", "diameter"), &BPProjectile::set_diameter);
    ClassDB::bind_method(D_METHOD("get_diameter"), &BPProjectile::get_diameter);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_diameter", PROPERTY_HINT_RANGE, "0.001,0.1,0.0001"), "set_diameter", "get_diameter");

    ClassDB::bind_method(D_METHOD("set_drag_model", "model"), &BPProjectile::set_drag_model);
    ClassDB::bind_method(D_METHOD("get_drag_model"), &BPProjectile::get_drag_model);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "projectile_drag_model", PROPERTY_HINT_ENUM, "G1,G2,G5,G6,G7,G8,GL"), "set_drag_model", "get_drag_model");

    // muzzle group
    ADD_GROUP("Muzzle", "muzzle_");

    ClassDB::bind_method(D_METHOD("set_muzzle_velocity", "velocity"), &BPProjectile::set_muzzle_velocity);
    ClassDB::bind_method(D_METHOD("get_muzzle_velocity"), &BPProjectile::get_muzzle_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "muzzle_velocity", PROPERTY_HINT_RANGE, "1,2000,1"), "set_muzzle_velocity", "get_muzzle_velocity");

    ClassDB::bind_method(D_METHOD("set_rifling_direction", "direction"), &BPProjectile::set_rifling_direction);
    ClassDB::bind_method(D_METHOD("get_rifling_direction"), &BPProjectile::get_rifling_direction);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "muzzle_rifling_direction", PROPERTY_HINT_ENUM, "Right,Left"), "set_rifling_direction", "get_rifling_direction");

    ClassDB::bind_method(D_METHOD("set_twist_rate", "rate"), &BPProjectile::set_twist_rate);
    ClassDB::bind_method(D_METHOD("get_twist_rate"), &BPProjectile::get_twist_rate);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "muzzle_twist_rate", PROPERTY_HINT_RANGE, "1,30,0.1"), "set_twist_rate", "get_twist_rate");

    // trajectory
    ClassDB::bind_method(D_METHOD("get_trajectory"), &BPProjectile::get_trajectory);
    ClassDB::bind_method(D_METHOD("is_active"), &BPProjectile::is_active);

    ADD_SIGNAL(MethodInfo("trajectory_updated"));
    ADD_SIGNAL(MethodInfo("projectile_landed"));
}

void BPProjectile::fire(double elevationDeg, double azimuthDeg)
{
    // build specs
    auto specs = projectile::ProjectileSpecs::create(m_mass, m_diameter)
        .withDragModel(static_cast<ballistics::external::forces::drag::DragCurveModel>(m_dragModel))
        .withMuzzle(m_muzzleVelocity,
            m_riflingDirection == 0 ? projectile::Direction::RIGHT : projectile::Direction::LEFT,
            m_twistRate);

    // create adapter over self (this is a RigidBody3D)
    m_body = std::make_unique<GodotRigidBody>(this, specs);

    // initial velocity
    double elev = math::deg2rad(elevationDeg);
    double azim = math::deg2rad(azimuthDeg);
    double ce = std::cos(elev);
    double se = std::sin(elev);
    double sa = std::sin(azim);
    double ca = std::cos(azim);
    m_body->setVelocity({ce * sa * m_muzzleVelocity, se * m_muzzleVelocity, ce * ca * m_muzzleVelocity});

    // clear trajectory
    m_trajectory.clear();
    m_trajectory.push_back(get_position());

    m_active = true;
}

void BPProjectile::step(BulletPhysics::math::IIntegrator& integrator, BulletPhysics::ballistics::external::PhysicsWorld& world, double delta)
{
    if (!m_active || !m_body)
    {
        return;
    }

    integrator.step(*m_body, &world, delta);

    math::Vec3 pos = m_body->getPosition();
    m_trajectory.push_back(Vector3(pos.x, pos.y, pos.z));

    emit_signal("trajectory_updated");

    if (pos.y <= 0.0 && m_trajectory.size() > 2)
    {
        m_active = false;
        emit_signal("projectile_landed");
    }
}

void BPProjectile::set_projectile_mass(double mass) { m_mass = mass; }
double BPProjectile::get_projectile_mass() const { return m_mass; }

void BPProjectile::set_diameter(double diameter) { m_diameter = diameter; }
double BPProjectile::get_diameter() const { return m_diameter; }

void BPProjectile::set_drag_model(int model) { m_dragModel = model; }
int BPProjectile::get_drag_model() const { return m_dragModel; }

void BPProjectile::set_muzzle_velocity(double vel) { m_muzzleVelocity = vel; }
double BPProjectile::get_muzzle_velocity() const { return m_muzzleVelocity; }

void BPProjectile::set_rifling_direction(int dir) { m_riflingDirection = dir; }
int BPProjectile::get_rifling_direction() const { return m_riflingDirection; }

void BPProjectile::set_twist_rate(double rate) { m_twistRate = rate; }
double BPProjectile::get_twist_rate() const { return m_twistRate; }

PackedVector3Array BPProjectile::get_trajectory() const { return m_trajectory; }
