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
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_mass", PROPERTY_HINT_RANGE, "0.001,1000.0,0.001,suffix:kg"), "set_projectile_mass", "get_projectile_mass");

    ClassDB::bind_method(D_METHOD("set_diameter", "diameter"), &BPProjectile::set_diameter);
    ClassDB::bind_method(D_METHOD("get_diameter"), &BPProjectile::get_diameter);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_diameter", PROPERTY_HINT_RANGE, "0.00001,1.0,0.00001,suffix:m"), "set_diameter", "get_diameter");

    ClassDB::bind_method(D_METHOD("set_drag_model", "model"), &BPProjectile::set_drag_model);
    ClassDB::bind_method(D_METHOD("get_drag_model"), &BPProjectile::get_drag_model);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "projectile_drag_model", PROPERTY_HINT_ENUM, "G1,G2,G5,G6,G7,G8,GL,Custom"), "set_drag_model", "get_drag_model");

    ClassDB::bind_method(D_METHOD("set_custom_drag_coefficient", "cd"), &BPProjectile::set_custom_drag_coefficient);
    ClassDB::bind_method(D_METHOD("get_custom_drag_coefficient"), &BPProjectile::get_custom_drag_coefficient);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_drag_coefficient", PROPERTY_HINT_RANGE, "0.0,10.0,0.01"), "set_custom_drag_coefficient", "get_custom_drag_coefficient");

    // muzzle group
    ADD_GROUP("Muzzle", "muzzle_");

    ClassDB::bind_method(D_METHOD("set_muzzle_velocity", "velocity"), &BPProjectile::set_muzzle_velocity);
    ClassDB::bind_method(D_METHOD("get_muzzle_velocity"), &BPProjectile::get_muzzle_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "muzzle_velocity", PROPERTY_HINT_RANGE, "0.0,1000.0,0.01,suffix:m/s"), "set_muzzle_velocity", "get_muzzle_velocity");

    ClassDB::bind_method(D_METHOD("set_rifling_direction", "direction"), &BPProjectile::set_rifling_direction);
    ClassDB::bind_method(D_METHOD("get_rifling_direction"), &BPProjectile::get_rifling_direction);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "muzzle_rifling_direction", PROPERTY_HINT_ENUM, "Right,Left"), "set_rifling_direction", "get_rifling_direction");

    ClassDB::bind_method(D_METHOD("set_twist_rate", "rate"), &BPProjectile::set_twist_rate);
    ClassDB::bind_method(D_METHOD("get_twist_rate"), &BPProjectile::get_twist_rate);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "muzzle_twist_rate", PROPERTY_HINT_RANGE, "0.1,100,0.1,suffix:cal/turn"), "set_twist_rate", "get_twist_rate");

    // advanced group
    ADD_GROUP("Advanced", "advanced_");

    ClassDB::bind_method(D_METHOD("set_overtuning_coefficient", "value"), &BPProjectile::set_overtuning_coefficient);
    ClassDB::bind_method(D_METHOD("get_overtuning_coefficient"), &BPProjectile::get_overtuning_coefficient);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "advanced_overtuning_coefficient", PROPERTY_HINT_RANGE, "0.0,10.0,0.01"), "set_overtuning_coefficient", "get_overtuning_coefficient");

    ClassDB::bind_method(D_METHOD("set_lift_coefficient", "value"), &BPProjectile::set_lift_coefficient);
    ClassDB::bind_method(D_METHOD("get_lift_coefficient"), &BPProjectile::get_lift_coefficient);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "advanced_lift_coefficient", PROPERTY_HINT_RANGE, "0.0,10.0,0.01"), "set_lift_coefficient", "get_lift_coefficient");

    ClassDB::bind_method(D_METHOD("set_magnus_coefficient", "value"), &BPProjectile::set_magnus_coefficient);
    ClassDB::bind_method(D_METHOD("get_magnus_coefficient"), &BPProjectile::get_magnus_coefficient);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "advanced_magnus_coefficient", PROPERTY_HINT_RANGE, "0.0,10.0,0.01"), "set_magnus_coefficient", "get_magnus_coefficient");

    ClassDB::bind_method(D_METHOD("is_active"), &BPProjectile::is_active);

    ADD_SIGNAL(MethodInfo("projectile_landed"));
}

void BPProjectile::_validate_property(PropertyInfo& p_property) const
{
    if (p_property.name == StringName("projectile_drag_coefficient")
        && m_dragModel != static_cast<int>(ballistics::external::forces::drag::DragCurveModel::CUSTOM))
    {
        p_property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
}

void BPProjectile::fire(double elevationDeg, double azimuthDeg)
{
    // build specs
    auto specs = projectile::ProjectileSpecs::create(m_mass, m_diameter);

    if (m_dragModel == static_cast<int>(ballistics::external::forces::drag::DragCurveModel::CUSTOM))
    {
        specs.withCustomDragCoefficient(m_customDragCoefficient);
    }
    else
    {
        specs.withDragModel(static_cast<ballistics::external::forces::drag::DragCurveModel>(m_dragModel));
    }

    specs.withMuzzle(m_muzzleVelocity,
        m_riflingDirection == 0 ? projectile::Direction::RIGHT : projectile::Direction::LEFT,
        m_twistRate);

    specs.withOvertuningCoefficient(m_overtuningCoefficient);
    specs.withLiftCoefficient(m_liftCoefficient);
    specs.withMagnusCoefficient(m_magnusCoefficient);

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

    if (pos.y <= 0.0)
    {
        m_active = false;
        emit_signal("projectile_landed");
    }
}

void BPProjectile::set_projectile_mass(double mass) { m_mass = mass; }
double BPProjectile::get_projectile_mass() const { return m_mass; }

void BPProjectile::set_diameter(double diameter) { m_diameter = diameter; }
double BPProjectile::get_diameter() const { return m_diameter; }

void BPProjectile::set_drag_model(int model) { m_dragModel = model; notify_property_list_changed(); }
int BPProjectile::get_drag_model() const { return m_dragModel; }

void BPProjectile::set_custom_drag_coefficient(double cd) { m_customDragCoefficient = cd; }
double BPProjectile::get_custom_drag_coefficient() const { return m_customDragCoefficient; }

void BPProjectile::set_muzzle_velocity(double vel) { m_muzzleVelocity = vel; }
double BPProjectile::get_muzzle_velocity() const { return m_muzzleVelocity; }

void BPProjectile::set_rifling_direction(int dir) { m_riflingDirection = dir; }
int BPProjectile::get_rifling_direction() const { return m_riflingDirection; }

void BPProjectile::set_twist_rate(double rate) { m_twistRate = rate; }
double BPProjectile::get_twist_rate() const { return m_twistRate; }

void BPProjectile::set_overtuning_coefficient(double val) { m_overtuningCoefficient = val; }
double BPProjectile::get_overtuning_coefficient() const { return m_overtuningCoefficient; }

void BPProjectile::set_lift_coefficient(double val) { m_liftCoefficient = val; }
double BPProjectile::get_lift_coefficient() const { return m_liftCoefficient; }

void BPProjectile::set_magnus_coefficient(double val) { m_magnusCoefficient = val; }
double BPProjectile::get_magnus_coefficient() const { return m_magnusCoefficient; }

