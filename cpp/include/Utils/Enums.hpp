#pragma once

namespace agent_enums {
enum class AgentMovement {
  FORWARD = 0,
  BACKWARD = 1,
  RIGHT = 2,
  LEFT = 3,
  PROJECT = 5,
  IDLE = 6,
};
};

namespace chunk_enums {
enum class Visibility { VISIBLE = 0, INVISIBLE = 1 };
};
