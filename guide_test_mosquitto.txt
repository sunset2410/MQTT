1. cai dat thu vien mosquitto vao ubuntu

sudo apt-get update
sudo apt-get install mosquitto
sudo apt-get install libmosquitto-dev


g++ --version

g++ main.cpp -o main


//code

#include <stdio.h>
#include <mosquitto.h>

int main()
{

	int rc;
	struct mosquitto* mosq;
	mosquitto_lib_init();
	mosq = mosquitto_new("publisher-test", true, NULL);
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


//







=====================================
need libmosquitto-dev to get the Header-Files for Debian.

-> apt-get update

-> apt-get install libmosquitto-dev

 copy "/usr/include/mosquitto.h" to your directory with your "client.c"-file.

Or you change:
#include "mosquitto.h"
to:
#include <mosquitto.h>
in the "client.c"-file (line 26) for a global search for the Header-File.


