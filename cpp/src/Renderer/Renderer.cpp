#include "Renderer/Renderer.hpp"

#include <raylib.h>

#include <cmath>
#include <memory>
#include <vector>

#include "Chunk/Chunk.hpp"
#include "Environment/Environment.hpp"
#include "Renderer/Agent.hpp"
#include "Utils/VecOperations.hpp"

using namespace chunk;
using namespace agent;
using namespace vec_operations;
using namespace renderer;
Renderer::Renderer(Agent& agt) : agt(agt), frustum() {}

void Renderer::TraverseChunks(std::vector<Chunk>& chunks_to_render) {
  frustum.Update(agt);
  RenderAgent();
  size_t index = 0;
  for (Chunk& chunk : chunks_to_render) {
    if (chunk.IsVisible()) {
      RenderVoxels(chunk.root_voxel, index);
    }
  }
}

void Renderer::RenderAgent() {
  DrawCube(agt.GetPosition(), 1.0f, 2.0f, 1.0f, RED);
}

void Renderer::RenderVoxels(std::shared_ptr<Voxel>& voxel, size_t& index) {
  if (voxel == nullptr || index == 100) {
    return;
  }

  // if (!voxel->IsBlank() && voxel->GetPosition().y > agt.GetPosition().y - 6)
  // {
  if (!voxel->IsBlank()) {
    // if (InFrustum(frustum, voxel)) {
    Vector3 pos = voxel->GetPosition();
    DrawCube(voxel->GetPosition(), 1.0f, 1.0f, 1.0f, voxel->color);
    // }
  }

  RenderVoxels(voxel->left, index);
  RenderVoxels(voxel->right, index);
}

Frustum::Frustum()
    : half_vert_side(HALF_VERT_SIDE),
      half_horiz_side(HALF_HORIZ_SIDE),
      z_far(Z_FAR),
      z_near(Z_NEAR) {}

void Frustum::Update(const Agent& agt) {
  Vector3 front_mul_far = z_far * agt.front;

  near_face = Plane(agt.GetPosition() + z_near * agt.front, agt.front);
  far_face = {agt.GetPosition() + front_mul_far, -agt.front};
  right_face = {
      agt.GetPosition(),
      Vector3CrossProduct(front_mul_far - agt.right * half_horiz_side, agt.up)};
  left_face = {
      agt.GetPosition(),
      Vector3CrossProduct(agt.up, front_mul_far + agt.right * half_horiz_side)};
  top_face = {
      agt.GetPosition(),
      Vector3CrossProduct(agt.right, front_mul_far - agt.up * half_vert_side)};
  bottom_face = {
      agt.GetPosition(),
      Vector3CrossProduct(front_mul_far + agt.up * half_vert_side, agt.right)};
}

Plane::Plane() {};

Plane::Plane(Vector3 normal, Vector3 dist_vec) : normal(normal) {
  Normalize(dist_vec);
  dist = VectorLength(dist_vec);
}

float Plane::GetDistanceToPoint(const Vector3& point) const {
  return Vector3DotProduct(normal, point) - dist;
}

bool Renderer::InFrustum(const Frustum& cfrustum,
                         std::shared_ptr<Voxel> voxel) const {
  return (cfrustum.left_face.IsOnPlane(voxel) &&
          cfrustum.right_face.IsOnPlane(voxel) &&
          cfrustum.top_face.IsOnPlane(voxel) &&
          cfrustum.bottom_face.IsOnPlane(voxel) &&
          cfrustum.near_face.IsOnPlane(voxel) &&
          cfrustum.far_face.IsOnPlane(voxel));
}

bool Plane::IsOnPlane(const std::shared_ptr<Voxel>& voxel) const {
  const float r = voxel->extents.x * std::abs(normal.x) +
                  voxel->extents.y * std::abs(normal.y) +
                  voxel->extents.z * std::abs(normal.z);
  return -r <= GetDistanceToPoint(voxel->center);
}
