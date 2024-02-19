/**************************************************************************************************
  Filename:       AlpacaDevicer.hpp
  Revised:        $Date: 2024-01-14$
  Revision:       $Revision: 01 $

  Description:    ASCOM Alpaca Device

  Copyright 2024 peter_n@gmx.de. All rights reserved.
            based on https://github.com/elenhinan/ESPAscomAlpacaServer
**************************************************************************************************/
#include "AlpacaDevice.h"

void AlpacaDevice::Begin()
{
    for (int i = 0; i <= kAlpacaMaxClients; i++)
    {
        _clients[i].client_id = 0;
        _clients[i].client_transaction_id = 0;
    }
    _alpaca_server->RspStatusClear(_rsp_status);
}

// create url and register callback for REST API
void AlpacaDevice::createCallBack(ArRequestHandlerFunction fn, WebRequestMethodComposite type, const char command[], bool devicemethod)
{
    char url[64];
    snprintf(url, sizeof(url), kAlpacaDeviceCommand, _device_type, _device_number, command);
    DBG_ALPACA_DEVICE_PRINTF("# Register handler for \"%s\" to %s\n", url, command);

    // register handler for generated URI
    _alpaca_server->getServerTCP()->on(url, type, fn);

    // add command to supported methods if devicemethod is true
    if (devicemethod)
    {
        int len = strlen(_supported_actions);
        _supported_actions[len - 1] = '\0';
        if (len > 2)
            strcat(_supported_actions, ", \""); // TODO
        else
            strcat(_supported_actions, "\"");
        strcat(_supported_actions, command);
        strcat(_supported_actions, "\"]");
    }
}

void AlpacaDevice::_setSetupPage()
{
    char url[64];
    snprintf(url, sizeof(url), kAlpacaDeviceCommand, _device_type, _device_number, "jsondata");
    // setup json get handler
    this->createCallBack(LHF(_getJsondata), HTTP_GET, "jsondata", false);
    // setup json post handler
    AsyncCallbackJsonWebHandler *jsonhandler = new AsyncCallbackJsonWebHandler(url, [this](AsyncWebServerRequest *request, JsonVariant &json)
                                                                               {    
        DBG_JSON_PRINTF("%04d BEGIN REQ AlpacaDevice::handler(%s)\n", g_dbg_line++, request->url().c_str());
        DBG_REQ
        JsonObject jsonObj = json.as<JsonObject>();
        this->AlpacaReadJson(jsonObj);
        request->send(200, F("application/json"), F("{\"recieved\":\"true\"}")); 
        DBG_JSON_PRINTF("%04d END   REQ AlpacaServer::handler(%s)\n", g_dbg_line++, request->url().c_str());          
        DBG_END });

    _alpaca_server->getServerTCP()->addHandler(jsonhandler);

    // serve static setup page
    DBG_ALPACA_DEVICE_PRINTF("# Register handler for \"%s\" to /www/setup.html\n", _device_url);
    _alpaca_server->getServerTCP()->serveStatic(_device_url, SPIFFS, "/www/setup.html");
}

void AlpacaDevice::RegisterCallbacks()
{
    this->createCallBack(LHF(AlpacaPutAction), HTTP_PUT, "action", false);
    this->createCallBack(LHF(AlpacaPutCommandBlind), HTTP_PUT, "commandblind", false);
    this->createCallBack(LHF(AlpacaPutCommandBool), HTTP_PUT, "commandbool", false);
    this->createCallBack(LHF(AlpacaPutCommandString), HTTP_PUT, "commandstring", false);
    this->createCallBack(LHF(AlpacaGetConnected), HTTP_GET, "connected", false);
    this->createCallBack(LHF(AlpacaPutConnected), HTTP_PUT, "connected", false);
    this->createCallBack(LHF(AlpacaGetDescription), HTTP_GET, "description", false);
    this->createCallBack(LHF(AlpacaGetDriverInfo), HTTP_GET, "driverinfo", false);
    this->createCallBack(LHF(AlpacaGetDriverVersion), HTTP_GET, "driverversion", false);
    this->createCallBack(LHF(AlpacaGetInterfaceVersion), HTTP_GET, "interfaceversion", false);
    this->createCallBack(LHF(AlpacaGetName), HTTP_GET, "name", false);
    this->createCallBack(LHF(AlpacaGetSupportedActions), HTTP_GET, "supportedactions", false);

    _setSetupPage();
}

