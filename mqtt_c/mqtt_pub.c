#include <iostream>
#include <mosquitto.h>
#include<thread>
#include<chrono>
using namespace std;

// chieu.tran
static void on_ping_wrapper(struct mosquitto *mosq, void *userdata, int rc)
{
	cout << "ping \n";
}


int main()
{

	int rc;
	struct mosquitto* mosq;
	mosquitto_lib_init();
	mosq = mosquitto_new("publisher-test", true, NULL);

	// callback set
	mosquitto_publish(mosq, NULL, "test/t1", 5, "hello", 0, false);
	mosquitto_ping_callback_set(mosq, on_ping_wrapper);

	//rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 60);
	if(rc != 0){

	printf("client can not connect to broker ");
	mosquitto_destroy(mosq);
	return -1;
	}

	printf("connect success to broker");
	mosquitto_publish(mosq, NULL, "test/t1", 5, "hello", 0, false);
	while(rc == 0){
		rc = mosquitto_loop(mosq, -1, 1);
	}

	//
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;



}
