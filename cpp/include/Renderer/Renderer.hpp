#pragma once
#include <memory>
#include <vector>

#include "Chunk/Chunk.hpp"
class Renderer {
 public:
  std::shared_ptr<Voxel> best_cand_voxel = nullptr;
  float best_distance_voxel = 0.0f;
  int rendered_voxels = 0;

  Renderer();

  void TraverseChunks(std::vector<Chunk>& chunks_to_render);
  void RenderVoxels(std::shared_ptr<Voxel>& voxel);
};
