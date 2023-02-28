#include "mqttmgr.h"
#include<iostream>
using namespace std;
#include<mutex>

mutex m;
map<int, MqttClient *> mapClients;


MqttClient *getMqttClient(int dcepf_handle_t)
{
    lock_guard<mutex> guard(m);
    auto it = mapClients.find(dcepf_handle_t);
    if (it == mapClients.end())
    {
        cout << "Element not found";
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

// callback
void on_connect_v5(struct mosquitto *, void *obj, int rc, int flag, const mosquitto_property *props)
{
    cout << __func__ << "\n";
    MqttClient *client = reinterpret_cast<MqttClient*>(obj);
    client->callBack.dcepf_mqtt_sp_notify_connack_cb_t(client->get_dcepf_handle_t(), obj, rc, flag, props);
}

void on_disconnect_v5(struct mosquitto *mosq, void *obj, int rc, const mosquitto_property *props)
{
    cout << __func__ << "\n";
    MqttClient *client = reinterpret_cast<MqttClient *>(obj);
    int dcepf_handle_t = client->get_dcepf_handle_t();
    client->callBack.dcepf_mqtt_sp_notify_disconnect_cb_t(dcepf_handle_t, obj, rc, props);
}

void on_subscribe_v5(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qo, const mosquitto_property *props)
{
    MqttClient *client = reinterpret_cast<MqttClient *>(obj);
    int dcepf_handle_t = client->get_dcepf_handle_t();
    client->callBack.dcepf_mqtt_sp_notify_suback_cb_t(dcepf_handle_t, obj, mid, qos_count, granted_qo, props);
}

void on_message_v5(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, const mosquitto_property *property)
{
    MqttClient *client = reinterpret_cast<MqttClient *>(obj);
    int dcepf_handle_t = client->get_dcepf_handle_t();
    client->callBack.dcepf_mqtt_sp_notify_message_cb_t(dcepf_handle_t, obj, message, property);
}

void on_publish_v5(struct mosquitto *, void *obj, int mid, int reason_code, const mosquitto_property *property)
{
    MqttClient *client = reinterpret_cast<MqttClient *>(obj);
    int dcepf_handle_t = client->get_dcepf_handle_t();
    client->callBack.dcepf_mqtt_sp_notify_puback_cb_t(dcepf_handle_t, obj, mid, reason_code, property);
}

void on_unsubscribe_v5(struct mosquitto *mosq, void *obj, int mid, const mosquitto_property *property)
{
    MqttClient *client = reinterpret_cast<MqttClient *>(obj);
    int dcepf_handle_t = client->get_dcepf_handle_t();
    client->callBack.dcepf_mqtt_sp_notify_unsuback_cb_t(dcepf_handle_t, obj, mid, property);
}

// end callback




// invoke function 
bool dcepf_mqtt_sp_invoke_client_init(int dcepf_handle_t, const char *id, void callback(int handle, dcepf_result_t result, mosquitto *instance))
{
    lock_guard<mutex> guard(m);
    MqttClient *mqttClient = new MqttClient(dcepf_handle_t);
    mqttClient->callBack.dcepf_mqtt_sp_invoke_client_init_cb_t = callback;
    mapClients.insert(pair<int, MqttClient *>(dcepf_handle_t, mqttClient));
    mosquitto *instance = mqttClient->dcepf_mqtt_sp_invoke_client_init(id, false);

    if (instance == nullptr) {
        mqttClient->callBack.dcepf_mqtt_sp_invoke_client_init_cb_t(dcepf_handle_t, DCEPF_RESULT_FAILED, instance);
    }
    else {
        mqttClient->callBack.dcepf_mqtt_sp_invoke_client_init_cb_t(dcepf_handle_t, DCEPF_RESULT_SUCCESS, instance);
    }
}

bool dcepf_mqtt_sp_invoke_connect(int dcepf_handle_t, const char *host, int port, int keepalive, const char *bind_address)
{
    return getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_connect(host, port, keepalive, bind_address);
}

bool dcepf_mqtt_sp_invoke_publish(int dcepf_handle_t, const char *topic, char *payload, int qos)
{
    return getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_publish(topic, payload, qos);
}

bool dcepf_mqtt_sp_invoke_disconnect(int dcepf_handle_t)
{
    return getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_disconnect();
}

bool dcepf_mqtt_sp_invoke_subscribe(int dcepf_handle_t, const char *sub, int qos)
{
    return getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_subscribe(sub, qos);
}

bool dcepf_mqtt_sp_invoke_client_cleanup(int dcepf_handle_t)
{
    return getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_client_cleanup();
}

bool dcepf_mqtt_sp_invoke_unsubscribe(int dcepf_handle_t, const char *sub)
{
    return getMqttClient(dcepf_handle_t)->dcepf_mqtt_sp_invoke_unsubscribe(sub);
}



// set callback function
bool dcepf_mqtt_sp_start_connack_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *obj, int rc, int flag, const mosquitto_property *props))
{
    MqttClient *client  = getMqttClient(dcepf_handle_t);
    client->callBack.dcepf_mqtt_sp_notify_connack_cb_t = callback;
    return client->dcepf_mqtt_sp_start_connack_notify(on_connect_v5);
}

bool dcepf_mqtt_sp_start_message_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, const struct mosquitto_message *, const mosquitto_property *props))
{
    MqttClient *client  = getMqttClient(dcepf_handle_t);
    client->callBack.dcepf_mqtt_sp_notify_message_cb_t = callback;
    return client->dcepf_mqtt_sp_start_message_notify(on_message_v5);
}

bool dcepf_mqtt_sp_start_disconnect_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, const mosquitto_property *props))
{
    MqttClient *client  = getMqttClient(dcepf_handle_t);
    client->callBack.dcepf_mqtt_sp_notify_disconnect_cb_t = callback;
    return client->dcepf_mqtt_sp_start_disconnect_notify(on_disconnect_v5);
}

bool dcepf_mqtt_sp_start_suback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, int, const int *, const mosquitto_property *props))
{
    MqttClient *client  = getMqttClient(dcepf_handle_t);
    client->callBack.dcepf_mqtt_sp_notify_suback_cb_t = callback;
    return client->dcepf_mqtt_sp_start_suback_notify(on_subscribe_v5);
}

bool dcepf_mqtt_sp_start_puback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, int, const mosquitto_property *props))
{
    MqttClient *client  = getMqttClient(dcepf_handle_t);
    client->callBack.dcepf_mqtt_sp_notify_puback_cb_t = callback;
    return client->dcepf_mqtt_sp_start_puback_notify(on_publish_v5);
}

bool dcepf_mqtt_sp_start_unsuback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, const mosquitto_property *props))
{
    MqttClient *client  = getMqttClient(dcepf_handle_t);
    client->callBack.dcepf_mqtt_sp_notify_unsuback_cb_t = callback;
    return client->dcepf_mqtt_sp_start_disconnect_notify(on_unsubscribe_v5);
}








