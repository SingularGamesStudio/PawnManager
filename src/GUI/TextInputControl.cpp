//
// Created by goodm on 04.05.2023.
//

#include "TextInputControl.h"
#include "PawnManagerClient.h"

#include "GameWindow.h"
#include "SFML/Graphics/Text.hpp"



void TextInputControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect r = win.getGridRectangle(rect);
    win.drawBorderedRect(r, 4, sf::Color::Black, sf::Color::White);
    std::string text2 = text;
    text2.insert(text2.begin() + pos, '|');
    sf::Text t(text2, PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color::White);
    sf::Vector2f delta = (r.getSize() - t.getLocalBounds().getSize()) * 0.5f;
    t.setPosition(r.getPosition() + delta - sf::Vector2f(0, 5));
    PawnManagerClient::window->draw(t);
}
TextInputControl::TextInputControl(GameWindow &win, const sf::IntRect &rect) : Control(win, rect), text(""), pos(0) {}
void TextInputControl::onCharInput(int ch) {
    Control::onCharInput(ch);
    if(ch >= 32 && ch < 127) {
        text.insert(text.begin() + pos, ch);
        ++pos;
    }
}
void TextInputControl::onKeyInput(sf::Keyboard::Key k) {
    Control::onKeyInput(k);
    if(k == sf::Keyboard::Left) {
        if(pos != 0) {
            --pos;
        }
    } else if(k == sf::Keyboard::Right) {
        if(pos != text.size()) {
            ++pos;
        }
    } else if(k == sf::Keyboard::BackSpace) {
        if(pos != 0) {
            text.erase(text.begin() + pos - 1);
            --pos;
        }
    } else if(k == sf::Keyboard::Delete) {
        if(pos != text.size()) {
            text.erase(text.begin() + pos);
        }
    }
}
