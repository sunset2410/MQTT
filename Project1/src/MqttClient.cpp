#include "MqttClient.h"
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
    cout <<__func__ << "\n";
    this->id = id;
    this->clean_session = clean_session;

    if (mosq == nullptr)
    {
        auto ret = mosquitto_lib_init();
        cout << ret <<"\n";
        mosq = mosquitto_new(this->id, this->clean_session, (void *)this);

        if (mosq != nullptr)
        {
            cout << "New instance of mosquitto created " << this->id << endl;
            
            // ret = mosquitto_loop_start(this->mosq);
            // cout << ret <<"\n";
            // cout << "Mosquitto loop start" << endl;
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

    ret = mosquitto_reconnect_delay_set(mosq, 1, 10, false);
    cout << ret << endl;
    return mosq;
}

bool MqttClient::dcepf_mqtt_sp_invoke_connect(const char *host, int port, int keepalive, const char *bind_address)
{
    cout <<__func__ << "\n";
    this->host = host;
    this->port = port;
    this->keepalive = keepalive;
    this->bind_address = bind_address;
    
    if (mosq != nullptr)
    {
        //mosquitto_tls_set(this->mosq, mosquitto.org.crt)
        auto ret = mosquitto_connect_bind_v5(this->mosq, this->host, this->port, this->keepalive, this->bind_address, this->mosq_prop);
        if (ret == MOSQ_ERR_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        } 
    }
    else
    {
        return false;
    }
}

bool MqttClient::dcepf_mqtt_sp_invoke_subscribe(const char *sub, int qos)
{
    cout <<__func__ << "\n";
    int ret = mosquitto_subscribe_v5(this->mosq, nullptr, sub, qos, 0 , nullptr);
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
    cout <<__func__ << "\n";
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
    cout <<__func__ << "\n";
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
    cout <<__func__ << "\n";
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
    cout <<__func__ << "\n";
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
    cout <<__func__ << "\n";
    mosquitto_connect_v5_callback_set(this->mosq, on_connect_v5);
    cout << "mosquitto_connect_v5_callback_set  " << this->id  << endl;
    //on_connect_v5(nullptr, this, 0, 0, nullptr);
}

bool MqttClient::dcepf_mqtt_sp_start_suback_notify(void on_subscribe(struct mosquitto *, void *, int, int, const int *, const mosquitto_property *props))
{
    cout <<__func__ << "\n";
    mosquitto_subscribe_v5_callback_set(this->mosq, on_subscribe);
    cout << "mosquitto_subscribe_v5_callback_set " << this->id << endl;
}

bool MqttClient::dcepf_mqtt_sp_start_message_notify(void on_message(struct mosquitto *, void *, const struct mosquitto_message *, const mosquitto_property *props))
{
    cout <<__func__ << "\n";
    mosquitto_message_v5_callback_set(this->mosq, on_message);
    cout << "mosquitto_message_v5_callback_set " << this->id << endl;
}

bool MqttClient::dcepf_mqtt_sp_start_disconnect_notify(void on_disconnect(struct mosquitto *, void *, int, const mosquitto_property *props))
{
    cout <<__func__ << "\n";
    mosquitto_disconnect_v5_callback_set(this->mosq, on_disconnect);
    cout << "mosquitto_disconnect_v5_callback_set " << this->id << endl;
}
bool MqttClient::dcepf_mqtt_sp_start_puback_notify(void on_publish(struct mosquitto *, void *, int, int, const mosquitto_property *props))
{
    cout <<__func__ << "\n";
    mosquitto_publish_v5_callback_set(this->mosq, on_publish);
    cout << "mosquitto_message_v5_callback_set " << this->id << endl;
}
bool MqttClient::dcepf_mqtt_sp_start_unsuback_notify(void on_unsubscribe(struct mosquitto *, void *, int, const mosquitto_property *props))
{
    cout <<__func__ << "\n";
    mosquitto_unsubscribe_v5_callback_set(this->mosq, on_unsubscribe);
    cout << "mosquitto_unsubscribe_v5_callback_set " << this->id << endl;
}

int MqttClient::get_dcepf_handle_t()
{
    return dcepf_handle_t;
}


// callback
/*
void MqttClient::on_connect_v5(struct mosquitto *, void *obj, int rc, int flag, const mosquitto_property *props)
{
    callbackPointers.dcepf_mqtt_sp_notify_connack_cb_t(this->dcepf_handle_t, obj, rc, flag, props);
}

void MqttClient::on_disconnect_v5(struct mosquitto *mosq, void *obj, int rc, const mosquitto_property *props)
{
   
}

void MqttClient::on_subscribe_v5(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qo, const mosquitto_property *props)
{
   
}

void MqttClient::on_message_v5(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, const mosquitto_property *property)
{
   
}

void MqttClient::on_publish_v5(struct mosquitto *, void *obj, int mid, int reason_code, const mosquitto_property *property)
{
   
}

void MqttClient::on_unsubscribe_v5(struct mosquitto *mosq, void *obj, int mid, const mosquitto_property *property)
{
  
}
*/
//end callback