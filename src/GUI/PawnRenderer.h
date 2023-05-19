//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_PAWNRENDERER_H
#define PAWNMANAGER_PAWNRENDERER_H
#include <set>

#include "SFML/Graphics/RenderWindow.hpp"

enum class expertisesID : uint8_t;
enum class FighterPawnType : uint8_t;

class PawnRenderer {
private:
    sf::RenderWindow& window;

public:
    explicit PawnRenderer(sf::RenderWindow& window);
    void drawWorkerPawn(const std::set<expertisesID>& expertises, sf::Vector2f pos, float size = 12);
    void drawFighterPawn(FighterPawnType type, sf::Vector2f pos, float size = 15);
};


#endif//PAWNMANAGER_PAWNRENDERER_H
