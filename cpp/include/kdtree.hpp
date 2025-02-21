#pragma once
#include <math.h>
#include <raylib.h>

#include <tuple>
#include <vector>

#include "agent.hpp"
#include "environment.hpp"
#include "kdtree.hpp"
using PositionVectors = std::tuple<double, double, double>;
using Voxel = EnvironmentObject;
using VoxelNode = EnvironmentNode<EnvironmentObject>;

class KDTree {
 public:
  std::shared_ptr<VoxelNode> root;
  std::shared_ptr<VoxelNode> best_node_cand;
  std::vector<std::shared_ptr<VoxelNode>> nodes_to_render;
  mutable double best_distance_cand;
  mutable int rendered = 0;
  KDTree();
  std::shared_ptr<VoxelNode> InsertNodes(
      std::shared_ptr<VoxelNode> last_free_node,
      std::shared_ptr<VoxelNode> voxel, int depth);

  bool CompareVectors3Dim(const Vector3& vec1, const Vector3& vec2) const;
  bool FindNodes(std::shared_ptr<VoxelNode> node,
                 const std::shared_ptr<VoxelNode>& voxel_node_to_find,
                 int depth) const;
  std::shared_ptr<VoxelNode> FindNearestNeighbor(
      const std::shared_ptr<VoxelNode> target);
  std::vector<std::shared_ptr<VoxelNode>> FindNodesInRange(
      const Vector3& position, float radius);
  void SearchNearestNode(std::shared_ptr<VoxelNode> node,
                         const Vector3& target_pos, int depth,
                         std::shared_ptr<VoxelNode>& best_node,
                         double& best_distance);
  void SearchNodesInRange(std::shared_ptr<VoxelNode> node,
                          const Vector3& position, float radius,
                          std::vector<std::shared_ptr<VoxelNode>>& found_nodes,
                          int depth);
  float CalculateDistance(const Vector3& a, const Vector3& b) const;
  void UpdateRoot(const std::shared_ptr<VoxelNode> voxel);
};
