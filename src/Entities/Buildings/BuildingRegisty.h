#include <vector>

class Recipe;

class BuildingIdea {
    std::vector<Recipe*> available;
    int radius;
    int index;
};

class BuildingRegisty {
public:
    static std::vector<BuildingIdea> database;
};
