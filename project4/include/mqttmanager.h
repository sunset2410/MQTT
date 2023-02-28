#ifndef LGMQTT_HPP_INCLUDED
#define LGMQTT_HPP_INCLUDED
#include "mqttclient.h"
#include <mosquitto.h>
#include <map>
#include <mutex>

using namespace std;

enum dcepf_result_t
{
    DCEPF_RESULT_SUCCESS,
    DCEPF_RESULT_CANCEL,
    DCEPF_RESULT_FAILED
};

struct CallBackPointers
{
    void (*dcepf_mqtt_sp_notify_connack_cb_t)(int, void *, int, int, const mosquitto_property *);
    void (*dcepf_mqtt_sp_notify_suback_cb_t)(int, void *, int, int, const int *, const mosquitto_property *props);
    void (*dcepf_mqtt_sp_notify_message_cb_t)(int, void *, const struct mosquitto_message *, const mosquitto_property *props);
    void (*dcepf_mqtt_sp_notify_disconnect_cb_t)(int, void *obj, int rc, const mosquitto_property *props);
    void (*dcepf_mqtt_sp_notify_unsuback_cb_t)(int, void *obj, int mid, const mosquitto_property *property);
    void (*dcepf_mqtt_sp_notify_puback_cb_t)(int, void *obj, int mid, int reason_code, const mosquitto_property *property);
    void (*dcepf_mqtt_sp_invoke_client_init_cb_t)(int handle, dcepf_result_t result, mosquitto *instance);
};

void on_connect_v5(struct mosquitto *, void *obj, int rc, int flag, const mosquitto_property *props);
void on_disconnect_v5(struct mosquitto *mosq, void *obj, int rc, const mosquitto_property *props);
void on_subscribe_v5(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qo, const mosquitto_property *);
void on_unsubscribe_v5(struct mosquitto *mosq, void *obj, int mid, const mosquitto_property *property);
void on_message_v5(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, const mosquitto_property *property);
void on_publish_v5(struct mosquitto *, void *obj, int mid, int reason_code, const mosquitto_property *property);

bool dcepf_mqtt_sp_invoke_client_init(int dcepf_handle_t, const char *id, void callback(int handle, dcepf_result_t result, mosquitto *instance));
bool dcepf_mqtt_sp_invoke_subscribe(int dcepf_handle_t, const char *sub, int qos);
bool dcepf_mqtt_sp_invoke_connect(int dcepf_handle_t, const char *host, int port, int keepalive, const char *bind_address);
bool dcepf_mqtt_sp_invoke_disconnect(int dcepf_handle_t);
bool dcepf_mqtt_sp_invoke_publish(int dcepf_handle_t, const char *topic, char *payload, int qos);
bool dcepf_mqtt_sp_invoke_unsubscribe(int dcepf_handle_t, const char *sub);
bool dcepf_mqtt_sp_invoke_client_cleanup(int dcepf_handle_t);

bool dcepf_mqtt_sp_start_connack_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *obj, int rc, int flag, const mosquitto_property *props));
bool dcepf_mqtt_sp_start_suback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, int, const int *, const mosquitto_property *props));
bool dcepf_mqtt_sp_start_message_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, const struct mosquitto_message *, const mosquitto_property *props));
bool dcepf_mqtt_sp_start_disconnect_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, const mosquitto_property *props));
bool dcepf_mqtt_sp_start_puback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, int, const mosquitto_property *props));
bool dcepf_mqtt_sp_start_unsuback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, const mosquitto_property *props));

MqttClient *getMqttClient(int dcepf_handle_t);
map<int, CallBackPointers>::iterator getCallBackPointer(int dcepf_handle_t);

#endif // LGMQTT_HPP_INCLUDED