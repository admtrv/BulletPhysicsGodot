/*
 * BPEnvironment.h
 */

#pragma once

#include <godot_cpp/classes/node.hpp>

#include "ballistics/external/environments/Environment.h"

#include <memory>

namespace godot {

// Base class for environment nodes. Add as child of BPPhysicsWorld.
class BPEnvironment : public Node {
    GDCLASS(BPEnvironment, Node)

public:
    BPEnvironment() = default;
    ~BPEnvironment() = default;

    virtual std::unique_ptr<BulletPhysics::ballistics::external::environments::IEnvironment> createEnvironment() const = 0;

protected:
    static void _bind_methods() {}
};

}
