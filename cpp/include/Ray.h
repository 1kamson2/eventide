#include <iostream>

#include "Globals.hpp"

class _Ray {
 public:
  Vector3 origin, direction;
  _Ray(const Vector3& origin, const Vector3& direction);
  Vector3 GetOrigin() const;
  Vector3 GetDirection() const;
  Vector3 At(float t) const;
};
