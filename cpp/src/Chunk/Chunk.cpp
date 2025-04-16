#include "Chunk/Chunk.hpp"

#include <cmath>
#include <cstddef>

using namespace chunk;
Chunk::Chunk(const Vector3& mid_point) : mid_point(mid_point) {
  std::cout << "[INFO] Chunk was created" << std::endl;
}

void Chunk::Update(std::shared_ptr<Voxel>& voxel) {
  root_voxel = Insert(root_voxel, voxel, 0);
}

std::shared_ptr<Voxel> Chunk::Insert(std::shared_ptr<Voxel>& last_free_voxel,
                                     std::shared_ptr<Voxel>& new_voxel,
                                     int depth) {
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
  if (!InAxisView(vec_pos.x, self_midpoint.x, max_allowed_dist)) {
    return false;
  }
  if (!InAxisView(vec_pos.y, self_midpoint.y, max_allowed_dist)) {
    return false;
  }
  if (!InAxisView(vec_pos.z, self_midpoint.z, max_allowed_dist)) {
    return false;
  }

  float dist = GetDistanceSquared(vec_pos, self_midpoint);
  return dist < (max_allowed_dist + LENGTH) * (max_allowed_dist + LENGTH);
}

bool Chunk::InAxisView(const float& first_scalar_point_value,
                       const float& second_scalar_point_value,
                       const float& max_allowed_dist) const {
  /*
   * Parameters:
   *  first_scalar_point_value: The value of point on given axis.
   *  second_scalar_point_value: The value of point on given axis.
   *  max_allowed_dist: The maximum distance allowed.
   * Function:
   *  Check if the distance in straight line doesn't exceed the max_allowed_dist
   *
   * */
  return fabs(first_scalar_point_value - second_scalar_point_value) <=
         max_allowed_dist + LENGTH;
}
float Chunk::GetDistanceSquared(Vector3& vec1, Vector3& vec2) {
  float x_dist = vec1.x - vec2.x;
  float y_dist = vec1.y - vec2.y;
  float z_dist = vec1.z - vec2.z;
  return x_dist * x_dist + y_dist * y_dist + z_dist * z_dist;
}

float Chunk::GetDistanceSquared(const std::shared_ptr<Voxel>& vox1,
                                const std::shared_ptr<Voxel>& vox2) {
  Vector3 vox1_pos = vox1->GetPosition();
  Vector3 vox2_pos = vox2->GetPosition();
  float x_dist = vox1_pos.x - vox2_pos.x;
  float y_dist = vox1_pos.y - vox2_pos.y;
  float z_dist = vox1_pos.z - vox2_pos.z;
  return x_dist * x_dist + y_dist * y_dist + z_dist * z_dist;
}

bool operator==(const Voxel& lhs, const Voxel& rhs) {
  Vector3 lhs_pos = lhs.GetPosition();
  Vector3 rhs_pos = rhs.GetPosition();
  return lhs_pos.x == rhs_pos.x && lhs_pos.y == rhs_pos.y &&
         lhs_pos.z == rhs_pos.z;
}
void Chunk::LoadVoxelsLTY(float& max_y, std::shared_ptr<Voxel>& curr_voxel) {
  // TODO: We shouldn't clear all buffer and then load again.
  if (curr_voxel == nullptr) {
    return;
  }

  // TODO: Should be able to exit faster
  if (curr_voxel->GetPosition().y >= -max_y) {
    Update(curr_voxel);
  }

  LoadVoxelsLTY(max_y, curr_voxel->left);
  LoadVoxelsLTY(max_y, curr_voxel->right);
}

bool Chunk::IsVoxelInChunk(const std::shared_ptr<Voxel>& voxel) {
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
