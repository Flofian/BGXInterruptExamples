#include "../plugin_sdk/plugin_sdk.hpp"
#include "NamiExample.h"
#include "VayneExample.h"
#include "DebugUtil.h"

/*
PLUGIN_NAME("Interrupt Example");
PLUGIN_TYPE(plugin_type::champion);
SUPPORTED_CHAMPIONS( champion_id::Nami, champion_id::Vayne);

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
{
    DECLARE_GLOBALS(plugin_sdk_good);

    switch (myhero->get_champion()) {
    case champion_id::Nami:
        nami::load();
        break;
    case champion_id::Vayne:
        vayne::load();
        break;
    default:
        console->print("Champion %s is not supported!", myhero->get_model_cstr());
        return false;
    }

    return true;
}

PLUGIN_API void on_sdk_unload()
{
    switch (myhero->get_champion())
    {
    case champion_id::Nami:
        nami::unload();
        break;
    case champion_id::Vayne:
        vayne::unload();
        break;
    default:
        break;
    }
}

*/
PLUGIN_NAME("Interrupt Util");
PLUGIN_TYPE(plugin_type::misc);

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
{
    DECLARE_GLOBALS(plugin_sdk_good);

    util::load();

    return true;
}

PLUGIN_API void on_sdk_unload()
{
    util::unload();
}
