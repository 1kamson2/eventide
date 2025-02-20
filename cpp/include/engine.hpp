#pragma once
#include <math.h>
#include <raylib.h>

#include <tuple>
#include <vector>
using namespace std;
#include "agent.hpp"
#include "environment.hpp"
using PositionVectors = std::tuple<double, double, double>;
using Voxel = EnvironmentObject;
using VoxelNode = EnvironmentNode<EnvironmentObject>;
class KDTree;
class Engine {
 public:
  Engine();
  // Engine(params);
  Agent AgentInstance;
  int width, height, fps, collision_where;
  int max_voxels_in_view;
  double gravity;
  /* This one holds all generated voxels in the memory */
  mutable std::vector<std::shared_ptr<VoxelNode>> env_buffer;
  /* This one is an interface for voxels */
  KDTree env_to_render();
  mutable EnvironmentState recent_env_state;

  /* TODO: SHOULD REFACTOR TO VOXEL NODE??? */
  void ProcessInput(const float& delta);
  void RenderVoxels(const float& delta);
  bool IsVoxelInView(const float& delta, PositionVectors delta_vector) const;
  void FindVoxelsInView(const float& delta);
  void Update(const float& delta);
  void DetectCollision(const float& delta);
  void ModifyEnvironment(const float& delta);
  std::vector<Voxel> LoadEnvironmentBuffer();
  PositionVectors VoxelPositionVectors(const float& delta, Voxel voxel) const;
  double VoxelDistanceVector(PositionVectors vector) const;
  bool VoxelHeuristic(const Voxel& voxel1, const Voxel& voxel2) const;
  void DebugInfo();
};

class KDTree {
 public:
  std::shared_ptr<VoxelNode> root;
  std::shared_ptr<VoxelNode> best_node_cand;
  std::vector<std::shared_ptr<VoxelNode>> nodes_to_render;
  mutable double best_distance_cand;
  mutable int rendered = 0;
  std::shared_ptr<VoxelNode> InsertNodes(
      std::shared_ptr<VoxelNode> last_free_node,
      std::shared_ptr<VoxelNode> voxel, int depth) {
    if (!last_free_node) {
      return voxel;
    }

    int cd = depth % 3;
    float voxel_dim_value = -1;
    float node_dim_value = -1;
    switch (cd) {
      case 0:
        voxel_dim_value = voxel->data.position.x;
        node_dim_value = last_free_node->data.position.x;
        break;
      case 1:
        voxel_dim_value = voxel->data.position.y;
        node_dim_value = last_free_node->data.position.y;
        break;
      case 2:
        voxel_dim_value = voxel->data.position.z;
        node_dim_value = last_free_node->data.position.z;
        break;
    }
    if (voxel_dim_value < node_dim_value) {
      last_free_node->left =
          InsertNodes(last_free_node->left, voxel, depth + 1);
    } else {
      last_free_node->right =
          InsertNodes(last_free_node->right, voxel, depth + 1);
    }

    return last_free_node;
  }

  inline bool CompareVectors3Dim(const Vector3& vec1,
                                 const Vector3& vec2) const {
    return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z;
  }

  bool FindNodes(std::shared_ptr<VoxelNode> node,
                 const std::shared_ptr<VoxelNode>& voxel_node_to_find,
                 int depth) const {
    if (node == nullptr) return false;

    if (CompareVectors3Dim(node->data.position,
                           voxel_node_to_find->data.position)) {
      return true;
    }

    int cd = depth % 3;
    float voxel_dim_value = -1;
    float node_dim_value = -1;
    switch (cd) {
      case 0:
        voxel_dim_value = voxel_node_to_find->data.position.x;
        node_dim_value = node->data.position.x;
        break;
      case 1:
        voxel_dim_value = voxel_node_to_find->data.position.y;
        node_dim_value = node->data.position.y;
        break;
      case 2:
        voxel_dim_value = voxel_node_to_find->data.position.z;
        node_dim_value = node->data.position.z;
        break;
    }
    if (voxel_dim_value < node_dim_value) {
      return FindNodes(node->left, voxel_node_to_find, depth + 1);
    } else {
      return FindNodes(node->right, voxel_node_to_find, depth + 1);
    }
  }

