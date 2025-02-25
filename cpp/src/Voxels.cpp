#include "Voxels.hpp"

ObjectData::ObjectData(BLOCK_ID block, AGENT_ID agent, const uint& color)
    : block(block), agent(agent), color(color) {}

template <typename Data>
Voxel<Data>::Voxel(Data data, int& x, int& y, int& z)
    : data(data),
      x_axis(static_cast<int_fast8_t>(x)),
      y_axis(static_cast<int_fast8_t>(y)),
      z_axis(static_cast<int_fast8_t>(z)) {
  std::cout << "[Voxel] Initialized the voxel. " << std::endl;
}
