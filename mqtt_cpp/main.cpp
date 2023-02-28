#include <iostream>
#include <string>
#include "MyMosquitto.h"
#include<thread>
#include<chrono>
using namespace std;

static void on_ping_wrapper(struct mosquitto *mosq, void *userdata, int rc)
{
	class mosquittopp *m = (class mosquittopp *)userdata;
	UNUSED(mosq);
	m->on_ping(rc);
}

int main()
{
    MyMosquitto* mosq = new MyMosquitto();
    mosq->connect("broker.emqx.io", 1883, 60);
    mosq->publish("automative/temparature","50");
    mosq->subscribe("automative/v");

    this_thread::sleep_for(chrono::seconds(20));
    //mosq->close();
    return 0;
}