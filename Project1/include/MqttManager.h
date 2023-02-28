#ifndef MQTTMGR_HPP_INCLUDED
#define MQTTMGR_HPP_INCLUDED
#include "MqttClient.h"
#include <mosquitto.h>
#include <map>
#include<iostream>
using namespace std;

    void on_connect_v5(struct mosquitto *, void *obj, int rc, int flag, const mosquitto_property *props);
    void on_subscribe_v5(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qo, const mosquitto_property *props);
    // void on_disconnect_v5(struct mosquitto *mosq, void *obj, int rc, const mosquitto_property *props);
    // void on_unsubscribe_v5(struct mosquitto *mosq, void *obj, int mid, const mosquitto_property *property);
    // void on_message_v5(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, const mosquitto_property *property);
    // void on_publish_v5(struct mosquitto *, void *obj, int mid, int reason_code, const mosquitto_property *property);

    MqttClient *getMqttClient(int dcepf_handle_t);
    dcepf_result_t dcepf_mqtt_sp_invoke_client_init(int dcepf_handle_t, const char *id, void callback(int handle, dcepf_result_t result, mosquitto *instance));
    dcepf_result_t dcepf_mqtt_sp_invoke_connect(int dcepf_handle_t, const char *host, int port, int keepalive, const char *bind_address);
    dcepf_result_t dcepf_mqtt_sp_invoke_subscribe(int dcepf_handle_t, const char *sub, int qos);
    //dcepf_result_t dcepf_mqtt_sp_invoke_disconnect(int dcepf_handle_t);
    dcepf_result_t dcepf_mqtt_sp_invoke_publish(int dcepf_handle_t, const char *topic, char *payload, int qos);
    //dcepf_result_t dcepf_mqtt_sp_invoke_unsubscribe(int dcepf_handle_t, const char *sub);
    //dcepf_result_t dcepf_mqtt_sp_invoke_client_cleanup(int dcepf_handle_t);

    dcepf_result_t dcepf_mqtt_sp_start_connack_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *obj, int rc, int flag, const mosquitto_property *props));
    dcepf_result_t dcepf_mqtt_sp_start_suback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, int, const int *, const mosquitto_property *props));
    //dcepf_result_t dcepf_mqtt_sp_start_message_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, const struct mosquitto_message *, const mosquitto_property *props));
    //dcepf_result_t dcepf_mqtt_sp_start_disconnect_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, const mosquitto_property *props));
    //dcepf_result_t dcepf_mqtt_sp_start_puback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, int, const mosquitto_property *props));
    //dcepf_result_t dcepf_mqtt_sp_start_unsuback_notify(int dcepf_handle_t, void callback(int dcepf_handle_t, void *, int, const mosquitto_property *props));


#endif 
