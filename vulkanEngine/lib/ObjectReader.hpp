#pragma once
#include "Util.hpp"
#include "ECS_lib.hpp"



class ObjectReader{
public:
    ObjectReader(ComponentList* list);
    ~ObjectReader();


    void readDataOBJ(Entity e, const char* path);
    



private:
    ComponentList* list;

};
