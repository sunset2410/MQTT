/**
  * Mosquitto C++ Wrapper
  * (C) 2017, Felix Niederwanger
  * MIT License
  */

#ifndef _MOSQUITTO_HPP_
#define _MOSQUITTO_HPP_

#include <string>
#include <sstream>

#include <unistd.h>
#include <mosquitto.h>


class MyMosquitto {
private:
    struct mosquitto *mosq;
	volatile bool running;
    
public:
	MyMosquitto();
	virtual ~MyMosquitto();
	
	/** Enable tls
	  * @param cacert CaCert file
	  * @param certfile Client certificate file. usually a .crt file
	  * @param keyfile Client key file, usually a .key file
	  */
	void set_tls(const char* cacert, const char* certfile, const char* keyfile);
	/** Enable or disable insecure TLS connections (No certificate check) */
	void set_tls_insecure(bool enabled);
	
	/** Set username and password for login */
	void set_username_password(const char* username, const char* password);
	
	/** Disconnects the client */
	void close();
	
	/** Subscribe to the given topic */
	void subscribe(const std::string &topic);
	
	void publish(const std::string &topic, const std::string &message);
	
	/**
	  * Connects to the given remote host
	  * @param remote Remote host where to connect to
	  * @param port Remote port. 1883 for unencrypted, 8883 usually for encrypted traffic
	  * @param aliveDelay Delay in seconds for pings for the connection to stay alive
	  */
	void connect(const char* remote, const int port = 1883, const int aliveDelay = 30);

	/** Method called when the client is connected */
	virtual void onConnected() {}
	/** Method called then an error occurred */
	virtual void onError(const char* msg) { (void)msg; }
	/** Method called when a new message arrives */
	virtual void onMessage(std::string topic, std::string message) { (void)topic; (void)message; }
	
	/** Loop through messages. This call usually blocks until the connection is closed
	  *@param tryReconnect if true, the client tries to reconnect if an error occurs */
	void loop(const bool tryReconnect = true);
	
	/** Cleanup mosquitto library. This call should be called before program termination */
	static void cleanup_library();
};


#endif