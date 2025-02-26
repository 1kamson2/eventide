#include "Globals.hpp"
namespace camvars {
const double CAMERA_DEFAULT_POSITION_X = 0.0f;
const double CAMERA_DEFAULT_POSITION_Y = 2.0f;
const double CAMERA_DEFAULT_POSITION_Z = 4.0f;
const Vector3 CAMERA_DEFAULT_TARGET = (Vector3){0.0f, 2.0f, 0.0f};
const Vector3 CAMERA_DEFAULT_ROTATION = (Vector3){0.0f, 1.0f, 0.0f};
const double CAMERA_DEFAULT_FOV = 90.0f;
const double CAMERA_DEFAULT_PERSPECTIVE = CAMERA_PERSPECTIVE;
const double CAMERA_DEFAULT_MODE = CAMERA_FIRST_PERSON;
const double CAMERA_DEFAULT_X_FACTOR = 0.1f;
const double CAMERA_DEFAULT_ROTATE_FACTOR = 0.05f;
const double CAMERA_DEFAULT_MAX_SPEED = 1.0f;
}  // namespace camvars

namespace engvars {
const int MAX_OBJECTS_IN_AREA = 1 << 18;
const int CHUNK_SIZE = 16;
const int TICKRATE = 20;

/* RENDER_DISTANCE unit: chunks */
const int RENDER_DISTANCE = 16;
const double EDGE_LENGTH = 1.0f;
const double CURSOR_LENGTH = 0.1f;

const int RED_COLOR_MASK = 0xFF000000;
const int GREEN_COLOR_MASK = 0x00FF0000;
const int BLUE_COLOR_MASK = 0x0000FF00;
const int ALPHA = 0x000000FF;
const int Z_COORD_MASK = 0x3FF;
const int Y_COORD_MASK = 0xFCC00;
const int X_COORD_MASK = 0x3FF00000;
}  // namespace engvars
namespace octree {
const uint32_t BOTTOM_LEFT_FRONT = 0;
const uint32_t BOTTOM_RIGHT_FRONT = 1;
const uint32_t BOTTOM_LEFT_BACK = 2;
const uint32_t BOTTOM_RIGHT_BACK = 3;
const uint32_t TOP_LEFT_FRONT = 4;
const uint32_t TOP_RIGHT_FRONT = 5;
const uint32_t TOP_LEFT_BACK = 6;
const uint32_t TOP_RIGHT_BACK = 7;
}  // namespace octree
