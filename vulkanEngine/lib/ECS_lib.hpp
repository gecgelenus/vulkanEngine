#pragma once

#include "Util.hpp"

#define MAX_ENTITY_COUNT 65536
#define MAX_COMPONENT_TYPE_COUNT 64

using Entity = uint32_t;
using Signature = std::bitset<MAX_COMPONENT_TYPE_COUNT>;


using Position2F = glm::vec2;
using Position3F = glm::vec3;

using Color3F = glm::vec3;
using Color4F = glm::vec4;

using TextureID = glm::uvec1;

using ModelMatrix = glm::mat4;

using Rotation = glm::vec3;
using Scale = glm::vec1; // I chose to not support multi-dimentional scale, because it requires heavier calculation on shaders for lighting

using Visible = glm::bvec1;

using Name = std::string; // Since strings are on heap, not efficent as it sounds

using VirtualMemoryOffset = VkDeviceSize; // Vertex and Index offset datas should be adjacent, because usually they are used together

using MemoryOffset = uint32_t;

using VirtualAllocation = VmaVirtualAllocation;

using DrawCommand = VkDrawIndexedIndirectCommand;

using ObjectProperty = objectProperties;

using VertexData3F = std::vector<Vertex>*;
#define NEW_VERTEX_DATA_3F new std::vector<Vertex>()

using VertexData2F = std::vector<Vertex2D>*;
#define NEW_VERTEX_DATA_2F new std::vector<Vertex2D>()

using IndexData = std::vector<uint32_t>*;
#define NEW_INDEX_DATA new std::vector<uint32_t>()


using MaterialData = std::vector<Material>*;
#define NEW_MATERIAL_DATA new std::vector<Material>()

using ID = uint32_t;



#define COMPONENT_POSITION_2F 0
#define COMPONENT_POSITION_3F 1
#define COMPONENT_COLOR_3F 2
#define COMPONENT_COLOR_4F 3
#define COMPONENT_TEXTURE_ID 4
#define COMPONENT_MODEL_MATRIX 5
#define COMPONENT_ROTATION 6
#define COMPONENT_SCALE 7
#define COMPONENT_VISIBLE 8
#define COMPONENT_NAME 9
#define COMPONENT_VIRTUAL_MEMORY_OFFSET_VERTEX 10
#define COMPONENT_VIRTUAL_MEMORY_OFFSET_INDEX 11
#define COMPONENT_MEMORY_OFFSET_VERTEX 12
#define COMPONENT_MEMORY_OFFSET_INDEX 13
#define COMPONENT_VIRTUAL_ALLOCATION_VERTEX 14
#define COMPONENT_VIRTUAL_ALLOCATION_INDEX 15
#define COMPONENT_DRAW_COMMAND 16
#define COMPONENT_OBJECT_PROPERTY 17
#define COMPONENT_VERTEX_DATA_3F 18
#define COMPONENT_VERTEX_DATA_2F 19
#define COMPONENT_INDEX_DATA 20
#define COMPONENT_MATERIAL_DATA 21
#define COMPONENT_MEMORY_OFFSET_MATERIAL 22
#define COMPONENT_MODEL_ID 23