void AlpacaDevice::SetDeviceNumber(int8_t device_number)
{
    _device_number = device_number;
    snprintf(_device_url, sizeof(_device_url), kAlpacaDeviceCommand, _device_type, _device_number, "setup");
    snprintf(_device_name, sizeof(_device_name), "%s-%i", _device_type, _device_number);
    snprintf(_device_uid, sizeof(_device_uid), "%s-%s%02X", _device_type, _alpaca_server->GetUID(), _device_number);
}

// alpaca commands
void AlpacaDevice::AlpacaPutAction(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
    _alpaca_server->ThrowRspStatusCommandNotImplemented(request, _rsp_status, "putaction");
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};
void AlpacaDevice::AlpacaPutCommandBlind(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_COMMAND_BLIND
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
    _alpaca_server->ThrowRspStatusCommandNotImplemented(request, _rsp_status, "commandblind");
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};
void AlpacaDevice::AlpacaPutCommandBool(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_COMMAND_BOOL
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
    _alpaca_server->ThrowRspStatusCommandNotImplemented(request, _rsp_status, "commandbool");
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};
void AlpacaDevice::AlpacaPutCommandString(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_COMMAND_STRING
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
    _alpaca_server->ThrowRspStatusCommandNotImplemented(request, _rsp_status, "commandstring");
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};
void AlpacaDevice::AlpacaPutConnected(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_CONNECTED
    uint32_t client_idx = 0;
    uint32_t client_id = 0;
    uint32_t client_transaction_id = 0;
    boolean connected = false;
    bool already_connected = false;
    bool to_many_clients_connected = false;
    bool client_not_found = false;
    bool connect_ok = false;
    bool disconnect_ok = false;

    _clients[0].client_id = 0;
    _clients[0].client_transaction_id = 0;
    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        bool get_client_id = _alpaca_server->GetParam(request, "ClientID", client_id, Spelling_t::kStrict);
        bool get_client_transaction_id = _alpaca_server->GetParam(request, "ClientTransactionID", client_transaction_id, Spelling_t::kStrict);
        bool get_connected = _alpaca_server->GetParam(request, "Connected", connected, Spelling_t::kStrict); // check 'Connected' and Connected value

        if (get_client_id == true && get_client_transaction_id == true &&
            client_id > 0 && client_transaction_id > 0 && get_connected == true)
        {
            if (connected) // names and values correct - try to connectd
            {
                for (int i = 1; i <= kAlpacaMaxClients; i++)
                {
                    if (_clients[i].client_id == client_id)
                    {
                        already_connected = true; // already connected
                        break;
                    }
                }

                for (int i = 1; i <= kAlpacaMaxClients; i++) // search empty client slot
                {
                    if (_clients[i].client_id == 0) // connect
                    {
                        client_idx = i;
                        //_isconnected = true;
                        connect_ok = true;
                        break;
                    }
                }

                if (connect_ok == false)
                    to_many_clients_connected = true; // to manny clients connected
            }
            else // names and values correct - try to disconnect
            {
                for (int i = 1; i <= kAlpacaMaxClients; i++) // search client to disconnect
                {
                    if (_clients[i].client_id == client_id) // disconnect
                    {
                        _clients[i].client_id = 0;
                        _clients[i].client_transaction_id = 0;
                        _clients[i].time_ms = 0;
                        disconnect_ok = true;
                        // if (GetNumberOfConnectedClients() == 0)
                        //     _isconnected == false;
                        break;
                    }
                }

                if (disconnect_ok == false) // client not found
                    client_not_found = true;
            }
            _clients[client_idx].client_id = (get_client_id == true) ? (uint32_t)client_id : 0;
            _clients[client_idx].client_transaction_id = (get_client_transaction_id == true) ? (uint32_t)client_transaction_id : 0;
            _clients[client_idx].time_ms = millis();

            if (already_connected == true) // already connected
                _alpaca_server->ThrowRspStatusClientAlreadyConnected(request, _rsp_status, client_id);

            if (to_many_clients_connected == true) // to manny clients connected
                _alpaca_server->ThrowRspStatusToMannyClients(request, _rsp_status, kAlpacaMaxClients);
        }
        else
        {
            _clients[client_idx].client_id = (get_client_id == true) ? (uint32_t)get_client_id : 0;
            _clients[client_idx].client_transaction_id = (get_client_transaction_id == true) ? (uint32_t)client_transaction_id : 0;

            if (get_client_id == false)
                _alpaca_server->ThrowRspStatusClientIDNotFound(request, _rsp_status);

            if (client_id <= 0)
                _alpaca_server->ThrowRspStatusClientIDInvalid(request, _rsp_status, client_id);

            if (get_client_transaction_id == false)
                _alpaca_server->ThrowRspStatusClientTransactionIDNotFound(request, _rsp_status);

            if (client_transaction_id <= 0)
                _alpaca_server->ThrowRspStatusClientTransactionIDInvalid(request, _rsp_status, client_transaction_id);

            if (get_connected == false) // check 'Connected' and Connected value
                _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Connected");
        }
    }
    catch (AlpacaRspStatus_t *rspStatus)
    {
        // empty;
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};
void AlpacaDevice::AlpacaGetConnected(AsyncWebServerRequest *request)
{
    DBG_DEVICE_GET_CONNECTED
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, client_idx > 0);
    DBG_END
};
void AlpacaDevice::AlpacaGetDescription(AsyncWebServerRequest *request)
{
    DBG_DEVICE_GET_DESCRIPTION
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, _device_description, JsonValue_t::kAsJsonStringValue);
    DBG_END
};
void AlpacaDevice::AlpacaGetDriverInfo(AsyncWebServerRequest *request)
{
    DBG_DEVICE_GET_DRIVER_INFO
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, _driver_info, JsonValue_t::kAsJsonStringValue);
    DBG_END
};
void AlpacaDevice::AlpacaGetDriverVersion(AsyncWebServerRequest *request)
{
    DBG_DEVICE_GET_DRIVER_VERSION
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, _driver_version, JsonValue_t::kAsJsonStringValue);
    DBG_END
};
void AlpacaDevice::AlpacaGetInterfaceVersion(AsyncWebServerRequest *request)
{
    DBG_DEVICE_GET_INTERFACE_VERSION
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, _device_interface_version);
    DBG_END
};
void AlpacaDevice::AlpacaGetName(AsyncWebServerRequest *request)
{
    DBG_DEVICE_GET_NAME
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, GetDeviceName(), JsonValue_t::kAsJsonStringValue);
    DBG_END
};
void AlpacaDevice::AlpacaGetSupportedActions(AsyncWebServerRequest *request)
{
    DBG_DEVICE_GET_SUPPORTED_ACTIONS
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, _supported_actions, JsonValue_t::kAsJsonStringValue);
    DBG_END
};

