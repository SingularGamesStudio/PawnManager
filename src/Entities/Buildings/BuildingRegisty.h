#include <vector>

class Recipe;

struct BuildingIdea {
    std::vector<Recipe*> available;
    int radius;
    int index;
    BuildingIdea(int radius, int index):radius(radius), index(index){}
};

class BuildingRegisty {
public:
    static std::vector<BuildingIdea> database;

    static void init();
};
