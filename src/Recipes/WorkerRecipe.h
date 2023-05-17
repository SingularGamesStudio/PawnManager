#ifndef WORKER_RECIPE_H
#define WORKER_RECIPE_H


#include "Recipe.h"


class WorkerRecipe : public Recipe {
protected:
    Recipe* cloneSelf() override;

public:
    std::vector<std::vector<expertisesID>> outWorkers;
    std::vector<expertisesID> trainExpertises;

    WorkerRecipe() {}
#ifdef SERVER_SIDE
    void finish() override;
#endif
    RecipeType getType() const override;
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const uint8_t* data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};

#endif