void AlpacaDevice::AlpacaReadJson(JsonObject &root)
{
    DBG_JSON_PRINTFJ(root, "%04d BEGIN AlpacaDevice::AlpacaReadJson(root=<%s>)\n", g_dbg_line++, s);
    const char *name = root["General"]["Name"];
    if (name)
        strlcpy(_device_name, name, sizeof(_device_name));
    const char *desc = root["General"]["Description"];
    if (desc)
        strlcpy(_device_description, desc, sizeof(_device_description));

    DBG_JSON_PRINTF("%04d END   AlpacaDevice::AlpacaReadJson() ... _device_name=%s _device_desc=%s\n", g_dbg_line++, _device_name, _device_description);
}

void AlpacaDevice::AlpacaWriteJson(JsonObject &root)
{
    DBG_JSON_PRINTF("%04d BEGIN AlpacaDevice::AlpacaWriteJson()\n", g_dbg_line++);
    // read-only values marked with #
    // JsonObject obj_general = root.createNestedObject("General");
    JsonObject obj_general = root["General"].to<JsonObject>();
    obj_general["Name"] = GetDeviceName();
    obj_general["Description"] = _device_description;
    obj_general["UID"] = _device_uid;
    DBG_JSON_PRINTFJ(root, "%04d END   AlpacaDevice::AlpacaWriteJson() ... root=<%s>\n", g_dbg_line++, s);
}

