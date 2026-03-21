/*
 * BPHumidity.h
 */

#pragma once

#include "BPEnvironment.h"
#include "ballistics/external/environments/Humidity.h"

namespace godot {

class BPHumidity : public BPEnvironment {
    GDCLASS(BPHumidity, BPEnvironment)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::environments::IEnvironment> createEnvironment() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::environments::Humidity>(m_humidity);
    }

    void set_humidity(double h) { m_humidity = h; }
    double get_humidity() const { return m_humidity; }

protected:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("set_humidity", "humidity"), &BPHumidity::set_humidity);
        ClassDB::bind_method(D_METHOD("get_humidity"), &BPHumidity::get_humidity);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "humidity", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_humidity", "get_humidity");
    }

private:
    double m_humidity = 50.0;
};

}
