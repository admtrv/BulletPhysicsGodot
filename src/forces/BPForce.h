/*
 * BPForce.h
 */

#pragma once

#include <godot_cpp/classes/node.hpp>

#include "ballistics/external/forces/Force.h"

#include <memory>

namespace godot {

// Base class for force nodes. Add as child of BPPhysicsWorld.
class BPForce : public Node {
    GDCLASS(BPForce, Node)

public:
    BPForce() = default;
    ~BPForce() = default;

    virtual std::unique_ptr<BulletPhysics::ballistics::external::forces::IForce> createForce() const = 0;

protected:
    static void _bind_methods() {}
};

}
