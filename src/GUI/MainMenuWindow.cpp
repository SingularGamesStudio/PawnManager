//
// Created by goodm on 16.03.2023.
//

#include "MainMenuWindow.h"

#include "ButtonControl.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"
#include "TextInputControl.h"
MainMenuWindow::MainMenuWindow() {
    slotCounts = sf::Vector2i(3, 3);
    controls.push_back(textInput = new TextInputControl(*this, sf::IntRect(0, 1, 2, 0)));
    textInput->text = "127.0.0.1";
    controls.push_back(new ButtonControl(*this, sf::IntRect(0, 2, 2, 0), "Connect", [this](){
        PawnManagerClient::connect(textInput->text, 57179);
        PawnManagerClient::winManager.popWindow();
    }));
}
void MainMenuWindow::updateAndRender() {
    GameWindow::updateAndRender();
    sf::FloatRect fr = getWindowRectangle();
    sf::Text t(std::string("PawnManager"), PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(0, 0, 0));
    t.setPosition(fr.getPosition() + sf::Vector2f(10, 10));
    PawnManagerClient::window->draw(t); }
