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
    /* Remove this later */
    EnvironmentObject cursor = run.AgentInstance.cursor;
    DrawCube(cursor.position, cursor.length, cursor.length, cursor.length,
             GREEN);

    EndMode3D();
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
