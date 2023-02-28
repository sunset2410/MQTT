g++ mqtt_pub.c -o main -lmosquitto


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


#####################################################

install -m 644 mosquitto_passwd.1 "/usr/local/share/man/man1/mosquitto_passwd.1"
install -m 644 mosquitto_pub.1 "/usr/local/share/man/man1/mosquitto_pub.1"
install -m 644 mosquitto_sub.1 "/usr/local/share/man/man1/mosquitto_sub.1"
install -m 644 mosquitto_rr.1 "/usr/local/share/man/man1/mosquitto_rr.1"
install -d "/usr/local/share/man/man7"
install -m 644 mqtt.7 "/usr/local/share/man/man7/mqtt.7"
install -m 644 mosquitto-tls.7 "/usr/local/share/man/man7/mosquitto-tls.7"
install -d "/usr/local/share/man/man3"
install -m 644 libmosquitto.3 "/usr/local/share/man/man3/libmosquitto.3"
make[1]: Leaving directory '/home/sunrise/projects/MQTT/MQTT/build_mosquitto/mosquitto/man'
install -d "/etc/mosquitto"
install -m 644 mosquitto.conf "/etc/mosquitto/mosquitto.conf.example"
install -m 644 aclfile.example "/etc/mosquitto/aclfile.example"
install -m 644 pwfile.example "/etc/mosquitto/pwfile.example"
install -m 644 pskfile.example "/etc/mosquitto/pskfile.example"
/sbin/ldconfig.real: /usr/local/lib/libmosquittopp.so.1 is not a symbolic link

/sbin/ldconfig.real: /usr/local/lib/libmosquitto.so.1 is not a symbolic link