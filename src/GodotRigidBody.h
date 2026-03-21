/*
 * GodotRigidBody.h
 */

#pragma once

#include "PhysicsBody.h"

#include <godot_cpp/classes/rigid_body3d.hpp>

// IProjectileBody (BulletPhysics) adapter over RigidBody3D (Godot)
class GodotRigidBody : public BulletPhysics::projectile::IProjectileBody {
public:
    GodotRigidBody(godot::RigidBody3D* body, const BulletPhysics::projectile::ProjectileSpecs& specs);

    std::unique_ptr<BulletPhysics::IPhysicsBody> clone() const override;

    // mass
    double getMass() const override;

    // position
    BulletPhysics::math::Vec3 getPosition() const override;
    void setPosition(const BulletPhysics::math::Vec3& pos) override;

    // velocity
    BulletPhysics::math::Vec3 getVelocity() const override;
    void setVelocity(const BulletPhysics::math::Vec3& vel) override;

    // forces
    const BulletPhysics::math::Vec3& getAccumulatedForces() const override;
    void addForce(const BulletPhysics::math::Vec3& force) override;
    void clearForces() override;

    // projectile specs
    const BulletPhysics::projectile::ProjectileSpecs& getProjectileSpecs() const override;

private:
    godot::RigidBody3D* m_body;
    BulletPhysics::projectile::ProjectileSpecs m_specs;
    BulletPhysics::math::Vec3 m_forces{};
    BulletPhysics::math::Vec3 m_velocity{};
    BulletPhysics::math::Vec3 m_position{};
};
