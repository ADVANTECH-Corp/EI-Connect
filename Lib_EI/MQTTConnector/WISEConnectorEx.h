/****************************************************************************/
/* Copyright(C) : Advantech Technologies, Inc.								*/
/* Create Date  : 2017/01/20 by Scott Chang								    */
/* Modified Date: 2017/01/20 by Scott Chang									*/
/* Abstract     : WISE Connector Extend API definition						*/
/* Reference    : None														*/
/****************************************************************************/
#ifndef _WISE_CONNECTOR_EX_H_
#define _WISE_CONNECTOR_EX_H_
#include <stdbool.h>


typedef void (*WC_CONNECT_CALLBACK)(void *userdata);
typedef void (*WC_LOSTCONNECT_CALLBACK)(void *userdata);
typedef void (*WC_DISCONNECT_CALLBACK)(void *userdata);
typedef void (*WC_MESSAGE_CALLBACK)(const char* topic, const void* payload, const int payloadlen, void *userdata);
typedef void* WiConn_t;

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#include <windows.h>
#ifndef WISE_CONNECTOR_API
	#define WISE_CONNECTOR_API __declspec(dllexport)
#endif
#else
	#define WISE_CONNECTOR_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function: wc_ex_initialize
 *
 * Initialize the client instance.
 *
 * Parameters:
 * 	devid -			String to use as the client id. If NULL, a random client id will be generated.
 * 	userdata -		A user pointer that will be passed as an argument to any
 *                  callbacks that are specified.
 *
 * Returns:
 * 	pointer of WISE Connect Object.
 *
 */
WISE_CONNECTOR_API WiConn_t wc_ex_initialize(char const * devid, void* userdata);

/*
 * Function: wc_ex_uninitialize
 *
 * Call to free resources associated with the library.
 *
 * Parameters:
 * conn -		pointer of WISE Connect Object.
 *
 * Returns:
 * 	None
 *
 */
WISE_CONNECTOR_API void wc_ex_uninitialize(WiConn_t conn);

/* 
 * Function: wc_ex_callback_set
 *
 * Set the connect callback. This is called when the broker sends a CONNACK
 * message in response to a connection.
 *
 * Parameters:
 *  conn -				pointer of WISE Connect Object.
 *  connect_cb -		a callback function in the following form:
 *						void callback(void *userdata)
 *  lostconnect_cb -	a callback function in the following form:
 *						void callback(int rc, void *userdata)
 *  disconnect_cb -		a callback function in the following form:
 *						void callback(void *userdata)
 *  message_cb -		a callback function in the following form:
 *						void callback(const char* topic, const void* payload, const int payloadlen, void *userdata)
 *
 * Callback Parameters:
 *  rc -		the return code of the connection response from internal connection library.
 *  userdata -	the user data provided in <wc_initialize>
 *  topic -		the topic that message received 
 *  payload -   the received message
 *  payloadlen - the length of received message
 *
 */
WISE_CONNECTOR_API void wc_ex_callback_set(WiConn_t conn, WC_CONNECT_CALLBACK connect_cb, WC_LOSTCONNECT_CALLBACK lostconnect_cb, WC_DISCONNECT_CALLBACK disconnect_cb, WC_MESSAGE_CALLBACK message_cb);

/*
 * Function: wc_ex_tls_set
 *
 * Configure the client for certificate based SSL/TLS support. Must be called
 * before <wc_connect>.
 *
 * Cannot be used in conjunction with <wc_tls_psk_set>.
 *
 * Define the Certificate Authority certificates to be trusted (ie. the server
 * certificate must be signed with one of these certificates) using cafile.
 *
 * If the server you are connecting to requires clients to provide a
 * certificate, define certfile and keyfile with your client certificate and
 * private key. If your private key is encrypted, provide a password callback
 * function.
 *
 * Parameters:
 *  conn -		  pointer of WISE Connect Object.
 *  cafile -      path to a file containing the PEM encoded trusted CA
 *                certificate files. Either cafile or capath must not be NULL.
 *  capath -      path to a directory containing the PEM encoded trusted CA
 *                certificate files. See mosquitto.conf for more details on
 *                configuring this directory. Either cafile or capath must not
 *                be NULL.
 *  certfile -    path to a file containing the PEM encoded certificate file
 *                for this client. If NULL, keyfile must also be NULL and no
 *                client certificate will be used.
 *  keyfile -     path to a file containing the PEM encoded private key for
 *                this client. If NULL, certfile must also be NULL and no
 *                client certificate will be used.
 *  password -	  if keyfile is encrypted, set the password to allow your client
 *                to pass the correct password for decryption.
 *
 * Returns:
 *	true or false.
 *
 */
