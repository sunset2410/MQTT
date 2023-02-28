#ifndef MQTTCLIENT_HPP_INCLUDED
#define MQTTCLIENT_HPP_INCLUDED
#include <mosquitto.h>

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

class MqttClient
{
private:
    mosquitto *mosq = nullptr;
    mosquitto_property *mosq_prop = nullptr;

    int dcepf_handle_t = -1;
    const char *id = "";
    bool clean_session = false;
    const char *host = "";
    int port = -1;
    int keepalive = -1 ;
    const char *bind_address = "";
public:
    CallBackPointers callbackPointers;
    MqttClient(int dcepf_handle_t);
    ~MqttClient();
    mosquitto* dcepf_mqtt_sp_invoke_client_init(const char *id, bool clean_session); // id : vin vehicle identity number
    bool dcepf_mqtt_sp_invoke_connect(const char *host, int port, int keepalive, const char *bind_address);
    bool dcepf_mqtt_sp_invoke_disconnect();
    bool dcepf_mqtt_sp_invoke_subscribe(const char *sub, int qos);
    bool dcepf_mqtt_sp_invoke_publish(const char *topic, char *payload, int qos);
    bool dcepf_mqtt_sp_invoke_unsubscribe(const char *sub);
    bool dcepf_mqtt_sp_invoke_client_cleanup();

    bool dcepf_mqtt_sp_start_connack_notify(void on_connect_v5(mosquitto *, void *, int, int, const mosquitto_property *));
    bool dcepf_mqtt_sp_start_suback_notify(void on_subscribe(struct mosquitto *, void *, int, int, const int *, const mosquitto_property *props));
    bool dcepf_mqtt_sp_start_message_notify(void on_message(struct mosquitto *, void *, const struct mosquitto_message *, const mosquitto_property *props));
    bool dcepf_mqtt_sp_start_disconnect_notify(void on_disconnect(struct mosquitto *, void *, int, const mosquitto_property *props));
    bool dcepf_mqtt_sp_start_puback_notify(void on_publish(struct mosquitto *, void *, int, int, const mosquitto_property *props));
    bool dcepf_mqtt_sp_start_unsuback_notify(void on_unsubscribe(struct mosquitto *, void *, int, const mosquitto_property *props));

    int get_dcepf_handle_t();
};

#endif // MQTTCLIENT_HPP_INCLUDED