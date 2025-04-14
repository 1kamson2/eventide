#include "Chunk/Chunk.hpp"

#include <cstddef>

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

bool Chunk::InView(Vector3& agent_pos, const float& render_dist) {
  Vector3 self_pos = GetMidPoint();
  float dist = GetDistanceSquared(agent_pos, self_pos);
  return dist < render_dist;
}

float Chunk::GetDistanceSquared(Vector3& vec1, Vector3& vec2) {
  float dist_x = vec1.x - vec2.x;
  float dist_y = vec1.y - vec2.y;
  float dist_z = vec1.z - vec2.z;
  return dist_x * dist_x + dist_y * dist_y + dist_z * dist_z;
}

float Chunk::GetDistanceSquared(const std::shared_ptr<Voxel>& vox1,
                                const std::shared_ptr<Voxel>& vox2) {
  Vector3 vox1_pos = vox1->GetPosition();
  Vector3 vox2_pos = vox2->GetPosition();
  float dist_x = vox1_pos.x - vox2_pos.x;
  float dist_y = vox1_pos.y - vox2_pos.y;
  float dist_z = vox1_pos.z - vox2_pos.z;
  return dist_x * dist_x + dist_y * dist_y + dist_z * dist_z;
}

bool operator==(const Voxel& lhs, const Voxel& rhs) {
  Vector3 lhs_pos = lhs.GetPosition();
  Vector3 rhs_pos = rhs.GetPosition();
  return lhs_pos.x == rhs_pos.x && lhs_pos.y == rhs_pos.y &&
         lhs_pos.z == rhs_pos.z;
}
void Chunk::LoadVoxelsToY(float& max_y,
                          std::vector<std::shared_ptr<Voxel>>& voxels_to_render,
                          std::shared_ptr<Voxel>& curr_voxel) {
  // TODO: We shouldn't clear all buffer and then load again.
  if (curr_voxel == nullptr) {
    return;
  }

  // TODO: Should be able to exit faster
  if (curr_voxel->GetPosition().y >= -max_y) {
    voxels_to_render.push_back(curr_voxel);
  } else {
    // TODO: Don't know if this works okay?
    return;
  }

  LoadVoxelsToY(max_y, voxels_to_render, curr_voxel->left);
  LoadVoxelsToY(max_y, voxels_to_render, curr_voxel->right);
}