const static std::bitset<64> MASK_POSITION_2F(1LL << (0LL));
const static std::bitset<64> MASK_POSITION_3F(1LL << (1LL));
const static std::bitset<64> MASK_COLOR_3F(1LL << (2LL));
const static std::bitset<64> MASK_COLOR_4F(1LL << (3LL));
const static std::bitset<64> MASK_TEXTURE_ID(1LL << (4LL));
const static std::bitset<64> MASK_MODEL_MATRIX(1LL << (5LL));
const static std::bitset<64> MASK_ROTATION(1LL << (6LL));
const static std::bitset<64> MASK_SCALE(1LL << (7LL));
const static std::bitset<64> MASK_VISIBLE(1LL << (8LL));
const static std::bitset<64> MASK_NAME(1LL << (9LL));
const static std::bitset<64> MASK_VIRTUAL_MEMORY_OFFSET_VERTEX(1LL << (10LL));
const static std::bitset<64> MASK_VIRTUAL_MEMORY_OFFSET_INDEX(1LL << (11LL));
const static std::bitset<64> MASK_MEMORY_OFFSET_VERTEX(1LL << (12LL));
const static std::bitset<64> MASK_MEMORY_OFFSET_INDEX(1LL << (13LL));
const static std::bitset<64> MASK_VIRTUAL_ALLOCATION_VERTEX(1LL << (14LL));
const static std::bitset<64> MASK_VIRTUAL_ALLOCATION_INDEX(1LL << (15LL));
const static std::bitset<64> MASK_DRAW_COMMAND(1LL << (16LL));
const static std::bitset<64> MASK_OBJECT_PROPERTY(1LL << (17LL));
const static std::bitset<64> MASK_VERTEX_DATA_3F(1LL << (18LL));
const static std::bitset<64> MASK_VERTEX_DATA_2F(1LL << (19LL));
const static std::bitset<64> MASK_INDEX_DATA(1LL << (20LL));
const static std::bitset<64> MASK_MATERIAL_DATA(1LL << (21LL));
const static std::bitset<64> MASK_MEMORY_OFFSET_MATERIAL(1LL << (22LL));
const static std::bitset<64> MASK_MODEL_ID(1LL << (23LL));

// HOLY, what a mess!   (ง •̀_•́)ง
#define OBJECT3D_MASK (MASK_POSITION_3F|MASK_COLOR_3F|MASK_VERTEX_DATA_3F \
|MASK_INDEX_DATA|MASK_MATERIAL_DATA|MASK_MEMORY_OFFSET_INDEX|MASK_MEMORY_OFFSET_VERTEX \
|MASK_MEMORY_OFFSET_MATERIAL|MASK_VIRTUAL_MEMORY_OFFSET_VERTEX|MASK_VIRTUAL_MEMORY_OFFSET_INDEX \
|MASK_VIRTUAL_ALLOCATION_VERTEX|MASK_VIRTUAL_ALLOCATION_INDEX|MASK_DRAW_COMMAND|MASK_SCALE \
|MASK_ROTATION|MASK_MODEL_MATRIX|MASK_OBJECT_PROPERTY|MASK_MODEL_ID|MASK_NAME)





template<typename T>
class ComponentArray{

public:

    ComponentArray();
    ~ComponentArray();

    void setComponent(Entity e, T component);
    uint32_t addComponent(Entity e);

    Entity deleteComponent(uint32_t index);

    T getComponent(uint32_t index);
    T* getComponentP(uint32_t index);

    uint8_t signatureIndex;
    std::vector<T> components;
    std::vector<Entity> entityPair;

};


template<typename T>
inline ComponentArray<T>::ComponentArray(){
    std::cout << "Component array created: " << typeid(T).name() << std::endl;
}


template<typename T>
ComponentArray<T>::~ComponentArray(){

}

template<typename T>
void ComponentArray<T>::setComponent(uint32_t index, T component){
    this->components[index] = component;
}

template<typename T>
uint32_t ComponentArray<T>::addComponent(Entity e){
    T t{};
    this->entityPair.push_back(e);
    this->components.push_back(t);

    return this->components.size()-1;
}

template<typename T>
Entity ComponentArray<T>::deleteComponent(uint32_t index){

    std::iter_swap(components.begin() + index, components.end()-1);
    components.pop_back();

    std::iter_swap(entityPair.begin() + index, entityPair.end()-1);
    entityPair.pop_back();


    return entityPair[index];
}


template<typename T>
T ComponentArray<T>::getComponent(uint32_t index){
    return components[index];
}
template<typename T>
T* ComponentArray<T>::getComponentP(uint32_t index){
    return &(components[index]);
}



