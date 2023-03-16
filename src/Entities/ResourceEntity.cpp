#include "Entity.h"
#include "../Resource.h"
#include "ResourceEntity.h"
#include<utility>

ResourceEntity::ResourceEntity(Resource res, std::pair<double,double> pos) : resource(res){

}
void ResourceEntity::destroy() {
}

