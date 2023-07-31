#include <iostream>
#include <time.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string.h>
#include<unistd.h>         
#include "example_emu2pc.h"    
#include "example_pc2emu_server.h"
#include "erpc_transport_setup.h"
#include <erpc_server_setup.h>
#include <erpc_arbitrated_client_setup.h>

using namespace std;

bool remote_connected = false;

/** Return the current time string */
static std::string now_str() {
	time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%F %T");
	return ss.str();
}

/** The servicer end implements the interface method */
binary_t * sendCanMsg(const binary_t * txInput){
	remote_connected = true;
	cout << "sendCanMsg called" << endl;
	string o ((char*)txInput->data);
	o.append("@").append(now_str());
	auto ol = strlen(o.c_str());
	char* buf = (char*)malloc(ol + 1);
	strncpy(buf,o.c_str(),ol);
	return new binary_t{(uint8_t*)buf,(uint32_t)ol};
}

void *tfunc(void *ptr) {
	uint32_t num_msg_sent = 0;
	while (1) {
		sleep(1);
		cout << "client thread running ..." << endl;

		if (remote_connected) {
			char msg[100] = {0};
			snprintf(msg, sizeof(msg), "CAN Message Received %d", num_msg_sent);
			binary_t b = {(uint8_t*)msg,(uint32_t)strlen(msg)};
			/* RPC call */
			canMsgRcvd(&b);
			cout << "send CAN Notification ..." << endl;
			num_msg_sent++;
		} else {
			cout << "remote not connected ..." << endl;
		}
	}
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {

	pthread_t cthread;
	int cthread_handle;

    cout << "ERPC Test Server ..." << endl;

	erpc_transport_t arbitrator;
	erpc_transport_t transport = erpc_transport_tcp_init("127.0.0.1", 5407, true);

	// MessageBufferFactory initialization
	cout << "initializing message buffer factory ..." << endl;
	erpc_mbf_t message_buffer_factory = erpc_mbf_dynamic_init();

	erpc_client_t client = erpc_arbitrated_client_init(transport, message_buffer_factory, &arbitrator);

	// eRPC server initialization
	cout << "initializting server ..." << endl;
	auto server = erpc_server_init(arbitrator, message_buffer_factory);

	// Add the generated interface service DEMO to the server,
	// see the generated source file erpcdemo_server.h
	cout << "adding service to server ..." << endl;
	erpc_add_service_to_server(server, create_IoExpanderEmulator_service());

	cthread_handle = pthread_create(&cthread, NULL, tfunc, NULL);

	// Start the server
	cout << "starting erpcdemo server ..." << endl;
	erpc_server_run(server); /* or erpc_server_poll(); */

	cout << "server is running ..." << endl;

	// Close the socket
	erpc_transport_tcp_close();

	return 0;

}
