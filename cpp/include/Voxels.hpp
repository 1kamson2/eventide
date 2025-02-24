#pragma once
#include "Globals.hpp"
/* EnvironmentObject - now ObjectData */
class ObjectData {
 public:
  BLOCK_ID block;
  AGENT_ID agent;
  int color;
  ObjectData(BLOCK_ID block, AGENT_ID agent, int color);
};

template <typename Data>
class Voxel {
  Data data;
  std::shared_ptr<Voxel> left = nullptr;
  std::shared_ptr<Voxel> right = nullptr;
  Voxel(Data data);
  float GetDimValue(const int& what_dim);
};
