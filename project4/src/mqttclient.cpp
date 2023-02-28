#include "mqttclient.h"
#include <bits/stdc++.h>

using namespace std;

MqttClient::MqttClient(int dcepf_handle_t)
{
    this->dcepf_handle_t = dcepf_handle_t;
}

MqttClient::~MqttClient()
{
    
}

mosquitto *MqttClient::dcepf_mqtt_sp_invoke_client_init(const char *id, bool clean_session)
{
    this->id = id;
    this->clean_session = clean_session;

    if (mosq == nullptr)
    {
        auto ret = mosquitto_lib_init();

        mosq = mosquitto_new(this->id, this->clean_session, (void *)this);

        if (mosq != nullptr)
        {
            cout << "New instance of mosquitto created " << this->id << endl;

            ret = mosquitto_loop_start(this->mosq);
            cout << "Mosquitto loop start" << endl;
        }
        else
        {
            cout << "Cannot create mosquitto instance " << this->id << endl;

            return nullptr;
        }
    }
    else
    {
        auto ret = mosquitto_reinitialise(mosq, this->id, this->clean_session, (void *)this);
        if (ret != MOSQ_ERR_SUCCESS)
        {
            return nullptr;
        }
    }

    auto ret = mosquitto_int_option(mosq, MOSQ_OPT_PROTOCOL_VERSION, MQTT_PROTOCOL_V5);
    cout << this->id << " " << ret << endl;

    mosquitto_reconnect_delay_set(mosq, 1, 10, false);
    return mosq;
}

bool MqttClient::dcepf_mqtt_sp_invoke_connect(const char *host, int port, int keepalive, const char *bind_address)
{
    this->host = host;
    this->port = port;
    this->keepalive = keepalive;
    this->bind_address = bind_address;

    if (mosq != nullptr)
    {
        auto ret = mosquitto_connect_bind_v5(this->mosq, this->host, this->port, this->keepalive, this->bind_address, this->mosq_prop);

        // if (ret == MOSQ_ERR_SUCCESS)
        // {
        //     ret = mosquitto_loop_start(this->mosq);
        //     cout << "Mosquitto loop start" << endl;
        //     if (ret == MOSQ_ERR_SUCCESS)
        //     {
        //         return true;
        //     }
        //     else
        //     {
        //         return false;
        //     }
        // }
    }
    else
    {
        return false;
    }
}

bool MqttClient::dcepf_mqtt_sp_invoke_subscribe(const char *sub, int qos)
{
    int ret = mosquitto_subscribe_v5(this->mosq, nullptr, sub, qos, NULL, nullptr);
    if (ret == MOSQ_ERR_SUCCESS)
    {
        cout << "mosquitto_subscribe_v5 success topic: " << sub << " QOS: " << qos << endl;
        return true;
    }
    else
    {
        cout << "mosquitto_subscribe_v5 failure topic: " << sub << " QOS: " << qos << endl;
        return false;
    }
}

bool MqttClient::dcepf_mqtt_sp_invoke_disconnect()
{
    int ret = mosquitto_disconnect_v5(this->mosq, 0, nullptr);
    if (ret == MOSQ_ERR_SUCCESS)
    {
        cout << "mosquitto_disconnect_v5 success " << endl;
        return true;
    }
    else
    {
        cout << "mosquitto_disconnect_v5 failure" << endl;
        return false;
    }
}

bool MqttClient::dcepf_mqtt_sp_invoke_publish(const char *topic, char *payload, int qos)
{
    int ret = mosquitto_publish_v5(this->mosq, NULL, topic, (unsigned)strlen(payload), static_cast<void *>(payload), qos, false, NULL);
    if (ret == MOSQ_ERR_SUCCESS)
    {
        cout << "mosquitto_publish_v5 success " << endl;
        return true;
    }
    else
    {
        cout << "mosquitto_publish_v5 failure" << endl;
        return false;
    }
}

bool MqttClient::dcepf_mqtt_sp_invoke_client_cleanup()
{
    auto ret = mosquitto_loop_stop(this->mosq, true);
    mosquitto_destroy(this->mosq);
    mosquitto_lib_cleanup();

    if (ret == MOSQ_ERR_SUCCESS)
    {
        cout << "mosquitto_loop_stop success" << endl;
        return true;
    }
    cout << "mosquitto_loop_stop failure" << endl;
    return false;
}

bool MqttClient::dcepf_mqtt_sp_invoke_unsubscribe(const char *sub)
{
    int ret = mosquitto_unsubscribe_v5(this->mosq, NULL, sub, NULL);
    if (ret != MOSQ_ERR_SUCCESS)
    {
        cout << "Mosquitto can not send message to backend" << endl;
        return false;
    }
    else
    {
        cout << "mosquitto_unsubscribe_v5 success" << endl;
        return true;
    }
}

bool MqttClient::dcepf_mqtt_sp_start_connack_notify(void on_connect_v5(mosquitto *, void *, int, int, const mosquitto_property *))
{
    mosquitto_connect_v5_callback_set(this->mosq, on_connect_v5);
    cout << "mosquitto_connect_v5_callback_set " << this->id << endl;
}
bool MqttClient::dcepf_mqtt_sp_start_suback_notify(void on_subscribe(struct mosquitto *, void *, int, int, const int *, const mosquitto_property *props))
{
    mosquitto_subscribe_v5_callback_set(this->mosq, on_subscribe);
    cout << "mosquitto_subscribe_v5_callback_set " << this->id << endl;
}
bool MqttClient::dcepf_mqtt_sp_start_message_notify(void on_message(struct mosquitto *, void *, const struct mosquitto_message *, const mosquitto_property *props))
{
    mosquitto_message_v5_callback_set(this->mosq, on_message);
    cout << "mosquitto_message_v5_callback_set " << this->id << endl;
}

bool MqttClient::dcepf_mqtt_sp_start_disconnect_notify(void on_disconnect(struct mosquitto *, void *, int, const mosquitto_property *props))
{
    mosquitto_disconnect_v5_callback_set(this->mosq, on_disconnect);
    cout << "mosquitto_disconnect_v5_callback_set " << this->id << endl;
}
bool MqttClient::dcepf_mqtt_sp_start_puback_notify(void on_publish(struct mosquitto *, void *, int, int, const mosquitto_property *props))
{
    mosquitto_publish_v5_callback_set(this->mosq, on_publish);
    cout << "mosquitto_message_v5_callback_set " << this->id << endl;
}
bool MqttClient::dcepf_mqtt_sp_start_unsuback_notify(void on_unsubscribe(struct mosquitto *, void *, int, const mosquitto_property *props))
{
    mosquitto_unsubscribe_v5_callback_set(this->mosq, on_unsubscribe);
    cout << "mosquitto_unsubscribe_v5_callback_set " << this->id << endl;
}

int MqttClient::get_dcepf_handle_t()
{
    return dcepf_handle_t;
}