  std::shared_ptr<VoxelNode> FindNearestNeighbor(
      const std::shared_ptr<VoxelNode> target) {
    if (!this->root) {
      return nullptr;
    }

    this->best_node_cand = nullptr;
    this->best_distance_cand = std::numeric_limits<double>::max();

    this->SearchNearestNode(this->root, target->data.position, 0,
                            this->best_node_cand, this->best_distance_cand);
    return this->best_node_cand;
  }

  // 1. To get the nodes, call FindNodesInRange
  std::vector<std::shared_ptr<VoxelNode>> FindNodesInRange(
      const Vector3& position, float radius) {
    /* Main function to access the closest nodes */
    std::vector<std::shared_ptr<VoxelNode>> found_nodes;
    if (!root) return found_nodes;

    SearchNodesInRange(root, position, radius, found_nodes, 0);
    this->rendered = 0;
    return found_nodes;
  }

  void SearchNearestNode(std::shared_ptr<VoxelNode> node,
                         const Vector3& target_pos, int depth,
                         std::shared_ptr<VoxelNode>& best_node,
                         double& best_distance) {
    if (!node) return;

    float current_distance = CalculateDistance(node->data.position, target_pos);

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

    // Determine which child to search first
    if (target_dim_value < node_dim_value) {
      SearchNearestNode(node->left, target_pos, depth + 1, best_node,
                        best_distance);

      // Check other side if needed
      float axis_distance = std::abs(target_dim_value - node_dim_value);
      if (axis_distance * axis_distance < best_distance) {
        SearchNearestNode(node->right, target_pos, depth + 1, best_node,
                          best_distance);
      }
    } else {
      SearchNearestNode(node->right, target_pos, depth + 1, best_node,
                        best_distance);

      // Check other side if needed
      float axis_distance = std::abs(target_dim_value - node_dim_value);
      if (axis_distance * axis_distance < best_distance) {
        SearchNearestNode(node->left, target_pos, depth + 1, best_node,
                          best_distance);
      }
    }
  }

  void SearchNodesInRange(std::shared_ptr<VoxelNode> node,
                          const Vector3& position, float radius,
                          std::vector<std::shared_ptr<VoxelNode>>& found_nodes,
                          int depth) {
    /* TODO: For now the precalculated value is not used */
    if (!node || this->rendered < 2048) return;

    // Check if current node is within radius
    float dist = CalculateDistance(node->data.position, position);
    if (dist <= radius) {
      this->rendered++;
      found_nodes.push_back(node);
    }

    // Get current dimension
    int cd = depth % 3;
    float pos_dim_value = -1;
    float node_dim_value = -1;

    switch (cd) {
      case 0:
        pos_dim_value = position.x;
        node_dim_value = node->data.position.x;
        break;
      case 1:
        pos_dim_value = position.y;
        node_dim_value = node->data.position.y;
        break;
      case 2:
        pos_dim_value = position.z;
        node_dim_value = node->data.position.z;
        break;
    }

    // Check appropriate sides based on position
    if (pos_dim_value < node_dim_value) {
      SearchNodesInRange(node->left, position, radius, found_nodes, depth + 1);

      // Check right side if sphere crosses partition
      if (pos_dim_value + radius >= node_dim_value) {
        SearchNodesInRange(node->right, position, radius, found_nodes,
                           depth + 1);
      }
    } else {
      SearchNodesInRange(node->right, position, radius, found_nodes, depth + 1);

      // Check left side if sphere crosses partition
      if (pos_dim_value - radius <= node_dim_value) {
        SearchNodesInRange(node->left, position, radius, found_nodes,
                           depth + 1);
      }
    }
  }

  inline float CalculateDistance(const Vector3& a, const Vector3& b) const {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  KDTree() : root(nullptr) {}
  void UpdateRoot(const std::shared_ptr<VoxelNode> voxel) {
    /* Updating the tree, use only shared pointers */
    this->root = InsertNodes(root, voxel, 0);
  }
};
