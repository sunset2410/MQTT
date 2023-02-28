#include <iostream>
#include "mqttmgr.h"
#include <chrono>
#include <thread>
using namespace std;

void dcepf_mqtt_sp_invoke_client_init_cb_t(int handle, dcepf_result_t result, mosquitto *instance)
{
    cout << "dcepf_mqtt_sp_invoke_client_init_cb_t" << endl;
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
    cout << "dcepf_mqtt_sp_notify_puback_cb_t" << endl;
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

void init() {
    dcepf_mqtt_sp_invoke_client_init(1,"VINDI1", dcepf_mqtt_sp_invoke_client_init_cb_t);
    dcepf_mqtt_sp_start_connack_notify(1, dcepf_mqtt_sp_notify_connack_cb_t);
    dcepf_mqtt_sp_start_suback_notify(1, dcepf_mqtt_sp_notify_suback_cb_t);
    dcepf_mqtt_sp_start_message_notify(1, dcepf_mqtt_sp_notify_message_cb_t);
    dcepf_mqtt_sp_start_disconnect_notify(1, dcepf_mqtt_sp_notify_disconnect_cb_t);
    dcepf_mqtt_sp_start_puback_notify(1, dcepf_mqtt_sp_notify_puback_cb_t);
    dcepf_mqtt_sp_start_unsuback_notify(1, dcepf_mqtt_sp_notify_unsuback_cb_t);
    //
    dcepf_mqtt_sp_invoke_client_init(2,"VINDI2", dcepf_mqtt_sp_invoke_client_init_cb_t);
    dcepf_mqtt_sp_start_connack_notify(2, dcepf_mqtt_sp_notify_connack_cb_t);
    dcepf_mqtt_sp_start_suback_notify(2, dcepf_mqtt_sp_notify_suback_cb_t);
    dcepf_mqtt_sp_start_message_notify(2, dcepf_mqtt_sp_notify_message_cb_t);
    dcepf_mqtt_sp_start_disconnect_notify(2, dcepf_mqtt_sp_notify_disconnect_cb_t);
    dcepf_mqtt_sp_start_puback_notify(2, dcepf_mqtt_sp_notify_puback_cb_t);
    dcepf_mqtt_sp_start_unsuback_notify(2, dcepf_mqtt_sp_notify_unsuback_cb_t);
}

void connect() {
    dcepf_mqtt_sp_invoke_connect(1, "test.mosquitto.org", 1883, 60, nullptr);
    dcepf_mqtt_sp_invoke_connect(2, "test.mosquitto.org", 1883, 60, nullptr);
}

void sub()
{
    dcepf_mqtt_sp_invoke_subscribe(1, "test/p1", 0);
    dcepf_mqtt_sp_invoke_subscribe(1, "test/p2", 0);
    dcepf_mqtt_sp_invoke_subscribe(2, "test/p3", 0);
    dcepf_mqtt_sp_invoke_subscribe(2, "test/p4", 0);
}

void pub() {
    dcepf_mqtt_sp_invoke_publish(1, "test/t1", "60", 0);
    dcepf_mqtt_sp_invoke_publish(1, "test/t2", "61", 1);
    dcepf_mqtt_sp_invoke_publish(2, "test/t3", "62", 2);
}

void disconnect()
{
    dcepf_mqtt_sp_invoke_disconnect(1);
    // dcepf_mqtt_sp_invoke_client_cleanup(100);
    dcepf_mqtt_sp_invoke_disconnect(2);
    // dcepf_mqtt_sp_invoke_client_cleanup(200);
}

void unsub()
{
    dcepf_mqtt_sp_invoke_unsubscribe(1, "test/p1");
    dcepf_mqtt_sp_invoke_unsubscribe(1, "test/p2");
    dcepf_mqtt_sp_invoke_unsubscribe(2, "test/p3");
    dcepf_mqtt_sp_invoke_unsubscribe(2, "test/p4");
}


int main() {
    init();
    int cmd;
    while(true) {
        cout << "0. Re init" << endl;
        cout << "1. Connect" << endl;
        cout << "2. Disconnect" << endl;
        cout << "3. Sub" << endl;
        cout << "4. Pub" << endl;
        cout << "5. Unsub" << endl;
        cin >> cmd;
        switch (cmd)
        {
        case 0:
            init();
            break;
        case 1:
            connect();
            break;
        case 2:
            disconnect();
            break;
        case 3:
            sub();
            break;
        case 4:
            pub();
            break;
        case 5:
            unsub();
            break;
        default:
            break;
        }
    }
    return 0;
}





int main2() {
    dcepf_mqtt_sp_invoke_client_init(1,"VINDI1", dcepf_mqtt_sp_invoke_client_init_cb_t);
    // Callback set
    dcepf_mqtt_sp_start_connack_notify(1, dcepf_mqtt_sp_notify_connack_cb_t);
    dcepf_mqtt_sp_start_suback_notify(1, dcepf_mqtt_sp_notify_suback_cb_t);
    dcepf_mqtt_sp_start_message_notify(1, dcepf_mqtt_sp_notify_message_cb_t);
    dcepf_mqtt_sp_start_disconnect_notify(1, dcepf_mqtt_sp_notify_disconnect_cb_t);
    dcepf_mqtt_sp_start_puback_notify(1, dcepf_mqtt_sp_notify_puback_cb_t);
    dcepf_mqtt_sp_start_unsuback_notify(1, dcepf_mqtt_sp_notify_unsuback_cb_t);
    this_thread::sleep_for(chrono::seconds(2));
    dcepf_mqtt_sp_invoke_connect(1, "test.mosquitto.org", 1883, 60, nullptr);
    this_thread::sleep_for(chrono::seconds(2));
    return 0;
}
