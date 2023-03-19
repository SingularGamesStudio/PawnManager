#include "server.h"
#include <ctime>

class myServer : public dlib::serverInterface {
public:
myServer(uint16_t port) : dlib::serverInterface(port) {}
virtual bool onConnection(std::shared_ptr<dlib::Connection> client) override {
	dlib::Packet p(dlib::Packet::Type::RAW_MESSAGE);
	p << 'H';
	client->send(p);
    return true;
}
virtual void onDisconnection(std::shared_ptr<dlib::Connection> client) override  {
}
virtual void onPacketReceive(std::shared_ptr<dlib::Connection> client, dlib::Packet p) override {
            std::cout << "SERVER received Packet:\n";
            for(char c : p.data){
                std::cout << c;
            }
            std::cout << "\n";
}

};

myServer serv(57179);

int main(){
	serv.start();
	unsigned long long cnt = 0;
	dlib::Packet p(dlib::Packet::Type::RAW_MESSAGE);
	p << 'P' << 'i' << 'Z' << 'z' << 'a';
	while(1) {
		if(cnt % 100000000 == 0) {
			serv.sendPacketAll(p);
		}
		serv.respond();
		cnt++;
	}
	serv.stop();
	return 0;
}

