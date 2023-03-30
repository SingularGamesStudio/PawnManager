#include <vector>

class Recipe;

class BuildingIdea {
    std::vector<Recipe*> available;
    int radius;
    int index;
};

class BuildingRegisty {
    std::vector<BuildingIdea> database;
};