void AlpacaDevice::_getJsondata(AsyncWebServerRequest *request)
{
    DBG_JSON_PRINTF("%04d BEGIN AlpacaDevice::_getJsondata()\n", g_dbg_line++);
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    AlpacaWriteJson(root);
    String ser_json = "";
    serializeJson(root, ser_json);
    request->send(200, kAlpacaJsonType, ser_json);
    DBG_JSON_PRINTFJ(root, "%04d END   AlpacaDevice::_getJsondata() ... ser_json=<%s>\n", g_dbg_line++, s);
}

uint32_t AlpacaDevice::getClientIdxByClientID(uint32_t clientID)
{
    for (int i = 1; i <= kAlpacaMaxClients; i++)
    {
        if (clientID == _clients[i].client_id)
        {
            return i;
        }
    }
    return 0;
}

void AlpacaDevice::CheckClientConnectionTimeout()
{
    uint32_t sys_time_ms = millis();
    for (uint32_t u = 1; u < kAlpacaMaxClients; u++)
    {
        if (_clients[u].client_id > 0)
        {
            uint32_t time_since_last_request_ms = sys_time_ms - _clients[u].time_ms;
            if (time_since_last_request_ms > kAlpacaClientConnectionTimeoutMs)
            {
                DBG_ALPACA_DEVICE_PRINTF("# %04d Alpaca Device <%s>: Client <%d> connection timeout after <%dsec> ... disconnected\n", g_dbg_line++, GetDeviceName(), _clients[u].client_id, time_since_last_request_ms / 1000);
                _clients[u].client_id = 0;
                _clients[u].client_transaction_id = 0;
                _clients[u].time_ms = 0;
            }
        }
    }
}

/*
 * Check request clientID, connection and clientTransactionId
 * client_idx = 0-not connected; 1,...,ALPACA_CLIENT_MAX if connected
 * _client[client_idx] is filled with ClientID and ClientTransactionID if possible
 * _rspStatus is filled
 * @return client_idx
 */
int32_t AlpacaDevice::checkClientDataAndConnection(AsyncWebServerRequest *request, uint32_t &client_idx, Spelling_t spelling)
{
    client_idx = 0;
    int32_t client_id = 0;
    int32_t client_transaction_id = 0;
    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        bool get_client_id = _alpaca_server->GetParam(request, "ClientID", client_id, spelling);
        bool get_client_transaction_id = _alpaca_server->GetParam(request, "ClientTransactionID", client_transaction_id, spelling);

        if (get_client_id && client_id > 0)
            client_idx = getClientIdxByClientID(client_id);

        _clients[client_idx].client_id = (client_id >= 0) ? (uint32_t)client_id : 0;
        _clients[client_idx].client_transaction_id = (client_transaction_id >= 0) ? (uint32_t)client_transaction_id : 0;
        _clients[client_idx].time_ms = millis();

        if (get_client_id == false)
            _alpaca_server->ThrowRspStatusClientIDNotFound(request, _rsp_status);

        if (client_id <= 0)
            _alpaca_server->ThrowRspStatusClientIDInvalid(request, _rsp_status, client_id);

        if (get_client_transaction_id == false)
            _alpaca_server->ThrowRspStatusClientTransactionIDNotFound(request, _rsp_status);

        if (client_transaction_id <= 0)
            _alpaca_server->ThrowRspStatusClientTransactionIDInvalid(request, _rsp_status, client_transaction_id);

        if (client_idx == 0)
            _alpaca_server->ThrowRspStatusClientNotConnected(request, _rsp_status, client_id);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }

    return client_idx;
}

// const uint32_t AlpacaDevice::GetNumberOfConnectedClients()
// {
//     uint32_t numberOfConnectedClients = 0;
//     for (int i = 1; i <= k_alpaca_max_clients; i++)
//     {
//         if (_clients[i].client_id != 0)
//             numberOfConnectedClients++;
//     }
//     return numberOfConnectedClients;
// }
