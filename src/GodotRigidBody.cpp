/*
 * GodotRigidBody.cpp
 */

#include "GodotRigidBody.h"

GodotRigidBody::GodotRigidBody(godot::RigidBody3D* body, const BulletPhysics::projectile::ProjectileSpecs& specs)
    : m_body(body), m_specs(specs)
{
    if (m_body)
    {
        godot::Vector3 pos = m_body->get_position();
        m_position = {pos.x, pos.y, pos.z};
    }
}

std::unique_ptr<BulletPhysics::IPhysicsBody> GodotRigidBody::clone() const
{
    auto copy = std::make_unique<GodotRigidBody>(m_body, m_specs);
    copy->m_velocity = m_velocity;
    copy->m_position = m_position;
    copy->m_forces = m_forces;
    return copy;
}

double GodotRigidBody::getMass() const
{
    return m_specs.mass;
}

BulletPhysics::math::Vec3 GodotRigidBody::getPosition() const
{
    return m_position;
}

void GodotRigidBody::setPosition(const BulletPhysics::math::Vec3& pos)
{
    m_position = pos;
    if (m_body)
    {
        m_body->set_position(godot::Vector3(pos.x, pos.y, pos.z));
    }
}

BulletPhysics::math::Vec3 GodotRigidBody::getVelocity() const
{
    return m_velocity;
}

void GodotRigidBody::setVelocity(const BulletPhysics::math::Vec3& vel)
{
    m_velocity = vel;
}

const BulletPhysics::math::Vec3& GodotRigidBody::getAccumulatedForces() const
{
    return m_forces;
}

void GodotRigidBody::addForce(const BulletPhysics::math::Vec3& force)
{
    m_forces += force;
}

void GodotRigidBody::clearForces()
{
    m_forces = BulletPhysics::math::Vec3{};
}

const BulletPhysics::projectile::ProjectileSpecs& GodotRigidBody::getProjectileSpecs() const
{
    return m_specs;
}
