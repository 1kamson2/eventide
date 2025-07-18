#pragma once

#include <raylib.h>

#include <memory>

namespace voxel {
class Voxel;

using VoxelRef = std::shared_ptr<Voxel>&;
using VoxelCRef = const std::shared_ptr<Voxel>&;
constexpr float LENGTH = 1.0f;

class Voxel {
 public:
  Color color;
  Vector3 position{0.0f, 0.0f, 0.0f};
  Vector3 center{LENGTH / 2, -LENGTH / 2, LENGTH / 2};
  Vector3 extents{LENGTH / 2, -LENGTH * 1.5f, LENGTH / 2};
  std::shared_ptr<Voxel> left = nullptr;
  std::shared_ptr<Voxel> right = nullptr;

  Voxel(Vector3 position, Color color);
  float GetAxis(const int& dim) const;
  Vector3 GetMidPoint() const;
  Vector3 GetPosition() const;
  Vector3 GetExtentsPoint() const;
  bool IsBlank();
};
}  // namespace voxel
