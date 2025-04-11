#include "Ray.h"

_Ray::_Ray(const Vector3& origin, const Vector3& direction)
    : origin(origin), direction(direction) {}

Vector3 _Ray::GetOrigin() const { return this->origin; }
Vector3 _Ray::GetDirection() const { return this->direction; }
Vector3 _Ray::At(float t) const {
  return (Vector3){origin.x + t * direction.x, origin.y + t * direction.y,
                   origin.z + t * direction.z};
}
