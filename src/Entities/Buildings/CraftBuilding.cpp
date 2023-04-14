#include "CraftBuilding.h"
#include "../../godobject.h"

#include <iostream>
#ifdef SERVER_SIDE
void CraftBuilding::stopRecipe() {
    if (current == nullptr) return;
    current->cancel();
    delete current;
    current = nullptr;
}

bool CraftBuilding::assignRecipe(Recipe* recipe) {
    stopRecipe();
    if (!recipe->checkRequirements(ptr<CraftBuilding>(id))) return false;
    current = recipe->clone();
    current->start(ptr<CraftBuilding>(id));
    return true;
}

void CraftBuilding::tick(double deltaTime) {
    if (current != nullptr) { current->tick(deltaTime); }
}
#endif
CraftBuilding::~CraftBuilding() {
#ifdef SERVER_SIDE
    stopRecipe();
#endif
}

BuildingType CraftBuilding::getType() const {
    return BuildingType::CRAFT_BUILDING;
}

std::vector<uint8_t> CraftBuilding::serialize() const { return serializeSelf(); }
size_t CraftBuilding::deserialize(const std::vector<uint8_t>& data) { return deserializeSelf(data); }

std::vector<uint8_t> CraftBuilding::serializeSelf() const {
    std::vector<uint8_t> result = Building::serializeSelf();
    result.resize(result.size() + sizeof(size_t));
    copyVariable(result.data() + result.size() - sizeof(size_t), recipes.size());
    for(Recipe* i : recipes) {
        std::vector<uint8_t> tmp = i->serialize();
        std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));
    }
    return result;
}

size_t CraftBuilding::deserializeSelf(const std::vector<uint8_t>& data) {
    size_t shift = Building::deserializeSelf(data);
    const uint8_t* curr = data.data() + shift;
    size_t size;
    curr += initializeVariable(curr, size);
    recipes.resize(size);
    for(size_t i = 0; i < size; ++i) {
        curr += godObject::getRecipe(curr, recipes[i]);
    }
    return curr - data.data();
}