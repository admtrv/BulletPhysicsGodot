/*
 * BPWind.h
 */

#pragma once

#include "BPEnvironment.h"
#include "ballistics/external/environments/Wind.h"

namespace godot {

class BPWind : public BPEnvironment {
    GDCLASS(BPWind, BPEnvironment)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::environments::IEnvironment> createEnvironment() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::environments::Wind>(
            BulletPhysics::math::Vec3(m_wind.x, m_wind.y, m_wind.z));
    }

    void set_wind(Vector3 w) { m_wind = w; }
    Vector3 get_wind() const { return m_wind; }

protected:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("set_wind", "wind"), &BPWind::set_wind);
        ClassDB::bind_method(D_METHOD("get_wind"), &BPWind::get_wind);
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "wind"), "set_wind", "get_wind");
    }

private:
    Vector3 m_wind = Vector3(0.0, 0.0, 0.0);
};

}
