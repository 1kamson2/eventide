#include "Chunk/Chunk.hpp"

#include <cmath>
#include <cstddef>

#include "Utils/Enums.hpp"

using namespace chunk;
using namespace chunk_enums;
Chunk::Chunk(const Vector3& mid_point) : mid_point(mid_point) {}

void Chunk::Update(VoxelRef voxel) {
  root_voxel = Insert(root_voxel, voxel, 0);
}

std::shared_ptr<Voxel> Chunk::Insert(VoxelRef last_free_voxel,
                                     VoxelRef new_voxel, int depth) {
  if (!last_free_voxel) {
    /* Free node has been found */
    return new_voxel;
  }
  int current_axis = depth % 3;
  float new_voxel_axis = new_voxel->GetAxis(current_axis);
  float voxel_axis = last_free_voxel->GetAxis(current_axis);

  if (new_voxel_axis < voxel_axis) {
    last_free_voxel->left = Insert(last_free_voxel->left, new_voxel, ++depth);
  } else {
    last_free_voxel->right = Insert(last_free_voxel->right, new_voxel, ++depth);
  }
  return last_free_voxel;
}

Vector3 Chunk::GetMidPoint() const { return mid_point; }

bool Chunk::InView(Vector3& vec_pos, const float& max_allowed_dist) {
  /*
   * Parameters:
   *   vec_pos: Vector that will be used to measure distance in respect to this
   *            vector.
   *   max_allowed_dist: Distance that will allow the chunks to be seen.
   *
   * Function:
   *   Check if the voxel is in agent's view.
   */
  Vector3 self_midpoint = GetMidPoint();

  float dist = GetDistanceSquared(vec_pos, self_midpoint);
  return dist < (max_allowed_dist + LENGTH) * (max_allowed_dist + LENGTH);
}

float Chunk::GetDistanceSquared(Vector3& vec1, Vector3& vec2) {
  float x_dist = vec1.x - vec2.x;
  float y_dist = vec1.y - vec2.y;
  float z_dist = vec1.z - vec2.z;
  return x_dist * x_dist + y_dist * y_dist + z_dist * z_dist;
}

float Chunk::GetDistanceSquared(VoxelCRef vox1, VoxelCRef vox2) {
  Vector3 vox1_pos = vox1->GetPosition();
  Vector3 vox2_pos = vox2->GetPosition();
  float x_dist = vox1_pos.x - vox2_pos.x;
  float y_dist = vox1_pos.y - vox2_pos.y;
  float z_dist = vox1_pos.z - vox2_pos.z;
  return x_dist * x_dist + y_dist * y_dist + z_dist * z_dist;
}

bool operator==(VoxelCRef lhs, VoxelCRef rhs) {
  Vector3 lhs_pos = lhs->GetPosition();
  Vector3 rhs_pos = rhs->GetPosition();
  return lhs_pos.x == rhs_pos.x && lhs_pos.y == rhs_pos.y &&
         lhs_pos.z == rhs_pos.z;
}
void Chunk::LoadVoxelsLTY(float& max_y, VoxelRef curr_voxel) {
  if (curr_voxel == nullptr) {
    return;
  }

  if (curr_voxel->GetPosition().y >= -max_y) {
    Update(curr_voxel);
  }

  LoadVoxelsLTY(max_y, curr_voxel->left);
  LoadVoxelsLTY(max_y, curr_voxel->right);
}

void Chunk::MarkAs(const Visibility& status) const {
  switch (status) {
    case Visibility::VISIBLE:
      visible = true;
      break;
    case Visibility::INVISIBLE:
      visible = false;
      break;
  }
}

bool Chunk::IsVisible() const { return visible; }

bool Chunk::IsVoxelInChunk(VoxelCRef voxel) {
  Vector3 vox_pos = voxel->GetPosition();
  Vector3 chunk_mid = GetMidPoint();

  return (vox_pos.x >= chunk_mid.x - SIZE / 2) &&
         (vox_pos.x <= chunk_mid.x + SIZE / 2) &&
         (vox_pos.y >= chunk_mid.y - SIZE / 2) &&
         (vox_pos.y <= chunk_mid.y + SIZE / 2) &&
         (vox_pos.z >= chunk_mid.z - SIZE / 2) &&
         (vox_pos.z <= chunk_mid.z + SIZE / 2);
}

bool Chunk::TheSameChunk(const Chunk& rhs) {
  Vector3 rhs_pos = rhs.GetMidPoint();
  Vector3 self_pos = GetMidPoint();
  return rhs_pos.x == self_pos.x && rhs_pos.y == self_pos.y &&
         rhs_pos.z == self_pos.z;
}
