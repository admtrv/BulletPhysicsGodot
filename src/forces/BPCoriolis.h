/*
 * BPCoriolis.h
 */

#pragma once

#include "BPForce.h"
#include "ballistics/external/forces/Coriolis.h"

namespace godot {

class BPCoriolis : public BPForce {
    GDCLASS(BPCoriolis, BPForce)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::forces::IForce> createForce() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::forces::Coriolis>();
    }

protected:
    static void _bind_methods() {}
};

}
