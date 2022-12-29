#include "MyMosquitto.h"


void mosquitto_callback_on_connect(struct mosquitto *mosq_obj, void *obj, int rc) {
	(void)mosq_obj;
	MyMosquitto *mosq = (MyMosquitto*)obj;
	
	switch(rc) {
	case 0:
		// Connected
		mosq->onConnected();
		break;
	case 1:
		mosq->onError("Connection refused (unacceptable protocol version)");
		break;
	case 2:
		mosq->onError("Connection refused (identifier rejected)");
		break;
	case 3:
		mosq->onError("Connection refused (broker unavailable)");
		break;
	case 5:
		mosq->onError("Connection refused (Username/Password wrong)");
		break;
	default:
		std::stringstream ss ;
		ss << "Connection failed (Error code " << rc << ")";
		std::string msg = ss.str();
		mosq->onError(msg.c_str());
		break;
	};
}

static void mosquitto_callback_on_message(struct mosquitto *mosq_obj, void *obj, const struct mosquitto_message *mosq_message) {
	(void)mosq_obj;
	MyMosquitto *mosq = (MyMosquitto*)obj;
	std::string topic(mosq_message->topic);
	
	// Build message
	std::stringstream ss;
	const int len = mosq_message->payloadlen;
	char* payload = (char*)mosq_message->payload;
	for(int i=0;i<len;i++) ss << (char)(payload[i]);
	
	mosq->onMessage(topic, ss.str());
}




MyMosquitto::MyMosquitto() {
    mosquitto_lib_init();
    this->running = false;
    this->mosq = mosquitto_new(NULL, true, this);
    if(this->mosq == NULL) throw "Error creating mosquitto instance";
    
    
	mosquitto_connect_callback_set(this->mosq, &mosquitto_callback_on_connect);
	mosquitto_message_callback_set(this->mosq, &mosquitto_callback_on_message);
}


MyMosquitto::~MyMosquitto() {
    mosquitto_destroy(this->mosq);
}

static int mosquitto_hpp_pw_callback(char *buf, int size, int rwflag, void *userdata) {
	(void)buf;
	(void)size;
	(void)rwflag;
	(void)userdata;
	return -1;
}

void MyMosquitto::set_tls(const char* cacert, const char* certfile, const char* keyfile) {
	if(mosquitto_tls_set(this->mosq, cacert, NULL, certfile, keyfile, &mosquitto_hpp_pw_callback) != MOSQ_ERR_SUCCESS) 
		throw "Error setting tls settings";
}

void MyMosquitto::set_tls_insecure(bool enabled) {
	int value = (enabled?1:0);
	if(mosquitto_tls_insecure_set(this->mosq, value) != MOSQ_ERR_SUCCESS) 
		throw "TLS insecure set failed";
}

void MyMosquitto::set_username_password(const char* username, const char* password) {
	if(mosquitto_username_pw_set(this->mosq, username, password) != MOSQ_ERR_SUCCESS)
		throw "Error setting username and password";
}


void MyMosquitto::connect(const char* remote, const int port, const int aliveDelay) {	
	int ret = mosquitto_connect(this->mosq, remote, port, aliveDelay);
	if(ret != MOSQ_ERR_SUCCESS)
		throw mosquitto_strerror(ret);
	this->running = true;
}
	
void MyMosquitto::subscribe(const std::string &topic) {
	const int qos = 0;
	int ret = mosquitto_subscribe(this->mosq, NULL, topic.c_str(), qos);
	if(ret != MOSQ_ERR_SUCCESS)
		throw mosquitto_strerror(ret);
}

void MyMosquitto::publish(const std::string &topic, const std::string &message) {
	const char *payload = message.c_str();
	const int len = (int)message.size();
	
	const int qos = 0;
	int ret = mosquitto_publish(this->mosq, NULL, topic.c_str(), len, (const void*)payload, qos, false);
	if(ret != MOSQ_ERR_SUCCESS)
		throw mosquitto_strerror(ret);
}

	
void MyMosquitto::close() {
	this->running = false;
	mosquitto_disconnect(this->mosq);
}

void MyMosquitto::loop(const bool tryReconnect) {
	int errorCounter = 0;
	
	// XXX: Consider switching to mosquitto_loop_forever
	
	do {
		const int ret = mosquitto_loop(this->mosq, 10000, 10);
		if(!this->running) return;
		
		if(ret != MOSQ_ERR_SUCCESS) {
			
			// Sleep for some time, if necessary
			if(errorCounter > 1) {
				int duration = errorCounter*errorCounter;
				if(duration > 60 || duration <= 0) duration = 60;
				::sleep(duration);		// Wait some time before trying again
			}
			
			if(mosquitto_reconnect(mosq) == MOSQ_ERR_SUCCESS) {
				// Reconnect successfull
				continue;
			} else {
				// Reconnect failed. Increase counter
				errorCounter++;
			}
		} else
			errorCounter = 0;
	} while(this->running && tryReconnect);
}

void MyMosquitto::cleanup_library() {
    mosquitto_lib_cleanup();
}