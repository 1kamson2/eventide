#include "Renderer/Renderer.hpp"

#include <memory>
#include <vector>

Renderer::Renderer() {}

void Renderer::TraverseChunks(std::vector<Chunk>& chunks_to_render) {
  float max_y = 2.0f;
  float y_start = 0;
  for (Chunk& chunk : chunks_to_render) {
    std::shared_ptr<Voxel> root =
        chunk.root_voxel->FindVoxelOnY(y_start, chunk.root_voxel);
    for (float y = 1.0f; y <= max_y; ++y) {
      RenderVoxels(root, y);
      root = root->FindVoxelOnY(y, root);
    }
  }
}

void Renderer::RenderVoxels(const std::shared_ptr<Voxel>& voxel, float y) {
  if (!voxel) return;

  if (voxel->GetPosition().y == y && !voxel->IsBlank()) {
    DrawCube(voxel->GetPosition(), 1.0f, 1.0f, 1.0f, voxel->color);
  }

  if (y <= voxel->GetPosition().y) {
    RenderVoxels(voxel->left, y);
  }
  if (y >= voxel->GetPosition().y) {
    RenderVoxels(voxel->right, y);
  }
}
