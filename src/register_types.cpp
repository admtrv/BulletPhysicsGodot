/*
 * register_types.cpp
 */

#include "register_types.h"

#include "BPPhysicsWorld.h"
#include "BPProjectile.h"
#include "forces/BPForce.h"
#include "forces/BPGravity.h"
#include "forces/BPDrag.h"
#include "forces/BPCoriolis.h"
#include "forces/BPLift.h"
#include "forces/BPMagnus.h"
#include "environments/BPEnvironment.h"
#include "environments/BPAtmosphere.h"
#include "environments/BPGeographic.h"
#include "environments/BPHumidity.h"
#include "environments/BPWind.h"

#include "geography/CoordinateMapping.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_bullet_physics(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }

    BulletPhysics::geography::CoordinateMapping::set(BulletPhysics::geography::mappings::Godot());

    GDREGISTER_CLASS(BPPhysicsWorld);
    GDREGISTER_CLASS(BPProjectile);

    GDREGISTER_ABSTRACT_CLASS(BPForce);
    GDREGISTER_CLASS(BPGravity);
    GDREGISTER_CLASS(BPDrag);
    GDREGISTER_CLASS(BPCoriolis);
    GDREGISTER_CLASS(BPLift);
    GDREGISTER_CLASS(BPMagnus);

    GDREGISTER_ABSTRACT_CLASS(BPEnvironment);
    GDREGISTER_CLASS(BPAtmosphere);
    GDREGISTER_CLASS(BPGeographic);
    GDREGISTER_CLASS(BPHumidity);
    GDREGISTER_CLASS(BPWind);
}

void uninitialize_bullet_physics(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
}

extern "C" {

GDExtensionBool GDE_EXPORT bullet_physics_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
{
    GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
    init_obj.register_initializer(initialize_bullet_physics);
    init_obj.register_terminator(uninitialize_bullet_physics);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
    return init_obj.init();
}

}
