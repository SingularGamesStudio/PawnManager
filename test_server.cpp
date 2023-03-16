#include "server.h"

dlib::server_interface serv(57179);

int main(){
	serv.start();
	while(1) {
		serv.respond();
	}
	serv.stop();
	return 0;
}