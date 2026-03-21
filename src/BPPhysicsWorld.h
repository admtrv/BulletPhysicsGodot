/*
 * BPPhysicsWorld.h
 */

#pragma once

#include <godot_cpp/classes/node3d.hpp>

#include "ballistics/external/PhysicsWorld.h"
#include "math/Integrator.h"

namespace godot {

// Godot node wrapping PhysicsWorld (BulletPhysics). Forces and environments are added as child nodes.
class BPPhysicsWorld : public Node3D {
    GDCLASS(BPPhysicsWorld, Node3D)

public:
    BPPhysicsWorld();
    ~BPPhysicsWorld();

    void _ready() override;
    void _physics_process(double delta) override;

    // access internal world
    BulletPhysics::ballistics::external::PhysicsWorld& getWorld() { return m_world; }

    // integrator
    void set_integrator(int type);
    int get_integrator() const;

    // rebuild forces/environments from children
    void rebuild();

protected:
    static void _bind_methods();

private:
    BulletPhysics::ballistics::external::PhysicsWorld m_world;
    std::unique_ptr<BulletPhysics::math::IIntegrator> m_integrator;
    int m_integratorType = 2; // 0=Euler, 1=Midpoint, 2=RK4

    std::unique_ptr<BulletPhysics::math::IIntegrator> createIntegrator() const;
    void findAndStepProjectiles(Node* node, double delta);
};

}
