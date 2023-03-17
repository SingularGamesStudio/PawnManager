#include "client.h"

class myClient : public dlib::clientInterface {
public:
	virtual void onPacketReceive(const dlib::Packet& p) override {
		std::cout << "CLIENT received Packet:\n";
        for(auto i : p.data)
        	std::cout << char(i);
        std::cout << std::endl;
    }
	void sendDick(){
		dlib::Packet p(dlib::Packet::Type::RAW_MESSAGE);
		p << '.' << '|' << '.';
		send(p);
	}
};

int main() {
	myClient c;
	c.connect("127.0.0.1", 57179);
	while(1){
		c.respond();
		
	}
	return 0;
}