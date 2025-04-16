#pragma once

enum class AgentAction {
  MOVE_X_NORTH = 0,
  MOVE_X_SOUTH = 1,
  MOVE_X_EAST = 2,
  MOVE_X_WEST = 3,
  ROLL = 4,
  PROJECTION = 5,
  IDLE = 6,
  TRY_TO_DESTROY = 100,
  TRY_TO_CREATE = 101,
};
