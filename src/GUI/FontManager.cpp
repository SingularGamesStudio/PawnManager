//
// Created by goodm on 30.03.2023.
//

#include "FontManager.h"
#include "EmbeddedFont.h"


FontManager::FontManager() : f() {
    f.loadFromMemory(font, sizeof(font));
}
