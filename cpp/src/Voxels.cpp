#include "Voxels.hpp"

ObjectData::ObjectData(BLOCK_ID block, AGENT_ID agent, int color)
    : block(block), agent(agent), color(color) {
  std::cout << "[ObjectData] Class created." << std::endl;
}

template <typename Data>
Voxel<Data>::Voxel(Data data) : data(data) {
  std::cout << "[Voxel] Class created." << std::endl;
}

template <typename Data>
float Voxel<Data>::GetDimValue(const int& what_dim) {
  float dim_value = 0;
  switch (what_dim) {
    case 0:
      dim_value = this->data.position.x;
      break;
    case 1:
      dim_value = this->data.position.y;
      break;
    case 2:
      dim_value = this->data.position.z;
      break;
    default:
      std::cerr << "[ERROR] User has given " << what_dim << " as a parameter."
                << std::endl;
      throw std::invalid_argument("[ERROR] This dimension doesn't exist");
      break;
  }
  return dim_value;
}
