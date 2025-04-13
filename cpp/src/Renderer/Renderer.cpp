#include "Renderer/Renderer.hpp"

#include <memory>
#include <vector>

Renderer::Renderer() {}

void Renderer::TraverseChunks(std::vector<Chunk>& chunks_to_render) {
  for (Chunk chunk : chunks_to_render) {
    RenderVoxels(chunk.root_voxel);
  }
}

void Renderer::RenderVoxels(std::shared_ptr<Voxel>& voxel_to_render) {
  // TODO: TRAVERSING FOR NOW HERE
  if (voxel_to_render == nullptr) {
    return;
  }
  if (!voxel_to_render->IsBlank()) {
    DrawCube(voxel_to_render->GetPosition(), 1, 1, 1, GREEN);
  }
  RenderVoxels(voxel_to_render->left);
  RenderVoxels(voxel_to_render->right);
}
