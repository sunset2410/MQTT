#include <iostream>
#include "unistd.h"
#include "mqttmanager.h"
#include <mutex>
using namespace std;
std::mutex mainMutex;

void dcepf_mqtt_sp_invoke_client_init_cb_t(int handle, dcepf_result_t result, mosquitto *instance)
{
    std::lock_guard<std::mutex> lock(mainMutex);
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
    std::lock_guard<std::mutex> lock(mainMutex);
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "rc: " << rc << endl;
    cout << "flag: " << flag << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_disconnect_cb_t(int dcepf_handle_t, void *obj, int rc, const mosquitto_property *props)
{
    std::lock_guard<std::mutex> lock(mainMutex);
    cout << "dcepf_mqtt_sp_notify_disconnect_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "rc: " << rc << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_suback_cb_t(int dcepf_handle_t, void *obj, int mid, int qos_count, const int *granted_qos, const mosquitto_property *property)
{
    std::lock_guard<std::mutex> lock(mainMutex);
    cout << "dcepf_mqtt_sp_notify_suback_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "qos_count: " << qos_count << endl;
    cout << "granted_qo: " << *granted_qos << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_unsuback_cb_t(int dcepf_handle_t, void *obj, int mid, const mosquitto_property *property)
{
    std::lock_guard<std::mutex> lock(mainMutex);
    cout << "dcepf_mqtt_sp_notify_puback_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "mid: " << mid << endl;
    cout << "********************************************" << endl;
}

void dcepf_mqtt_sp_notify_message_cb_t(int dcepf_handle_t, void *obj, const struct mosquitto_message *message, const mosquitto_property *property)
{
    std::lock_guard<std::mutex> lock(mainMutex);
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
    std::lock_guard<std::mutex> lock(mainMutex);
    cout << "dcepf_mqtt_sp_notify_puback_cb_t" << endl;
    cout << "dcepf_handle_t: " << dcepf_handle_t << endl;
    cout << "mid: " << mid << endl;
    cout << "reason_code: " << reason_code << endl;
    cout << "********************************************" << endl;
}
void initDemo()
{
    // Init
    dcepf_mqtt_sp_invoke_client_init(100, "VIN01", dcepf_mqtt_sp_invoke_client_init_cb_t);
    dcepf_mqtt_sp_invoke_client_init(200, "VIN02", dcepf_mqtt_sp_invoke_client_init_cb_t);
    dcepf_mqtt_sp_invoke_client_init(300, "VIN03", dcepf_mqtt_sp_invoke_client_init_cb_t);

    // Callback set
    dcepf_mqtt_sp_start_connack_notify(100, dcepf_mqtt_sp_notify_connack_cb_t);
    dcepf_mqtt_sp_start_suback_notify(100, dcepf_mqtt_sp_notify_suback_cb_t);
    dcepf_mqtt_sp_start_message_notify(100, dcepf_mqtt_sp_notify_message_cb_t);
    dcepf_mqtt_sp_start_disconnect_notify(100, dcepf_mqtt_sp_notify_disconnect_cb_t);
    dcepf_mqtt_sp_start_puback_notify(100, dcepf_mqtt_sp_notify_puback_cb_t);
    dcepf_mqtt_sp_start_unsuback_notify(100, dcepf_mqtt_sp_notify_unsuback_cb_t);

    dcepf_mqtt_sp_start_connack_notify(200, dcepf_mqtt_sp_notify_connack_cb_t);
    dcepf_mqtt_sp_start_suback_notify(200, dcepf_mqtt_sp_notify_suback_cb_t);
    dcepf_mqtt_sp_start_message_notify(200, dcepf_mqtt_sp_notify_message_cb_t);
    dcepf_mqtt_sp_start_disconnect_notify(200, dcepf_mqtt_sp_notify_disconnect_cb_t);
    dcepf_mqtt_sp_start_puback_notify(200, dcepf_mqtt_sp_notify_puback_cb_t);
    dcepf_mqtt_sp_start_unsuback_notify(200, dcepf_mqtt_sp_notify_unsuback_cb_t);

    dcepf_mqtt_sp_start_connack_notify(300, dcepf_mqtt_sp_notify_connack_cb_t);
    dcepf_mqtt_sp_start_suback_notify(300, dcepf_mqtt_sp_notify_suback_cb_t);
    dcepf_mqtt_sp_start_message_notify(300, dcepf_mqtt_sp_notify_message_cb_t);
    dcepf_mqtt_sp_start_disconnect_notify(300, dcepf_mqtt_sp_notify_disconnect_cb_t);
    dcepf_mqtt_sp_start_puback_notify(300, dcepf_mqtt_sp_notify_puback_cb_t);
    dcepf_mqtt_sp_start_unsuback_notify(300, dcepf_mqtt_sp_notify_unsuback_cb_t);
}

void connect()
{
    // Connect
    dcepf_mqtt_sp_invoke_connect(100, "broker.hivemq.com", 1883, 60, nullptr);
    dcepf_mqtt_sp_invoke_connect(200, "broker.hivemq.com", 1883, 60, nullptr);
    dcepf_mqtt_sp_invoke_connect(300, "broker.hivemq.com", 1883, 60, nullptr);
}

void sub()
{
    // Sub
    //dcepf_mqtt_sp_invoke_subscribe(100, "topic/qos0", 0);
    //dcepf_mqtt_sp_invoke_subscribe(100, "topic/qos1", 1);
    //dcepf_mqtt_sp_invoke_subscribe(100, "topic/qos2", 2);
    //dcepf_mqtt_sp_invoke_subscribe(200, "topic/hello", 0);
    dcepf_mqtt_sp_invoke_subscribe(200, "test/t1", 0);
    dcepf_mqtt_sp_invoke_subscribe(300, "test/t2/", 0);
}

void pub()
{
    dcepf_mqtt_sp_invoke_publish(100, "test/t1", "6 0", 0);
    dcepf_mqtt_sp_invoke_publish(100, "test/t2", "6 1", 1);
    dcepf_mqtt_sp_invoke_publish(100, "test/t3", "6 2", 2);
}

void disconnect()
{
    dcepf_mqtt_sp_invoke_disconnect(100);
    // dcepf_mqtt_sp_invoke_client_cleanup(100);

    dcepf_mqtt_sp_invoke_disconnect(200);
    // dcepf_mqtt_sp_invoke_client_cleanup(200);

    dcepf_mqtt_sp_invoke_disconnect(300);
    // dcepf_mqtt_sp_invoke_client_cleanup(300);
}

void unsub()
{
    dcepf_mqtt_sp_invoke_unsubscribe(100, "test/t1");
    dcepf_mqtt_sp_invoke_unsubscribe(100, "test/t2");
    dcepf_mqtt_sp_invoke_unsubscribe(100, "test/t3");
    dcepf_mqtt_sp_invoke_unsubscribe(200, "test/#");
    dcepf_mqtt_sp_invoke_unsubscribe(300, "test/t2/#");
}

int main(int, char **)
{
    initDemo();

    int cmd;
    for (;;)
    {
        cout << "0. ReInit" << endl;
        cout << "1. Connect" << endl;
        cout << "2. Disconnect" << endl;
        cout << "3. Sub" << endl;
        cout << "4. Pub" << endl;
        cout << "5. Unsub" << endl;
        cin >> cmd;
        switch (cmd)
        {
        case 0:
            initDemo();
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