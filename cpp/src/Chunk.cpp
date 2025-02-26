#include "Chunk.hpp"

#include <memory>
#include <stdexcept>

#include "Environment.hpp"
#include "ResourceManager.hpp"
Chunk::Chunk()
    : root(nullptr),
      best_voxel_cand(nullptr),
      best_distance_cand(0),
      rendered(0),
      voxels_to_render() {}

std::shared_ptr<Voxel> Chunk::InsertVoxel(
    std::shared_ptr<Voxel>& last_free_voxel, std::shared_ptr<Voxel>& new_voxel,
    int depth) {
  /*
   * This function traverses recursively through the KD-Tree, while meeting the
   * conditions. This also divides the entire tree into left and right side. If
   * we find the null value, we insert the Voxel, thus it is now ready to be
   * used.
   */
  if (!last_free_voxel) {
    /* We found a Node, that is free */
    return new_voxel;
  }

  int curr_dim = depth % 3;
  float new_voxel_dim_value = new_voxel->GetDimValue(curr_dim);
  float voxel_dim_value = last_free_voxel->GetDimValue(curr_dim);
  if (new_voxel_dim_value < voxel_dim_value) {
    last_free_voxel->left =
        InsertVoxel(last_free_voxel->left, new_voxel, depth + 1);
  } else {
    last_free_voxel->right =
        InsertVoxel(last_free_voxel->right, new_voxel, depth + 1);
  }

  return last_free_voxel;
}

bool Chunk::FindVoxel(const std::shared_ptr<Voxel>& current_voxel,
                      const std::shared_ptr<Voxel>& voxel_to_find,
                      int depth) const {
  /* This function returns if the voxels are the same */
  /* Probably should return the address where it is? */
  /* If not used, delete it */
  if (current_voxel == nullptr) return false;

  if (CompareVoxels(current_voxel->data.position,
                    voxel_to_find->data.position)) {
    return true;
  }

  int curr_dim = depth % 3;
  float voxel_dim_value = voxel_to_find->GetDimValue(curr_dim);
  float curr_voxel_dim_value = current_voxel->GetDimValue(curr_dim);
  if (voxel_dim_value < curr_voxel_dim_value) {
    return FindVoxel(current_voxel->left, voxel_to_find, depth + 1);
  } else {
    return FindVoxel(current_voxel->right, voxel_to_find, depth + 1);
  }
}

std::shared_ptr<Voxel> Chunk::FindNearestVoxel(
    const std::shared_ptr<Voxel> target) {
  if (!this->root) {
    return nullptr;
  }

  this->best_voxel_cand = nullptr;
  this->best_distance_cand = std::numeric_limits<double>::max();

  this->SearchForNearestVoxel(this->root, target->data.position, 0,
                              this->best_voxel_cand, this->best_distance_cand);
  return this->best_voxel_cand;
}

void Chunk::FindVoxelsInRange(const Vector3& position, const float& radius) {
  /* This is the main function of this KD-Tree class. Here will be returned the
   * nearest Voxels in given range */
  if (!root) {
    std::cerr << "[ERROR] You should initialize the tree. Call UpdateRoot(...) "
                 "before. "
              << std::endl;
    throw std::invalid_argument(
        "[ERROR] Halting the program, tree not initialized.");
    return;
  };
  /* TODO: There should be a faster / better way rather than clearing the vector
   * everytime? */
  this->voxels_to_render.clear();

  ObjectInfo pos_into_voxel =
      ObjectInfo(BLOCK_ID::NO, AGENT_ID::NO, EDGE_LENGTH, position, BLANK);
  std::shared_ptr<Voxel> voxel_node = std::make_shared<Voxel>(pos_into_voxel);
  LoadRenderBuffer(root, voxel_node, radius, 0);
  return;
}

void Chunk::SearchForNearestVoxel(std::shared_ptr<Voxel>& node,
                                  const Vector3& target_pos, int depth,
                                  std::shared_ptr<Voxel>& best_node,
                                  double& best_distance) {
  if (!node) return;

  float current_distance = GetDistance(node->data.position, target_pos);

  // Update best if current is closer
  if (current_distance < best_distance) {
    best_distance = current_distance;
    best_node = node;
  }

  // Get current dimension value
  int cd = depth % 3;
  float target_dim_value = -1;
  float node_dim_value = -1;

  switch (cd) {
    case 0:
      target_dim_value = target_pos.x;
      node_dim_value = node->data.position.x;
      break;
    case 1:
      target_dim_value = target_pos.y;
      node_dim_value = node->data.position.y;
      break;
    case 2:
      target_dim_value = target_pos.z;
      node_dim_value = node->data.position.z;
      break;
  }

  if (target_dim_value < node_dim_value) {
    SearchForNearestVoxel(node->left, target_pos, depth + 1, best_node,
                          best_distance);

    float axis_distance = std::abs(target_dim_value - node_dim_value);
    if (axis_distance * axis_distance < best_distance) {
      SearchForNearestVoxel(node->right, target_pos, depth + 1, best_node,
                            best_distance);
    }
  } else {
    SearchForNearestVoxel(node->right, target_pos, depth + 1, best_node,
                          best_distance);

    float axis_distance = std::abs(target_dim_value - node_dim_value);
    if (axis_distance * axis_distance < best_distance) {
      SearchForNearestVoxel(node->left, target_pos, depth + 1, best_node,
                            best_distance);
    }
  }
}

void Chunk::LoadRenderBuffer(std::shared_ptr<Voxel>& voxel,
                             std::shared_ptr<Voxel>& temp_voxel, float radius,
                             int depth) {
  if (!voxel) return;

  float dist = GetDistance(voxel->data.position, temp_voxel->data.position);
  if (dist <= radius) {
    this->voxels_to_render.push_back(voxel);
  }

  int curr_dim = depth % 3;

  float pos_dim_value = temp_voxel->GetDimValue(curr_dim);
  float voxel_dim_value = voxel->GetDimValue(curr_dim);

  if (pos_dim_value < voxel_dim_value) {
    LoadRenderBuffer(voxel->left, temp_voxel, radius, depth + 1);

    if (pos_dim_value + radius >= voxel_dim_value) {
      LoadRenderBuffer(voxel->right, temp_voxel, radius, depth + 1);
    }
  } else {
    LoadRenderBuffer(voxel->right, temp_voxel, radius, depth + 1);

    if (pos_dim_value - radius <= voxel_dim_value) {
      LoadRenderBuffer(voxel->left, temp_voxel, radius, depth + 1);
    }
  }
}

float Chunk::GetDistance(const Vector3& vec1, const Vector3& vec2) const {
  float dx = vec1.x - vec2.x;
  float dy = vec1.y - vec2.y;
  float dz = vec1.z - vec2.z;
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

inline bool Chunk::CompareVoxels(const Vector3& vec1,
                                 const Vector3& vec2) const {
  return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z;
}

void Chunk::UpdateRoot(std::shared_ptr<Voxel>& voxel) {
  /* Updating the tree, use only shared pointers */
  this->root = InsertVoxel(root, voxel, 0);
}
