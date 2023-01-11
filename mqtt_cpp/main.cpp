#include <iostream>
#include <string>
#include "MyMosquitto.h"

int main()
{
    MyMosquitto* mosq = new MyMosquitto();
    mosq->connect("broker.emqx.io", 1883, 60);
    mosq->publish("automative/temparature","50");
    mosq->subscribe("automative/v");
    //mosq->close();
    return 0;
}