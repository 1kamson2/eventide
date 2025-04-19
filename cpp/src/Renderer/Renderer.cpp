#include "Renderer/Renderer.hpp"

#include <cmath>
#include <memory>
#include <vector>

#include "Renderer/Agent.hpp"
#include "Utils/VecOperations.hpp"

using namespace chunk;
using namespace agent;
using namespace vec_operations;
using namespace renderer;
Renderer::Renderer(Agent& agt) : agt(agt), frustum() {}

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
  Vector3 pos = voxel->GetPosition();

  if (!voxel->IsBlank()) {
    frustum.Update(agt);
    if (InFrustum(frustum, voxel)) {
      DrawCube(voxel->GetPosition(), 1.0f, 1.0f, 1.0f, voxel->color);
    }
  }

  RenderVoxels(voxel->left);
  RenderVoxels(voxel->right);
}

Frustum::Frustum()
    : half_vert_side(HALF_VERT_SIDE),
      half_horiz_side(HALF_HORIZ_SIDE),
      z_far(Z_FAR),
      z_near(Z_NEAR) {}

void Frustum::Update(const Agent& agt) {
  Vector3 front_mul_far = z_far * agt.front;

  near_face = Plane(agt.position + z_near * agt.front, agt.front);
  far_face = {agt.position + front_mul_far, -agt.front};
  right_face = {
      agt.position,
      Vector3CrossProduct(front_mul_far - agt.right * half_horiz_side, agt.up)};
  left_face = {
      agt.position,
      Vector3CrossProduct(agt.up, front_mul_far + agt.right * half_horiz_side)};
  top_face = {
      agt.position,
      Vector3CrossProduct(agt.right, front_mul_far - agt.up * half_vert_side)};
  bottom_face = {
      agt.position,
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
  return GetDistanceToPoint(voxel->center) > -LENGTH / 2;
}
