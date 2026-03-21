/*
 * BPGeographic.h
 */

#pragma once

#include "BPEnvironment.h"
#include "ballistics/external/environments/Geographic.h"
#include "math/Angles.h"

namespace godot {

class BPGeographic : public BPEnvironment {
    GDCLASS(BPGeographic, BPEnvironment)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::environments::IEnvironment> createEnvironment() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::environments::Geographic>(
            BulletPhysics::math::deg2rad(m_latitude),
            BulletPhysics::math::deg2rad(m_longitude),
            0.0);
    }

    void set_latitude(double lat) { m_latitude = lat; }
    double get_latitude() const { return m_latitude; }

    void set_longitude(double lon) { m_longitude = lon; }
    double get_longitude() const { return m_longitude; }

protected:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("set_latitude", "latitude"), &BPGeographic::set_latitude);
        ClassDB::bind_method(D_METHOD("get_latitude"), &BPGeographic::get_latitude);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "latitude", PROPERTY_HINT_RANGE, "-90,90,0.1"), "set_latitude", "get_latitude");

        ClassDB::bind_method(D_METHOD("set_longitude", "longitude"), &BPGeographic::set_longitude);
        ClassDB::bind_method(D_METHOD("get_longitude"), &BPGeographic::get_longitude);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "longitude", PROPERTY_HINT_RANGE, "-180,180,0.1"), "set_longitude", "get_longitude");
    }

private:
    double m_latitude = 0.0;
    double m_longitude = 0.0;
};

}
