//
// Created by goodm on 16.03.2023.
//

#include "PawnManagerClient.h"
#include "SFML/Window.hpp"
#include "MainMenuWindow.h"
#include "../testSystem.h"

PawnManagerClient::PawnManagerClient() : window(sf::VideoMode(800, 600), "Pawn Manager"),
                                         view(window.getDefaultView()), winManager(), pawnRenderer(window) {
    winManager.pushWindow(new MainMenuWindow());
}

void PawnManagerClient::run() {
    player = initTest();
    while (window.isOpen()) {
        sf::Event evt{};
        while(window.pollEvent(evt)) {
            if(evt.type == sf::Event::Closed) {
                window.close();
            }
            if (evt.type == sf::Event::Resized) {
                view.setSize({
                                     static_cast<float>(evt.size.width),
                                     static_cast<float>(evt.size.height)
                             });
                view.setCenter(evt.size.width / 2, evt.size.height / 2);
                window.setView(view);
            }
        }
        updateAndRender();
        window.display();
    }
}

PawnManagerClient::~PawnManagerClient() {
    while(winManager.windowCount() > 0) {
        winManager.popWindow();
    }
}

void PawnManagerClient::updateAndRender() {
    tick();
    window.clear(sf::Color::White);
    winManager.updateAndRender();
    pawnRenderer.drawWorkerPawn({expertisesID::DummyMetalworking, expertisesID::DummtTrainership}, sf::Vector2f(300, 300));
}
