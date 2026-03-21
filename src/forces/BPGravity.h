/*
 * BPGravity.h
 */

#pragma once

#include "BPForce.h"
#include "ballistics/external/forces/Gravity.h"

namespace godot {

class BPGravity : public BPForce {
    GDCLASS(BPGravity, BPForce)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::forces::IForce> createForce() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::forces::Gravity>();
    }

protected:
    static void _bind_methods() {}
};

}
