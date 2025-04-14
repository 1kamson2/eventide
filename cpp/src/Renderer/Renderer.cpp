#include "Renderer/Renderer.hpp"

#include <memory>
#include <vector>

Renderer::Renderer() {}

void Renderer::TraverseChunks(std::vector<Chunk>& chunks_to_render) {
  float max_y = 2.0f;
  for (Chunk& chunk : chunks_to_render) {
    RenderVoxels(chunk.root_voxel);
  }
}

void Renderer::RenderVoxels(std::shared_ptr<Voxel>& voxel) {
  if (voxel == nullptr) {
    return;
  }
  if (!voxel->IsBlank()) {
    DrawCube(voxel->GetPosition(), 1.0f, 1.0f, 1.0f, voxel->color);
  }

  RenderVoxels(voxel->left);
  RenderVoxels(voxel->right);
}
