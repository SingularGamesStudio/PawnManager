#include "SFML/Window.hpp"

int main() {
    sf::Window window(sf::VideoMode(800, 600), "Pawn Manager");
    while (window.isOpen()) {
        sf::Event evt;
        while(window.pollEvent(evt)) {
            if(evt.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    return 0;
}