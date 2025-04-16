#pragma once

#include <raylib.h>

#include <memory>
namespace voxel {

constexpr float LENGTH = 1.0f;

class Voxel {
 public:
  Color color;
  Vector3 position;
  std::shared_ptr<Voxel> left = nullptr;
  std::shared_ptr<Voxel> right = nullptr;

  Voxel(Vector3 position, Color color);
  float GetAxis(const int& dim) const;
  Vector3 GetPosition() const;
  bool IsBlank();
};
}  // namespace voxel
