/*
 * BPAtmosphere.h
 */

#pragma once

#include "BPEnvironment.h"
#include "ballistics/external/environments/Atmosphere.h"

namespace godot {

class BPAtmosphere : public BPEnvironment {
    GDCLASS(BPAtmosphere, BPEnvironment)

public:
    std::unique_ptr<BulletPhysics::ballistics::external::environments::IEnvironment> createEnvironment() const override
    {
        return std::make_unique<BulletPhysics::ballistics::external::environments::Atmosphere>(
            m_baseTemperature, m_basePressure);
    }

    void set_base_temperature(double t) { m_baseTemperature = t; }
    double get_base_temperature() const { return m_baseTemperature; }

    void set_base_pressure(double p) { m_basePressure = p; }
    double get_base_pressure() const { return m_basePressure; }

protected:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("set_base_temperature", "temperature"), &BPAtmosphere::set_base_temperature);
        ClassDB::bind_method(D_METHOD("get_base_temperature"), &BPAtmosphere::get_base_temperature);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_temperature", PROPERTY_HINT_RANGE, "200,350,0.1"), "set_base_temperature", "get_base_temperature");

        ClassDB::bind_method(D_METHOD("set_base_pressure", "pressure"), &BPAtmosphere::set_base_pressure);
        ClassDB::bind_method(D_METHOD("get_base_pressure"), &BPAtmosphere::get_base_pressure);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_pressure", PROPERTY_HINT_RANGE, "80000,120000,100"), "set_base_pressure", "get_base_pressure");
    }

private:
    double m_baseTemperature = 288.15;  // ISA base temperature (K)
    double m_basePressure = 101325.0;   // ISA base pressure (Pa)
};

}
