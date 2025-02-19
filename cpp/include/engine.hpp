#pragma once
#include <raylib.h>

#include <iostream>

#include "agent.hpp"
#include "environment.hpp"
#include "resource_manager.hpp"
class Engine {
 public:
  Engine();
  // Engine(params);
  Agent AgentInstance;
  int width, height, fps, collision_where;
  double gravity;
  EnvironmentObject* env;

  void ProcessInput(const float& delta);
  void RenderPrimitives(const float& delta);
  void Update(const float& delta);
  void DetectCollision(const float& delta);
  void DebugInfo();
};
