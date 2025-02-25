#include "Chunk.hpp"

#include <cstdint>
#include <limits>
#include <memory>

#include "Globals.hpp"

template <typename Data>
Chunk<Data>::Chunk(const int& id, const float& x, const float& y,
                   const float& z)
    : root(nullptr),
      best_voxel_cand(nullptr),
      best_distance_cand(0),
      id(id),
      start_of_chunk((Vector3){x, y, z}) {
  std::cout << "[Chunk] Initialized with id: " << id << "." << std::endl;
}

template <typename Data>
std::unique_ptr<Voxel<Data>> Chunk<Data>::InsertVoxel(
    std::unique_ptr<Voxel<Data>>& last_free_voxel,
    std::unique_ptr<Voxel<Data>>& new_voxel, int depth, int8_t x, int8_t y,
    int8_t z) {
  /*
   * This function traverses recursively through the KD-Tree, while meeting the
   * conditions. This also divides the entire tree into left and right side. If
   * we find the null value, we insert the Voxel<Data>, thus it is now ready
   * to be used.
   */
  if (!last_free_voxel) {
    /* We found a Node, that is free */
    new_voxel->axis_x = x;
    new_voxel->axis_y = y;
    new_voxel->axis_z = z;
    return new_voxel;
  }

  int curr_dim = depth % 3;
  float new_voxel_dim_value = new_voxel->GetDimValue(curr_dim);
  float voxel_dim_value = last_free_voxel->GetDimValue(curr_dim);
  if (new_voxel_dim_value < voxel_dim_value) {
    x = curr_dim == 0 ? x + 1 : x;
    y = curr_dim == 1 ? y + 1 : y;
    z = curr_dim == 2 ? z + 1 : z;
    last_free_voxel->left =
        InsertVoxel(last_free_voxel->left, new_voxel, depth + 1, x, y, z);
  } else {
    x = curr_dim == 0 ? x - 1 : x;
    y = curr_dim == 1 ? y - 1 : y;
    z = curr_dim == 2 ? z - 1 : z;
    last_free_voxel->right =
        InsertVoxel(last_free_voxel->right, new_voxel, depth + 1);
  }

  return last_free_voxel;
}

template <typename Data>
bool Chunk<Data>::FindVoxel(const std::unique_ptr<Voxel<Data>>& current_voxel,
                            const std::unique_ptr<Voxel<Data>>& voxel_to_find,
                            int depth) const {
  if (current_voxel == nullptr) return false;

  if (CompareVoxels(current_voxel->data.position,
                    voxel_to_find->data.position)) {
    return true;
  }

  int curr_dim = depth % 3;
  int8_t voxel_dim_value = voxel_to_find->GetDimValue(curr_dim);
  int8_t curr_voxel_dim_value = current_voxel->GetDimValue(curr_dim);
  if (voxel_dim_value < curr_voxel_dim_value) {
    return FindVoxel(current_voxel->left, voxel_to_find, depth + 1);
  } else {
    return FindVoxel(current_voxel->right, voxel_to_find, depth + 1);
  }
}

template <typename Data>
std::unique_ptr<Voxel<Data>> Chunk<Data>::FindNearestVoxel(
    const std::unique_ptr<Voxel<Data>> target) {
  if (!this->root) {
    return nullptr;
  }

  this->best_voxel_cand = nullptr;
  this->best_distance_cand = std::numeric_limits<double>::max();

  this->SearchForNearestVoxel(this->root, target->data.position, 0,
                              this->best_voxel_cand, this->best_distance_cand);
  return this->best_voxel_cand;
}

template <typename Data>
void Chunk<Data>::SearchForNearestVoxel(std::unique_ptr<Voxel<Data>>& node,
                                        const Vector3& target_pos, int depth,
                                        std::unique_ptr<Voxel<Data>>& best_node,
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

template <typename Data>
int8_t Chunk<Data>::GetDistance(
    const std::unique_ptr<Voxel<Data>>& voxel1,
    const std::unique_ptr<Voxel<Data>>& voxel2) const {
  int8_t dx = voxel1->axis_x - voxel2->axis_x;
  int8_t dy = voxel1->axis_y - voxel2->axis_y;
  int8_t dz = voxel1->axis_z - voxel2->axis_z;
  return dx * dx + dy * dy + dz * dz;
}

template <typename Data>
inline bool Chunk<Data>::CompareVoxels(
    const std::unique_ptr<Voxel<Data>>& voxel1,
    const std::unique_ptr<Voxel<Data>>& voxel2) const {
  return voxel1->axis_x == voxel2->axis_x && voxel1->axis_y == voxel2->axis_y &&
         voxel1->axis_z == voxel2->axis_z;
}

template <typename Data>
void Chunk<Data>::UpdateRoot(std::unique_ptr<Voxel<Data>>& voxel) {
  /* Updating the tree, use only unique
   * pointers */
  this->root = InsertVoxel(root, voxel, 0);
}

template <typename Data>
bool Chunk<Data>::ShouldRenderChunk(const Cursor& cursor) const {
  float dist_cursor = cursor.position.x * cursor.position.x +
                      cursor.position.y * cursor.position.y +
                      cursor.position.z * cursor.position.z;
  float dist_chunk = start_of_chunk.x * start_of_chunk.x +
                     start_of_chunk.y * start_of_chunk.y +
                     start_of_chunk.z * start_of_chunk.z;
  return dist_chunk - dist_cursor <= RENDER_DISTANCE * RENDER_DISTANCE;
}
