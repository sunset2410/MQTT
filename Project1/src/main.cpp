#include <iostream>
#include <mosquitto.h>
#include "MqttManager.h"
using namespace std;

//----------------callback-----------------------------------//
void dcepf_mqtt_sp_invoke_client_init_cb_t(int handle, dcepf_result_t result, mosquitto *instance)
{
    cout <<  " dcepf_mqtt_sp_invoke_client_init_cb_t" << endl;
    cout << "handle: " << handle << endl;
    cout << "dcepf_result_t: " << result << endl;
    if (instance == nullptr)
    {
        cout << "instance: "
             << "nullptr" << endl;
    }
    else
    {
        cout << "instance: " << instance << endl;
    }
}

void dcepf_mqtt_sp_notify_connack_cb_t(int dcepf_handle_t, void *obj, int rc, int flag, const mosquitto_property *props)
{
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "rc: " << rc << endl;
    cout << "flag: " << flag << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_disconnect_cb_t(int dcepf_handle_t, void *obj, int rc, const mosquitto_property *props)
{
    cout << "dcepf_mqtt_sp_notify_disconnect_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "rc: " << rc << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_suback_cb_t(int dcepf_handle_t, void *obj, int mid, int qos_count, const int *granted_qos, const mosquitto_property *property)
{
    cout << "dcepf_mqtt_sp_notify_suback_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "qos_count: " << qos_count << endl;
    cout << "granted_qo: " << *granted_qos << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_unsuback_cb_t(int dcepf_handle_t, void *obj, int mid, const mosquitto_property *property)
{
    cout << __func__ << "dcepf_mqtt_sp_notify_puback_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "mid: " << mid << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_message_cb_t(int dcepf_handle_t, void *obj, const struct mosquitto_message *message, const mosquitto_property *property)
{
    cout << "dcepf_mqtt_sp_notify_message_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "mid: " << message->mid << endl;
    cout << "topic: " << message->topic << endl;
    cout << "payload: " << reinterpret_cast<char *>(message->payload) << endl;
    cout << "payloadlen: " << message->payloadlen << endl;
    cout << "qos: " << message->qos << endl;
    cout << "retain: " << message->retain << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_puback_cb_t(int dcepf_handle_t, void *obj, int mid, int reason_code, const mosquitto_property *property)
{
    cout << "dcepf_mqtt_sp_notify_puback_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "mid: " << mid << endl;
    cout << "reason_code: " << reason_code << endl;
    cout << "********************************************" << endl;
}


/////////////////////////////////////////////////////////////////

void init() {
	dcepf_mqtt_sp_invoke_client_init(1999, "a11999", dcepf_mqtt_sp_invoke_client_init_cb_t);
    // set callback
    dcepf_mqtt_sp_start_connack_notify(1999, dcepf_mqtt_sp_notify_connack_cb_t);
    dcepf_mqtt_sp_start_suback_notify(1999, dcepf_mqtt_sp_notify_suback_cb_t);
    //dcepf_mqtt_sp_start_message_notify(11, dcepf_mqtt_sp_notify_message_cb_t);
    //dcepf_mqtt_sp_start_disconnect_notify(11, dcepf_mqtt_sp_notify_disconnect_cb_t);
    //dcepf_mqtt_sp_start_puback_notify(11, dcepf_mqtt_sp_notify_puback_cb_t);
    //dcepf_mqtt_sp_start_unsuback_notify(11, dcepf_mqtt_sp_notify_unsuback_cb_t);
}

void connect() {
    //dcepf_mqtt_sp_invoke_connect(1999, "broker.hivemq.com", 1883, 60, nullptr);
    dcepf_mqtt_sp_invoke_connect(1999, "broker.hivemq.com", 1883, 60, nullptr);
}

void sub() {
    dcepf_mqtt_sp_invoke_subscribe(1999, "test/t1", 0);
}

void pub() { 
    dcepf_mqtt_sp_invoke_publish(1999, "test/t2", "hihi", 0);
}


void unsub() {


}

void disconnect() {


}



int main() {
	init();
    connect();
    //sub();
    pub();
	return 0;
}
















/*
int main1()
{

	int rc;
	struct mosquitto* mosq;
	mosquitto_lib_init();
	mosq = mosquitto_new("publisher-test", true, NULL);
	mosquitto_loop_start(mosq);
	//rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	rc = mosquitto_connect(mosq, "broker.emqx.io", 1883, 60);
	if(rc != 0){

	printf("client can not connect to broker");
	mosquitto_destroy(mosq);
	return -1;
	}
	printf("connect success to broker");
	mosquitto_publish(mosq, NULL, "test/t1", 6, "hello", 0, false);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
*/