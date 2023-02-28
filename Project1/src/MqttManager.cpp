#include "MqttManager.h"

map <int, MqttClient*> mapClients; 

MqttClient* getMqttClient(int dcepf_handle_t)
{
    //cout <<__func__ << "\n";
    auto it = mapClients.find(dcepf_handle_t);
    if (it == mapClients.end())
    {
        cout << "Element not found";
        return nullptr;
    }
    else
    {
        cout << it->second << "\n";
        return it->second;       
    }
}


dcepf_result_t dcepf_mqtt_sp_invoke_client_init(int dcepf_handle_t, const char *id, void callback(int handle, dcepf_result_t result, mosquitto *instance))
{
    cout <<__func__ << "\n";
    MqttClient *mqttClient = new MqttClient(dcepf_handle_t);
    cout << "mqttClient: " << mqttClient << endl;
    mapClients.insert(pair<int, MqttClient*>(dcepf_handle_t, mqttClient));
    mosquitto *instance = mqttClient->dcepf_mqtt_sp_invoke_client_init(id, false);
    if (instance != nullptr)
        callback(dcepf_handle_t, DCEPF_RESULT_SUCCESS, instance);
    else
        callback(dcepf_handle_t, DCEPF_RESULT_FAILED, instance);

    return DCEPF_RESULT_SUCCESS;
}


dcepf_result_t dcepf_mqtt_sp_invoke_connect(int dcepf_handle_t, const char *host, int port, int keepalive, const char *bind_address)
{
    cout <<__func__ << "\n";
    bool ret = getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_connect(host, port, keepalive, bind_address);
    if(ret) 
        return DCEPF_RESULT_SUCCESS;
    else 
        return DCEPF_RESULT_FAILED;
}



dcepf_result_t dcepf_mqtt_sp_invoke_subscribe(int dcepf_handle_t, const char *sub, int qos)
{
    cout <<__func__ << "\n";
    bool ret = getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_subscribe(sub, qos);
    if(ret) 
        return DCEPF_RESULT_SUCCESS;
    else 
        return DCEPF_RESULT_FAILED;
}


dcepf_result_t dcepf_mqtt_sp_start_connack_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *obj, int rc, int flag, const mosquitto_property *props))
{
    cout <<__func__ << "\n";
    getMqttClient(dcepf_handle_t)->callbackPointers.dcepf_mqtt_sp_notify_connack_cb_t = callback;
    bool ret = getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_start_connack_notify(on_connect_v5);
    if(ret) 
        return DCEPF_RESULT_SUCCESS;
    else 
        return DCEPF_RESULT_FAILED;
}

dcepf_result_t dcepf_mqtt_sp_start_suback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, int, const int *, const mosquitto_property *props))
{
    cout <<__func__ << "\n";
    getMqttClient(dcepf_handle_t)->callbackPointers.dcepf_mqtt_sp_notify_suback_cb_t = callback;
    bool ret =  getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_start_suback_notify(on_subscribe_v5);
    if(ret) 
        return DCEPF_RESULT_SUCCESS;
    else 
        return DCEPF_RESULT_FAILED;
}


dcepf_result_t dcepf_mqtt_sp_invoke_publish(int dcepf_handle_t, const char *topic, char *payload, int qos)
{
    cout <<__func__ << "\n";
    bool ret =  getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_publish(topic, payload, qos);
    if(ret) 
        return DCEPF_RESULT_SUCCESS;
    else 
        return DCEPF_RESULT_FAILED;
}



////////////////////////////////////////////////

void on_connect_v5(struct mosquitto *, void *obj, int rc, int flag, const mosquitto_property *props)
{
    cout << __func__ << "\n";
    MqttClient *client = reinterpret_cast<MqttClient *>(obj);
    int dcepf_handle_t = client->get_dcepf_handle_t();
    client->callbackPointers.dcepf_mqtt_sp_notify_connack_cb_t(dcepf_handle_t, obj, rc, flag, props);
}


void on_subscribe_v5(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qo, const mosquitto_property *props)
{
    cout << __func__ << "\n";
    MqttClient *client = reinterpret_cast<MqttClient *>(obj);
    int dcepf_handle_t = client->get_dcepf_handle_t();
    client->callbackPointers.dcepf_mqtt_sp_notify_suback_cb_t(dcepf_handle_t, obj, mid, qos_count, granted_qo, props);
}

// void on_disconnect_v5(struct mosquitto *mosq, void *obj, int rc, const mosquitto_property *props)
// {
//     std::lock_guard<std::mutex> lock(mMosqCallbackMutex);
//     MqttClient *client = reinterpret_cast<MqttClient *>(obj);
//     int dcepf_handle_t = client->get_dcepf_handle_t();
//     auto it = getCallBackPointer(dcepf_handle_t);
//     it->second.dcepf_mqtt_sp_notify_disconnect_cb_t(dcepf_handle_t, obj, rc, props);
// }



// void on_message_v5(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, const mosquitto_property *property)
// {
//     std::lock_guard<std::mutex> lock(mMosqCallbackMutex);
//     MqttClient *client = reinterpret_cast<MqttClient *>(obj);
//     int dcepf_handle_t = client->get_dcepf_handle_t();
//     auto it = getCallBackPointer(dcepf_handle_t);
//     it->second.dcepf_mqtt_sp_notify_message_cb_t(dcepf_handle_t, obj, message, property);
// }

// void on_publish_v5(struct mosquitto *, void *obj, int mid, int reason_code, const mosquitto_property *property)
// {
//     std::lock_guard<std::mutex> lock(mMosqCallbackMutex);
//     MqttClient *client = reinterpret_cast<MqttClient *>(obj);
//     int dcepf_handle_t = client->get_dcepf_handle_t();
//     auto it = getCallBackPointer(dcepf_handle_t);
//     it->second.dcepf_mqtt_sp_notify_puback_cb_t(dcepf_handle_t, obj, mid, reason_code, property);
// }

// void on_unsubscribe_v5(struct mosquitto *mosq, void *obj, int mid, const mosquitto_property *property)
// {
//     std::lock_guard<std::mutex> lock(mMosqCallbackMutex);
//     MqttClient *client = reinterpret_cast<MqttClient *>(obj);
//     int dcepf_handle_t = client->get_dcepf_handle_t();
//     auto it = getCallBackPointer(dcepf_handle_t);
//     it->second.dcepf_mqtt_sp_notify_unsuback_cb_t(dcepf_handle_t, obj, mid, property);
// }