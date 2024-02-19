/**************************************************************************************************
  Filename:       AlpacaDebug.h
  Revised:        $Date: 2024-01-18$
  Revision:       $Revision: 01 $

  Description:    Debugging for ESP32 Alpaca server

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include <Arduino.h>

// For declaration include "#define VAR_DECLS" in (only) one *.cpp (see AlpacaServer.cpp)
#ifndef VAR_DECLS
#define _DECL_ extern
#define _INIT_(x)
#else
#define _DECL_
#define _INIT_(x) = x
#endif

// definition and declaration of global variables; don't touch
_DECL_ bool gDbg _INIT_(false);
_DECL_ uint32_t g_dbg_line _INIT_(0);
_DECL_ Stream *debugstream _INIT_(&Serial);

// don't touch
#define Dbg           \
    if (gDbg == true) \
    debugstream


// comment/uncomment  to debug / for release
#define RELEASE

// don't touch
#ifdef RELEASE 
#define DEBUG_PRINTF(...)
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#else
#define DEBUG_PRINTF(...) \
    if (gDbg)             \
        debugstream->printf(__VA_ARGS__);
#define DEBUG_PRINT(...) \
    if (gDbg)            \
        debugstream->print(__VA_ARGS__);
#define DEBUG_PRINTLN(...) \
    if (gDbg)              \
        debugstream->println(__VA_ARGS__);

#endif

// comment/uncomment to disable/enable AlpacaServer debugging
#define DBG_ALPACA_SERVER_PRINTF(...)
#define DBG_ALPACA_SERVER_PRINTIPLN(...)

// don't touch
#ifndef DBG_ALPACA_SERVER_PRINTF
#define DBG_ALPACA_SERVER_PRINTF(...) debugstream->printf(__VA_ARGS__);
#endif
#ifndef DBG_ALPACA_SERVER_PRINTIPLN
#define DBG_ALPACA_SERVER_PRINTIPLN(...) debugstream->println(__VA_ARGS__);
#endif

// comment/uncomment to disable/enable AlpacaDevice debugging
#define DBG_ALPACA_DEVICE_PRINTF(...)

// dont't touch
#ifndef DBG_ALPACA_DEVICE_PRINTF
#define DBG_ALPACA_DEVICE_PRINTF(...) debugstream->printf(__VA_ARGS__);
#endif

// comment/uncomment to disable/enable Json debugging
#define DBG_JSON_PRINTFJ(...)
#define DBG_JSON_PRINTF(...)

// don't touch
#ifndef DBG_JSON_PRINTF
#define DBG_JSON_PRINTF(...) debugstream->printf(__VA_ARGS__);
#endif
#ifndef DBG_JSON_PRINTFJ
#define DBG_JSON_PRINTFJ(json, ...)       \
    {                                     \
        char s[1024] = {0};               \
        serializeJson(json, s);           \
        debugstream->printf(__VA_ARGS__); \
    };
#endif


// Debug Alpaca Requests
// comment DBG_REQ to disable
#define DBG_SERVER_GET_MNG_API_VERSION       // DBG_REQ;
#define DBG_SERVER_GET_MNG_DESCRIPTION       // DBG_REQ;
#define DBG_SERVER_GET_MNG_CONFIGUREDDEVICES // DBG_REQ;

#define DBG_DEVICE_GET_CONNECTED         // DBG_REQ;
#define DBG_DEVICE_GET_DESCRIPTION       // DBG_REQ;
#define DBG_DEVICE_GET_DRIVER_INFO       // DBG_REQ;
#define DBG_DEVICE_GET_DRIVER_VERSION    // DBG_REQ;
#define DBG_DEVICE_GET_INTERFACE_VERSION // DBG_REQ;
#define DBG_DEVICE_GET_NAME              // DBG_REQ;
#define DBG_DEVICE_GET_SUPPORTED_ACTIONS // DBG_REQ;

#define DBG_DEVICE_PUT_ACTION_REQ     // DBG_REQ;
#define DBG_DEVICE_PUT_COMMAND_BLIND  // DBG_REQ;
#define DBG_DEVICE_PUT_COMMAND_BOOL   // DBG_REQ;
#define DBG_DEVICE_PUT_COMMAND_STRING // DBG_REQ;
#define DBG_DEVICE_PUT_CONNECTED      // DBG_REQ;

#define DBG_CC_GET_BRIGHTNESS       // DBG_REQ;
#define DBG_CC_GET_CALIBRATOR_STATE // DBG_REQ;
#define DBG_CC_GET_COVER_STATE      // DBG_REQ;
#define DBG_CC_GET_MAX_BRIGHTNESS   // DBG_REQ;

#define DBG_CC_PUT_CALIBRATOR_OFF // DBG_REQ;
#define DBG_CC_PUT_CALIBRATOR_ON  // DBG_REQ;
#define DBG_CC_PUT_CLOSE_COVER    // DBG_REQ;
#define DBG_CC_PUT_HALT_COVER     // DBG_REQ;
#define DBG_CC_PUT_OPEN_COVER     // DBG_REQ;

#define DBG_SWITCH_GET_MAX_SWITCH         // DBG_REQ;
#define DBG_SWITCH_CAN_WRITE              // DBG_REQ;
#define DBG_SWITCH_GET_SWITCH             // DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_DESCRIPTION // DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_NAME        // DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_VALUE       // DBG_REQ;
#define DBG_SWITCH_GET_MIN_SWITCH_VALUE   // DBG_REQ;
#define DBG_SWITCH_GET_MAX_SWITCH_VALUE   // DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_STEP        // DBG_REQ;

#define DBG_SWITCH_PUT_SET_SWITCH       // DBG_REQ;
#define DBG_SWITCH_PUT_SET_SWITCH_NAME  // DBG_REQ;
#define DBG_SWITCH_PUT_SET_SWITCH_VALUE // DBG_REQ;

#define DBG_OBSERVING_CONDITIONS_GET_AVERAGE_PERIOD         // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_CLOUD_COVER            // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_DEW_POINT              // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_HUMIDITY               // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_PRESSURE               // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_RAIN_RATE              // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SKY_BRIGHTNESS         // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SKY_QUALITY            // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SKY_TEMPERATURE        // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_STAR_FWHM              // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_TEMPERATURE            // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_WIND_DIRECTION         // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_WIND_GUES              // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_WIND_SPEED             // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SENSOR_DESCRIPTION     // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_TIME_SINCE_LAST_UPDATE // DBG_REQ;

#define DBG_OBSERVING_CONDITIONS_GET_PUT_AVERAGE_PERIOD // DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_PUT_REFRESH            // DBG_REQ;

// don't touch
#ifdef RELEASE
#define DBG_REQ
#define DBG_RESPOND_VALUE
#define DBG_END
#else
#define DBG_REQ                                       \
    gDbg = true;                                      \
    Dbg->printf("\n%04d Alpaca REQ (", g_dbg_line++); \
    Dbg->print(request->client()->remoteIP());        \
    Dbg->printf(") %s ", request->methodToString());  \
    Dbg->print(request->url());                       \
    for (int i = 0; i < request->args(); i++)         \
    {                                                 \
        Dbg->print(" - ");                            \
        Dbg->print(request->argName(i));              \
        Dbg->print("='");                             \
        Dbg->print(request->arg(i));                  \
        Dbg->print("'");                              \
    }                                                 \
    Dbg->println("");
#define DBG_RESPOND_VALUE \
    Dbg->printf("# %d Alpaca RSP %s\n", (int32_t)rsp_status.http_status, response);
#define DBG_END gDbg = false;

#endif