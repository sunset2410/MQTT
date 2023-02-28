#include <iostream>
#include <mosquitto.h>
#include<thread>
#include<chrono>
#include<cstring>

using namespace std;

// chieu.tran
static void on_ping_wrapper(struct mosquitto *mosq, void *userdata, int rc)
{
	cout << "ping \n";
}

// struct mqtt5__property {
// 	struct mqtt5__property *next;
// 	union {
// 		uint8_t i8;
// 		uint16_t i16;
// 		uint32_t i32;
// 		uint32_t varint;
// 		struct mqtt__string bin;
// 		struct mqtt__string s;
// 	} value;
// 	struct mqtt__string name;
// 	int32_t identifier;
// 	bool client_generated;
// };

typedef mqtt5__property mosquitto_property;

int main()
{
    cout << "main start"<< endl;
	int rc;
	struct mosquitto* mosq;
	mosquitto_lib_init();
	mosq = mosquitto_new("publisher-test", true, NULL);

    cout << "main 1 "<< endl;
	// callback set
	//mosquitto_publish(mosq, NULL, "test/t1", 5, "hello", 0, false);
	//mosquitto_ping_callback_set(mosq, on_ping_wrapper);

	//rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 60);
	if(rc != 0){

    cout << "main 2 client can not connect to broker "<< endl;
	printf("client can not connect to broker ");
	mosquitto_destroy(mosq);
	return -1;
	}

    cout << "main 3 connect success to broker"<< endl;
	printf("connect success to broker");

    mosquitto_property  *properties = NULL;
    // Add properties
    mosquitto_property_free_all(&properties);

    if (properties == NULL) {
        cout << "properties null" << endl;
    } else {
        cout << "properties not null" << endl;
    }

    rc;
    const char* value {"test"};
    const uint16_t _len{4};
	rc = mosquitto_property_add_binary(&properties, 22, value, _len);
    cout << "rc----:" << rc << endl;
  

    //mosquitto_property_add_byte(&properties, 1U, 99);

    if (properties == NULL) {
        cout << "properties null" << endl;
    } else {
        cout << "properties not null" << endl;
    }

    uint8_t keepalive = 0; // default value
    cout << (int)keepalive << endl;
    const mosquitto_property  *prop = mosquitto_property_read_byte(properties, 1U, &keepalive, false);
    if (prop != NULL)
        cout << (int)keepalive << endl;
    else {
        cout << "mosquitto_property_read_byte not found" << endl;
    }    

	mosquitto_publish(mosq, NULL, "test/t1", 5, "hallo", 0, false);
	while(rc == 0){
		rc = mosquitto_loop(mosq, -1, 1);
	}

	cout << "main 4 "<< endl;
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;



}