WISE_CONNECTOR_API bool wc_ex_tls_set(WiConn_t conn, const char *cafile, const char *capath, const char *certfile, const char *keyfile, const char* password);

/*
 * Function: wc_ex_tls_psk_set
 *
 * Configure the client for pre-shared-key based TLS support. Must be called
 * before <wc_connect>.
 *
 * Cannot be used in conjunction with <wc_tls_set>.
 *
 * Parameters:
 *  conn -	   pointer of WISE Connect Object.
 *  psk -      the pre-shared-key in hex format with no leading "0x".
 *  identity - the identity of this client. May be used as the username
 *             depending on the server settings.
 *	ciphers -  a string describing the PSK ciphers available for use. See the
 *	           "openssl ciphers" tool for more information. If NULL, the
 *	           default ciphers will be used.
 *
 * Returns:
 *	true or false.
 *
 */
WISE_CONNECTOR_API bool wc_ex_tls_psk_set(WiConn_t conn, const char *psk, const char *identity, const char *ciphers);

/*
 * Function: wc_ex_connect
 *
 * Connect to server.
 *
 * Parameters:
 *  conn -		pointer of WISE Connect Object.
 * 	ip -		the hostname or ip address of the server to connect to.
 * 	port -      the network port to connect to. Usually 1883.
 * 	username -  the username to send as a string, or NULL to disable
 *              authentication.
 * 	password -  the password to send as a string. Set to NULL when username is
 * 	            valid in order to send just a username.
 * 	keepalive - the number of seconds to kept connection.
 *  willtopic - the topic on which to publish the will.
 *  willmsg -   pointer to the data to send. If payloadlen > 0 this must be a
 *              valid memory location.
 *  msglen -    the size of the willmsg (bytes).
 *
 * Returns:
 *	true or false.
 */
WISE_CONNECTOR_API bool wc_ex_connect(WiConn_t conn, char const * ip, int port, char const * username, char const * password, int keepalive, char* willtopic, const void *willmsg, int msglen );

/*
 * Function: wc_ex_disconnect
 *
 * Disconnect from the server.
 *
 * Parameters:
 *  conn -		pointer of WISE Connect Object.
 *	bForce -	floag to dsiconnect without waiting disconnect packet send.
 *
 * Returns:
 *	true or false.
 */
WISE_CONNECTOR_API bool wc_ex_disconnect(WiConn_t conn, bool bForce);

/* 
 * Function: wc_ex_publish
 *
 * Publish a message on a given topic.
 * 
 * Parameters:
 *  conn -		pointer of WISE Connect Object.
 *  topic -		the topic to publish message. 
 * 	msg -		pointer to the data to send. If msglen > 0 this must be a
 *				valid memory location.
 * 	msglen -	the size of the payload (bytes). 
 * 	retain	-	enable flag to retain this message in broker.
 *  qos		-	QoS 1, 2, 3
 *
 * Returns:
 * 	true or false.
 *
 */
WISE_CONNECTOR_API bool wc_ex_publish(WiConn_t conn, const char* topic, const void *msg, int msglen, int retain, int qos);

/*
 * Function: wc_ex_subscribe
 *
 * Subscribe to a topic.
 *
 * Parameters:
 *  conn -	pointer of WISE Connect Object.
 *	topic -	the subscription pattern.
 *  qos	-	QoS 1, 2, 3
 *
 * Returns:
 *	true or false.
 */
WISE_CONNECTOR_API bool wc_ex_subscribe(WiConn_t conn, const char* topic, int qos);

/*
 * Function: wc_ex_unsubscribe
 *
 * Unsubscribe from a topic.
 *
 * Parameters:
 *  conn -	pointer of WISE Connect Object.
 *	topic -	the unsubscription pattern.
 *
 * Returns:
 *	true or false.
 */
WISE_CONNECTOR_API bool wc_ex_unsubscribe(WiConn_t conn, const char* topic);

/*
 * Function: wc_ex_address_get
 *
 * To get local IP address.
 *
 * Parameters:
 *  conn -		pointer of WISE Connect Object.
 *	address	-	to get local IP address.
 *
 * Returns:
 *	true or false.
 */
WISE_CONNECTOR_API bool wc_ex_address_get(WiConn_t conn, char *address);

/*
 * Function: wc_ex_current_error_string_get
 *
 * Call to obtain a const string description the error number.
 *
 * Parameters:
 *  conn -		pointer of WISE Connect Object.
 *
 * Returns:
 *	A constant string describing the error.
 */
WISE_CONNECTOR_API const char* wc_ex_current_error_string_get(WiConn_t conn);


#ifdef __cplusplus
}
#endif

#endif //_WISE_CONNECTOR_EX_H_
