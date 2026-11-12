/*
 * configuration.cpp -- Application-specific configuration handling
 *
 * Copyright (C) 2025  Dygma Lab S.L.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AltRepeatDygma.h"
#include "AutoshiftDygma.h"
#include "Battery.h"
#include "Ble_manager.h"
#include "CapsWordDygma.h"
#include "CombosDygma.h"
#include "ConditionalLayersDygma.h"
#include "configuration.h"
#include "Config_manager.h"
#include "DynamicMacrosDygma.h"
#include "EEPROMKeymapDygma.h"
#include "FirmwareVersion.h"
#include "KeyOverridesDygma.h"
#include "KeyRoleManager.h"
#include "LEDCustomDygma.h"
#include "LEDLayers.h"
#include "LEDManager.h"
#include "LEDPaletteRGB.h"
#include "LayerHoldModsDygma.h"
#include "MouseKeysDygma.h"
#include "QukeysDygma.h"
#include "SuperkeysHandler.h"

typedef struct PACK __attribute__((aligned(4)))
{
    FirmwareVersion::device_spec_t device_spec;

    kaleidoscope::plugin::EEPROMKeymap::keymap_config_t keymap;
    kaleidoscope::plugin::MouseKeys_::MouseKeys_config_t MouseKeys;
    kaleidoscope::plugin::KeyRoleManager::keyrole_config_t keyrole;
    kaleidoscope::plugin::Qukeys::Qukeys_config_t Qukeys;
    kaleidoscope::plugin::SuperkeysHandler::superkey_config_t superkey;
    kaleidoscope::plugin::DynamicMacros::macros_config_t macros;

    LEDManager::ledmanager_conf_t ledmanager;
    LEDPaletteRGB::palette_config_t palette;
    LEDLayers::colormap_config_t colormap;

    Battery::battery_conf_t battery;

    BleManager::connections_config_t ble_connections;

    kaleidoscope::plugin::AutoshiftDygma::autoshift_config_t autoshift;
    kaleidoscope::plugin::CapsWordTriggerDygma::capsword_config_t capsword;
    kaleidoscope::plugin::CombosDygma::combos_config_t combos;
    kaleidoscope::plugin::AltRepeatDygma::altrepeat_config_t altrepeat;
    kaleidoscope::plugin::KeyOverridesDygma::keyoverrides_config_t keyoverrides;
    kaleidoscope::plugin::Qukeys::QuickTap_config_t quicktap;
    kaleidoscope::plugin::ConditionalLayersDygma::condlayers_config_t condlayers;
    kaleidoscope::plugin::Qukeys::DualFunction_config_t dualfunction;
    kaleidoscope::plugin::LayerHoldModsDygma::holdmods_config_t holdmods;
    kaleidoscope::plugin::KeyRoleManager::keymap_format_config_t keymap_format;
    kaleidoscope::plugin::LEDCustomDygma::ledcustom_config_t ledcustom;

} config_cache_t;


#define CONFIG_CACHE_MAX_SIZE   8192

static_assert( sizeof( config_cache_t ) <= CONFIG_CACHE_MAX_SIZE,
               "config_cache_t does not fit in the flash storage region" );

static config_cache_t config_cache;

static result_t _cfg_item_request_cb( ConfigManager::cfg_item_type_t item_type, const void ** pp_item )
{
    switch( item_type )
    {
        case ConfigManager::CFG_ITEM_TYPE_DEVICE_SPEC:

            *pp_item = &config_cache.device_spec;

            break;

        case ConfigManager::CFG_ITEM_TYPE_LEDS_LEDMANAGER:

            *pp_item = &config_cache.ledmanager;

            break;

        case ConfigManager::CFG_ITEM_TYPE_LEDS_PALETTE:

            *pp_item = &config_cache.palette;

            break;

        case ConfigManager::CFG_ITEM_TYPE_LEDS_COLORMAP:

            *pp_item = &config_cache.colormap;

            break;

        case ConfigManager::CFG_ITEM_TYPE_BATTERY:

            *pp_item = &config_cache.battery;

            break;

        case ConfigManager::CFG_ITEM_TYPE_BLE_CONNECTIONS:

            *pp_item = &config_cache.ble_connections;

            break;

        default:

            ASSERT_DYGMA( false, "Invalid KBDMEM item requested" );

            return RESULT_ERR;
    }

    return RESULT_OK;
}

static result_t _cfg_item_request_kbdmem_cb( kbdmem_item_type_t item_type, const void ** pp_item )
{
    switch( item_type )
    {
        case KBDMEM_ITEM_TYPE_KEYMAP:

            *pp_item = &config_cache.keymap;

            break;

        case KBDMEM_ITEM_TYPE_MOUSEKEYS:

            *pp_item = &config_cache.MouseKeys;

            break;

        case KBDMEM_ITEM_TYPE_KEYROLE:

            *pp_item = &config_cache.keyrole;

            break;

        case KBDMEM_ITEM_TYPE_QUKEYS:

            *pp_item = &config_cache.Qukeys;

            break;

        case KBDMEM_ITEM_TYPE_SUPERKEY:

            *pp_item = &config_cache.superkey;

            break;

        case KBDMEM_ITEM_TYPE_MACROS:

            *pp_item = &config_cache.macros;

            break;

        case KBDMEM_ITEM_TYPE_AUTOSHIFT:

            *pp_item = &config_cache.autoshift;

            break;

        case KBDMEM_ITEM_TYPE_CAPSWORD:

            *pp_item = &config_cache.capsword;

            break;

        case KBDMEM_ITEM_TYPE_COMBOS:

            *pp_item = &config_cache.combos;

            break;

        case KBDMEM_ITEM_TYPE_ALTREPEAT:

            *pp_item = &config_cache.altrepeat;

            break;

        case KBDMEM_ITEM_TYPE_KEYOVERRIDES:

            *pp_item = &config_cache.keyoverrides;

            break;

        case KBDMEM_ITEM_TYPE_QUICKTAP:

            *pp_item = &config_cache.quicktap;

            break;

        case KBDMEM_ITEM_TYPE_CONDLAYERS:

            *pp_item = &config_cache.condlayers;

            break;

        case KBDMEM_ITEM_TYPE_DUALFUNC:

            *pp_item = &config_cache.dualfunction;

            break;

        case KBDMEM_ITEM_TYPE_HOLDMODS:

            *pp_item = &config_cache.holdmods;

            break;

        case KBDMEM_ITEM_TYPE_KEYMAP_FORMAT:

            *pp_item = &config_cache.keymap_format;

            break;

        case KBDMEM_ITEM_TYPE_LEDCUSTOM:

            *pp_item = &config_cache.ledcustom;

            break;

        default:

            ASSERT_DYGMA( false, "Invalid KBDMEM item requested" );

            return RESULT_ERR;
    }

    return RESULT_OK;
}

result_t configuration_init( void )
{
    result_t result = RESULT_ERR;

    ConfigManager::ConfigManager_config_t config;

    config.p_config_cache = (uint8_t *)&config_cache;
    config.config_cache_size = sizeof( config_cache );

    /* Callbacks */
    config.item_request_cb = _cfg_item_request_cb;
    config.item_request_kbdmem_cb = _cfg_item_request_kbdmem_cb;

    result = ConfigManager.init( &config );
    EXIT_IF_ERR( result, "ConfigManager.init failed" );

_EXIT:
    return result;
}

void configuration_save( void )
{
    ConfigManager.config_save_now();
}

void configuration_run()
{
    ConfigManager.run();
}
