//
// Created by ME on 14.04.2023.
//

#include "godobject.h"

#include "Recipes/BuildRecipe.h"
#include "Recipes/CraftRecipe.h"
#include "Recipes/Recipe.h"

#ifdef SERVER_SIDE
GameServer* godObject::global_server = nullptr;
#endif

#ifdef CLIENT_SIDE
LocalController* godObject::local_server = nullptr;
#endif

size_t godObject::getRecipe(const uint8_t* data, Recipe*& out) {
    RecipeType type = static_cast<RecipeType>(data[0]);
    switch (type) {
        case RecipeType::BUILD_RECIPE:
            out = new BuildRecipe();
            break;
        case RecipeType::CRAFT_RECIPE:
            out = new CraftRecipe();
            break;
        default:
            throw std::logic_error("Recipe is pure virtual, no BASE_RECIPE allowed");
            break;
    }
    return out->deserialize(data + 1) + 1;
}