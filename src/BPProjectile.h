/*
 * BPProjectile.h
 */

#pragma once

#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>

#include "GodotRigidBody.h"

#include "ballistics/external/PhysicsWorld.h"
#include "math/Integrator.h"
#include "math/Angles.h"

namespace godot {

// Extends RigidBody3D (Godot) with projectile simulation (BulletPhysics). Add mesh, collider etc. as children.
class BPProjectile : public RigidBody3D {
    GDCLASS(BPProjectile, RigidBody3D)

public:
    BPProjectile();
    ~BPProjectile();

    // fire with angles (speed from muzzle_velocity)
    void fire(double elevationDeg, double azimuthDeg);

    // step simulation (called by BPPhysicsWorld)
    void step(BulletPhysics::math::IIntegrator& integrator, BulletPhysics::ballistics::external::PhysicsWorld& world, double delta);

    bool is_active() const { return m_active; }

    // projectile specs
    void set_projectile_mass(double mass);
    double get_projectile_mass() const;

    void set_diameter(double diameter);
    double get_diameter() const;

    void set_drag_model(int model);
    int get_drag_model() const;

    // muzzle specs
    void set_muzzle_velocity(double vel);
    double get_muzzle_velocity() const;

    void set_rifling_direction(int dir);
    int get_rifling_direction() const;

    void set_twist_rate(double rate);
    double get_twist_rate() const;

    // trajectory
    PackedVector3Array get_trajectory() const;

protected:
    static void _bind_methods();

private:
    bool m_active = false;

    // projectile
    double m_mass = 0.0095;
    double m_diameter = 0.00762;
    int m_dragModel = 4; // G7

    // muzzle
    double m_muzzleVelocity = 800.0;
    int m_riflingDirection = 0; // 0=RIGHT, 1=LEFT
    double m_twistRate = 12.0;

    // physics adapter
    std::unique_ptr<GodotRigidBody> m_body;

    // trajectory
    PackedVector3Array m_trajectory;
};

}
