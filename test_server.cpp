#include "server.h"

dlib::serverInterface serv(57179);

int main(){
	serv.start();
	while(1) {
		serv.respond();
	}
	serv.stop();
	return 0;
}