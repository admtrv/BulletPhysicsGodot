/*
 * BPLift.h
 */

#pragma once

#include "BPForce.h"
#include "ballistics/external/forces/SpinDrift.h"

namespace godot {

class BPLift : public BPForce {
    GDCLASS(BPLift, BPForce)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::forces::IForce> createForce() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::forces::Lift>();
    }

protected:
    static void _bind_methods() {}
};

}
