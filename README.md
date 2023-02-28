# MQTT
MQTT

cd /usr/include

need libmosquitto-dev to get the Header-Files for Debian.

-> apt-get update

-> apt-get install libmosquitto-dev

 copy "/usr/include/mosquitto.h" to your directory with your "client.c"-file.

Or you change:
#include "mosquitto.h"
to:
#include <mosquitto.h>
in the "client.c"-file (line 26) for a global search for the Header-File.