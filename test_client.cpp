#include "client.h"

class myClient : public dlib::clientInterface {
public:
	void sendDick(){
		dlib::Packet p(dlib::Packet::Type::RAW_MESSAGE);
		p << ".|.";
		send(p);
	}
};

//int main() {
//	myClient c;
//	c.connect("127.0.0.1", 57179);
//	while(1){
//		std::cin.get();
//		c.sendDick();
//	}
//	return 0;
//}