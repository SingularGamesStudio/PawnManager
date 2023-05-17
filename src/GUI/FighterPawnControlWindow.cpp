//
// Created by goodm on 17.05.2023.
//

#include "FighterPawnControlWindow.h"

#include "../Core/godobject.h"
#include "../Entities/Buildings/Building.h"
#include "ButtonControl.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"
void FighterPawnControlWindow::updateAndRender() {
    std::vector<std::pair<FighterPawnType, int>> pawnCnts = getPawnCnts();
    if(pawnCnts.empty()) {
        PawnManagerClient::winManager.popWindow();
        return;
    }
    if(currentSelected >= pawnCnts.size()) {
        currentSelected = 0;
    }
    GameWindow::updateAndRender();
    sf::FloatRect fr = getWindowRectangle();
    sf::Text t(std::string(isProtection()?"Protect building " : "Attack building ") + std::to_string(currentSelected),
               PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(0, 0, 0));
    t.setPosition(fr.getPosition() + sf::Vector2f(10, 10));
    PawnManagerClient::window->draw(t);
    slot->drawType = 2;
    slot->fighterPawnType = pawnCnts[currentSelected].first;
    pawnCnt->label = std::string("Count: ") + std::to_string(pawnCnts[currentSelected].second);
}
FighterPawnControlWindow::FighterPawnControlWindow(ptr<Building> b) : currentSelected(0), b(b) {
    slotCounts = sf::Vector2i(5, 5);
    controls.push_back(new ButtonControl(*this, sf::IntRect(1, 4, 2, 0), "Send", [this](){
        //TODO send attack or protect to server
        godObject::local_server->mainPlayer->localAttack(this->b);
    }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(0, 4, 0, 0), "<", [this](){
        int pawnTypes = getPawnCnts().size();
        --currentSelected;
        if(currentSelected < 0) {
            currentSelected += pawnTypes;
        }
    }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(4, 4, 0, 0), ">", [this](){
        int pawnTypes = getPawnCnts().size();
        ++currentSelected;
        if(currentSelected >= pawnTypes) {
            currentSelected -= pawnTypes;
        }
    }));
    controls.push_back(slot = new SlotControl(*this, sf::IntRect(0, 1, 0, 0)));
    controls.push_back(pawnCnt = new LabelControl(*this, sf::IntRect(1, 1, 3, 0), "Count: 0"));
    controls.push_back(new LabelControl(*this, sf::IntRect(0, 2, 4, 0), "Count of pawns to send:"));
    controls.push_back(countInput = new TextInputControl(*this, sf::IntRect(0, 3, 4, 0)));
}
bool FighterPawnControlWindow::isProtection() { return b->owner == godObject::local_server->mainPlayer; }

std::vector<std::pair<FighterPawnType, int>> FighterPawnControlWindow::getPawnCnts() {
    std::map<FighterPawnType, int> res;
    for(ptr<Pawn> p : godObject::local_server->mainPlayer->pawns) {
        ptr<FighterPawn> fp = p.dyn_cast<FighterPawn>();
        if(fp && fp->currentTask.id == TaskID::Idle) {
            ++res[fp->getType()];
        }
    }
    return std::vector<std::pair<FighterPawnType, int>>(res.begin(), res.end());
}