struct ComponentList{
    std::queue<Entity> entityQueue;
    boost::unordered_flat_map<Entity, Signature> entities;
    
// COMPONENT_POSITION_2F 0
    ComponentArray<Position2F> comp_position_2f;
    boost::unordered_flat_map<uint32_t, uint32_t> map_position_2f;
// COMPONENT_POSITION_3F 1
    ComponentArray<Position3F> comp_position_3f;
    boost::unordered_flat_map<uint32_t, uint32_t> map_position_3f;
// COMPONENT_COLOR_3F 2
    ComponentArray<Color3F> comp_color_3f;
    boost::unordered_flat_map<uint32_t, uint32_t> map_color_3f;
// COMPONENT_COLOR_4F 3
    ComponentArray<Color4F> comp_color_4f;
    boost::unordered_flat_map<uint32_t, uint32_t> map_color_4f;
// COMPONENT_TEXTURE_ID 4
    ComponentArray<TextureID> comp_texture_id;
    boost::unordered_flat_map<uint32_t, uint32_t> map_texture_id;
// COMPONENT_MODEL_MATRIX 5
    ComponentArray<ModelMatrix> comp_model_matrix;
    boost::unordered_flat_map<uint32_t, uint32_t> map_model_matrix;
// COMPONENT_ROTATION 6
    ComponentArray<Rotation> comp_rotation;
    boost::unordered_flat_map<uint32_t, uint32_t> map_rotation;
// COMPONENT_SCALE 7
    ComponentArray<Scale> comp_scale;
    boost::unordered_flat_map<uint32_t, uint32_t> map_scale;
// COMPONENT_VISIBLE 8
    ComponentArray<Visible> comp_visible;
    boost::unordered_flat_map<uint32_t, uint32_t> map_visible;
// COMPONENT_NAME 9
    ComponentArray<Name> comp_name;
    boost::unordered_flat_map<uint32_t, uint32_t> map_name;
// COMPONENT_VIRTUAL_MEMORY_OFFSET_VERTEX 10
    ComponentArray<VirtualMemoryOffset> comp_virtual_mem_offset_vertex;
    boost::unordered_flat_map<uint32_t, uint32_t> map_virtual_mem_offset_vertex;
// COMPONENT_VIRTUAL_MEMORY_OFFSET_INDEX 11
    ComponentArray<VirtualMemoryOffset> comp_virtual_mem_offset_index;
    boost::unordered_flat_map<uint32_t, uint32_t> map_virtual_mem_offset_index;
// COMPONENT_MEMORY_OFFSET_VERTEX 12
    ComponentArray<MemoryOffset> comp_mem_offset_vertex;
    boost::unordered_flat_map<uint32_t, uint32_t> map_mem_offset_vertex;
// COMPONENT_MEMORY_OFFSET_INDEX 13
    ComponentArray<MemoryOffset> comp_mem_offset_index;
    boost::unordered_flat_map<uint32_t, uint32_t> map_mem_offset_index;
// COMPONENT_VIRTUAL_ALLOCATION_VERTEX 14
    ComponentArray<VirtualAllocation> comp_virtual_alloc_vertex;
    boost::unordered_flat_map<uint32_t, uint32_t> map_virtual_alloc_vertex;
// COMPONENT_VIRTUAL_ALLOCATION_INDEX 15
    ComponentArray<VirtualAllocation> comp_virtual_alloc_index;
    boost::unordered_flat_map<uint32_t, uint32_t> map_virtual_alloc_index;
// COMPONENT_DRAW_COMMAND 16
    ComponentArray<DrawCommand> comp_draw_command;
    boost::unordered_flat_map<uint32_t, uint32_t> map_draw_command;
// COMPONENT_OBJECT_PROPERTY 17
    ComponentArray<ObjectProperty> comp_object_property;
    boost::unordered_flat_map<uint32_t, uint32_t> map_object_property;
// COMPONENT_VERTEX_DATA_3F 18
    ComponentArray<VertexData3F> comp_vertices_3f;
    boost::unordered_flat_map<uint32_t, uint32_t> map_vertices_3f;
// COMPONENT_VERTEX_DATA_2F 19
    ComponentArray<VertexData2F> comp_vertices_2f;
    boost::unordered_flat_map<uint32_t, uint32_t> map_vertices_2f;
// COMPONENT_INDEX_DATA 20
    ComponentArray<IndexData> comp_indices;
    boost::unordered_flat_map<uint32_t, uint32_t> map_indices;
// COMPONENT_MATERIAL_DATA 21
    ComponentArray<MaterialData> comp_materials;
    boost::unordered_flat_map<uint32_t, uint32_t> map_materials;
// COMPONENT_MEMORY_OFFSET_MATERIAL 22
    ComponentArray<MemoryOffset> comp_mem_offset_material;
    boost::unordered_flat_map<uint32_t, uint32_t> map_mem_offset_material;
// COMPONENT_MODEL_ID 23
    ComponentArray<ID> comp_model_id;
    boost::unordered_flat_map<uint32_t, uint32_t> map_model_id;
};


