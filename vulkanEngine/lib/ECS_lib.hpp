#pragma once

#include "Util.hpp"

using Entity = uint32_t;
using Signature = std::bitset<64>;


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










template<typename T>
class ComponentArray{

public:

    ComponentArray();
    ~ComponentArray();

    void setComponent(Entity e, T component);
    uint32_t addComponent(Entity e);

    Entity deleteComponent(uint32_t index);

    T getComponent(uint32_t index);


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


struct ComponentList{
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
};


template<typename T>
static inline void deleteComponent(ComponentArray<T>& array, boost::unordered_flat_map<uint32_t, uint32_t>& map, Entity e){
    map[array.deleteComponent(map[e])] = map[e];
}


template<typename T>
static inline void addComponent(ComponentArray<T>& array, boost::unordered_flat_map<uint32_t, uint32_t>& map, Entity e){
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
