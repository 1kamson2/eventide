#pragma once
#include <cstdint>
#include <memory>
#include <vector>

#include "Globals.hpp"
using namespace engvars;
class ObjectData {
 public:
  BLOCK_ID block;
  AGENT_ID agent;
  /* R G B Alpha */
  uint color;
  ObjectData(BLOCK_ID block, AGENT_ID agent, const uint& color);
};

template <typename Data>
class Voxel {
  /* Implementation of KdTree Node */
  Data data;
  int_fast8_t x_axis{0}, y_axis{0}, z_axis{0};
  std::unique_ptr<Voxel<Data>> left;
  std::unique_ptr<Voxel<Data>> right;
  Voxel(Data data, int& x, int& y, int& z);
};
