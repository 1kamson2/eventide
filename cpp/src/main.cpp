#include <raylib.h>

#include <iostream>

#include "engine.hpp"

int main() {
  Engine run{};
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(run.width, run.height, "Test");
  SetTargetFPS(run.fps);
  DisableCursor();
  float delta;
  while (!WindowShouldClose()) {
    delta = GetFrameTime();

    BeginDrawing();
    ClearBackground(BLUE);
    BeginMode3D(run.AgentInstance.camera);
    run.Update(delta);
    /* TODO: Use meshing instead of drawing cubes. */
    DrawCube(run.AgentInstance.cursor->data.position,
             run.AgentInstance.cursor->data.length,
             run.AgentInstance.cursor->data.length,
             run.AgentInstance.cursor->data.length, GREEN);

    EndMode3D();
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
