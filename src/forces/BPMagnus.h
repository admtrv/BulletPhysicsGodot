/*
 * BPMagnus.h
 */

#pragma once

#include "BPForce.h"
#include "ballistics/external/forces/SpinDrift.h"

namespace godot {

class BPMagnus : public BPForce {
    GDCLASS(BPMagnus, BPForce)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::forces::IForce> createForce() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::forces::Magnus>();
    }

protected:
    static void _bind_methods() {}
};

}