template<typename T>
static inline void deleteComponent(ComponentList* list, ComponentArray<T>& array, boost::unordered_flat_map<uint32_t, uint32_t>& map, Entity e){
    list->entities[e].reset(array.signatureIndex);
    map[array.deleteComponent(map[e])] = map[e];
}


template<typename T>
static inline void addComponent(ComponentList* list, ComponentArray<T>& array, boost::unordered_flat_map<uint32_t, uint32_t>& map, Entity e){
    list->entities[e].set(array.signatureIndex);
    map.insert(std::pair<uint32_t, uint32_t>(e, array.addComponent(e)));
}

template<typename T>
static inline void setComponent(ComponentArray<T>& array, boost::unordered_flat_map<uint32_t, uint32_t>& map, Entity e, T component){

    array.setComponent(map[e], component);
}

template<typename T>
static inline T getComponent(ComponentArray<T>& array, boost::unordered_flat_map<uint32_t, uint32_t>& map, Entity e){
    return array.getComponent(map[e]);
}


template<typename T>
static inline T* getComponentP(ComponentArray<T>& array, boost::unordered_flat_map<uint32_t, uint32_t>& map, Entity e){
    return array.getComponentP(map[e]);
}

static inline Entity createEntity(ComponentList* list){
    Entity e = list->entityQueue.front();
    list->entityQueue.pop();
    list->entities.insert(std::pair<Entity, Signature>(e, {}));
    return e;
}

