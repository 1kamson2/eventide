#include "Renderer/Renderer.hpp"

#include <memory>
#include <vector>

Renderer::Renderer() {}

// void Renderer::TraverseChunks(
//     std::vector<Chunk>& chunks_to_render,
//     std::vector<std::shared_ptr<Voxel>>& voxels_to_render) {
//   float max_y = 2.0f;
//   for (Chunk& chunk : chunks_to_render) {
//   }
// }

void Renderer::RenderVoxels(
    const std::vector<std::shared_ptr<Voxel>>& voxel_to_render) {
  for (std::shared_ptr<Voxel> voxel : voxel_to_render) {
    if (!voxel->IsBlank()) {
      DrawCube(voxel->GetPosition(), 1.0f, 1.0f, 1.0f, voxel->color);
    }
  }
}
