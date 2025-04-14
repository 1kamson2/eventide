#pragma once
#include <raylib.h>

#include <memory>
#include <vector>

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