static inline Entity createEntity(ComponentList* list, Signature signature){
    Entity e = list->entityQueue.front();
    list->entityQueue.pop();
    list->entities.insert(std::pair<Entity, Signature>(e, {}));

    if(signature.test(COMPONENT_POSITION_2F)){
        addComponent(list, list->comp_position_2f, list->map_position_2f, e);
        setComponent(list->comp_position_2f, list->map_position_2f, e, {});
    }
    if(signature.test(COMPONENT_POSITION_3F)){
        addComponent(list, list->comp_position_3f, list->map_position_3f, e);
        setComponent(list->comp_position_3f, list->map_position_3f, e, {});
    }
    if(signature.test(COMPONENT_COLOR_3F)){
        addComponent(list, list->comp_color_3f, list->map_color_3f, e);
        setComponent(list->comp_color_3f, list->map_color_3f, e, glm::vec3(1.0f));
    }
    if(signature.test(COMPONENT_COLOR_4F)){
        addComponent(list, list->comp_color_4f, list->map_color_4f, e);
        setComponent(list->comp_color_4f, list->map_color_4f, e, glm::vec4(1.0f));
    }
    if(signature.test(COMPONENT_TEXTURE_ID)){
        addComponent(list, list->comp_texture_id, list->map_texture_id, e);
        setComponent(list->comp_texture_id, list->map_texture_id, e, {});
    }
    if(signature.test(COMPONENT_MODEL_MATRIX)){
        addComponent(list, list->comp_model_matrix, list->map_model_matrix, e);
        setComponent(list->comp_model_matrix, list->map_model_matrix, e, glm::mat4(1.0f));
    }
    if(signature.test(COMPONENT_ROTATION)){
        addComponent(list, list->comp_rotation, list->map_rotation, e);
        setComponent(list->comp_rotation, list->map_rotation, e, {});
    }
    if(signature.test(COMPONENT_SCALE)){
        addComponent(list, list->comp_scale, list->map_scale, e);
        setComponent(list->comp_scale, list->map_scale, e, {});
    }
    if(signature.test(COMPONENT_VISIBLE)){
        addComponent(list, list->comp_visible, list->map_visible, e);
        setComponent(list->comp_visible, list->map_visible, e, {});
    }
    if(signature.test(COMPONENT_NAME)){
        addComponent(list, list->comp_name, list->map_name, e);
        setComponent(list->comp_name, list->map_name, e, {});
    }
    if(signature.test(COMPONENT_VIRTUAL_MEMORY_OFFSET_VERTEX)){
        addComponent(list, list->comp_virtual_mem_offset_vertex, list->map_virtual_mem_offset_vertex, e);
        setComponent(list->comp_virtual_mem_offset_vertex, list->map_virtual_mem_offset_vertex, e, {});
    }
    if(signature.test(COMPONENT_VIRTUAL_MEMORY_OFFSET_INDEX)){
        addComponent(list, list->comp_virtual_mem_offset_index, list->map_virtual_mem_offset_index, e);
        setComponent(list->comp_virtual_mem_offset_index, list->map_virtual_mem_offset_index, e, {});
    }
    if(signature.test(COMPONENT_MEMORY_OFFSET_VERTEX)){
        addComponent(list, list->comp_mem_offset_vertex, list->map_mem_offset_vertex, e);
        setComponent(list->comp_mem_offset_vertex, list->map_mem_offset_vertex, e, {});
    }
    if(signature.test(COMPONENT_MEMORY_OFFSET_INDEX)){
        addComponent(list, list->comp_mem_offset_index, list->map_mem_offset_index, e);
        setComponent(list->comp_mem_offset_index, list->map_mem_offset_index, e, {});
    }
    if(signature.test(COMPONENT_VIRTUAL_ALLOCATION_VERTEX)){
        addComponent(list, list->comp_virtual_alloc_vertex, list->map_virtual_alloc_vertex, e);
        setComponent(list->comp_virtual_alloc_vertex, list->map_virtual_alloc_vertex, e, {});
    }
    if(signature.test(COMPONENT_VIRTUAL_ALLOCATION_INDEX)){
        addComponent(list, list->comp_virtual_alloc_index, list->map_virtual_alloc_index, e);
        setComponent(list->comp_virtual_alloc_index, list->map_virtual_alloc_index, e, {});
    }
    if(signature.test(COMPONENT_DRAW_COMMAND)){
        addComponent(list, list->comp_draw_command, list->map_draw_command, e);
        setComponent(list->comp_draw_command, list->map_draw_command, e, {});
    }
    if(signature.test(COMPONENT_OBJECT_PROPERTY)){
        addComponent(list, list->comp_object_property, list->map_object_property, e);
        setComponent(list->comp_object_property, list->map_object_property, e, {});
    }
    if(signature.test(COMPONENT_VERTEX_DATA_3F)){
        addComponent(list, list->comp_vertices_3f, list->map_vertices_3f, e);
        setComponent(list->comp_vertices_3f, list->map_vertices_3f, e, NEW_VERTEX_DATA_3F);
    }
    if(signature.test(COMPONENT_VERTEX_DATA_2F)){
        addComponent(list, list->comp_vertices_2f, list->map_vertices_2f, e);
        setComponent(list->comp_vertices_2f, list->map_vertices_2f, e, NEW_VERTEX_DATA_2F);
    }
    if(signature.test(COMPONENT_INDEX_DATA)){
        addComponent(list, list->comp_indices, list->map_indices, e);
        setComponent(list->comp_indices, list->map_indices, e, NEW_INDEX_DATA);
    }
    if(signature.test(COMPONENT_MATERIAL_DATA)){
        addComponent(list, list->comp_materials, list->map_materials, e);
        setComponent(list->comp_materials, list->map_materials, e, NEW_MATERIAL_DATA);
    }
    if(signature.test(COMPONENT_MEMORY_OFFSET_MATERIAL)){
        addComponent(list, list->comp_mem_offset_material, list->map_mem_offset_material, e);
        setComponent(list->comp_mem_offset_material, list->map_mem_offset_material, e, {});
    }
    if(signature.test(COMPONENT_MODEL_ID)){
        addComponent(list, list->comp_model_id, list->map_model_id, e);
        setComponent(list->comp_model_id, list->map_model_id, e, {});
    }
   


    return e;
}

static inline void deleteEntity(ComponentList* list, Entity e){
    list->entityQueue.push(e);
    list->entities.erase(e);
}