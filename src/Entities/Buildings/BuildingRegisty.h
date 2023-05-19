#ifndef BUILDINGREGISTRY_H
#define BUILDINGREGISTRY_H
#include <vector>
#include <string>

class Recipe;
class BuildRecipe;

struct BuildingIdea {
    std::vector<Recipe*> available;
    int radius;
    int index;
#ifdef CLIENT_SIDE
    std::string name;
#endif
    BuildingIdea(int radius, int index) : radius(radius), index(index)
#ifdef CLIENT_SIDE
                                          , name()
#endif
    {}
};

class BuildingRegisty {
public:
    static std::vector<BuildRecipe*> database;

    static void init();
};
#endif