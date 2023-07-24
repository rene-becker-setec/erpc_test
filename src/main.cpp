#include <iostream>
#include <time.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string.h>
#include "example_server.h"

using namespace std;

/** Return the current time string */
static std::string now_str() {
	time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%F %T");
	return ss.str();
}

/** The servicer end implements the interface method */
binary_t * RD_demoHello(const binary_t * txInput){
	cout << "RD_demoHello called" << endl;
	string o ((char*)txInput->data);
	o.append("@").append(now_str());
	auto ol = strlen(o.c_str());
	char* buf = (char*)malloc(ol + 1);
	strncpy(buf,o.c_str(),ol);
	return new binary_t{(uint8_t*)buf,(uint32_t)ol};
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
    cout << "Welcome";
}

