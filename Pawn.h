class Pawn : public Entity{
    Building* destination;
    virtual void moveTo(Building* destination);
    virtual void getResource(Resource* toGet);
};