//
// Created by goodm on 04.05.2023.
//

#include "TextInputControl.h"
#include "GameWindow.h"

void TextInputControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect r = win.getGridRectangle(rect);
    win.drawBorderedRect(r, 4, sf::Color::Black, sf::Color::White);
}
TextInputControl::TextInputControl(GameWindow &win, const sf::IntRect &rect) : Control(win, rect), text(""), pos(0) {}
void TextInputControl::onCharInput(int ch) {
    Control::onCharInput(ch);
    if(win.selectedControl == this) {
        if(ch == 8) {
            if(pos != 0) {
                text.erase(text.begin() + pos);
                --pos;
            }
        }
    }
}
