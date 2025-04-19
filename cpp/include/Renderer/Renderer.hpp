#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include "Chunk/Chunk.hpp"
#include "Renderer/Agent.hpp"
#include "Utils/VecOperations.hpp"
#include "Voxel/Voxel.hpp"

using namespace voxel;
using namespace chunk;
using namespace vec_operations;
using namespace agent;
namespace renderer {
constexpr float Z_FAR = 5.0f;
constexpr float Z_NEAR = 3.0f;
const float HALF_VERT_SIDE = Z_FAR * tanf(DEFAULT_FOV * 0.5f);
const float HALF_HORIZ_SIDE = HALF_VERT_SIDE * ASPECT;

struct Plane {
  Vector3 normal{0.0f, 1.0f, 0.0f};
  float dist = 0.0f;
  Plane();
  bool IsOnPlane(const std::shared_ptr<Voxel>& voxel) const;
  Plane(Vector3 normal, Vector3 dist_vec);
  float GetDistanceToPoint(const Vector3& point) const;
};

struct Frustum {
  const float half_vert_side;
  const float half_horiz_side;
  float z_far;
  float z_near;
  Plane top_face;
  Plane bottom_face;
  Plane right_face;
  Plane left_face;
  Plane far_face;
  Plane near_face;
  Frustum();
  void Update(const Agent& agt);
};

class Renderer {
 public:
  std::shared_ptr<Voxel> best_cand_voxel = nullptr;
  float best_distance_voxel = 0.0f;
  int rendered_voxels = 0;
  Agent& agt;
  Frustum frustum;
  Renderer(Agent& agt);

  bool InFrustum(const Frustum& cfrustum, std::shared_ptr<Voxel> voxel) const;
  void TraverseChunks(std::vector<Chunk>& chunks_to_render);
  void RenderVoxels(std::shared_ptr<Voxel>& voxel);
};
};  // namespace renderer
