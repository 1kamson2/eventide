#include <raylib.h>

#include <iostream>

#include "Engine/Engine.hpp"

int main() {
  Engine run{};
  float delta{0};
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(WIDTH, HEIGHT, "Test");
  SetTargetFPS(1000);
  DisableCursor();
  while (!WindowShouldClose()) {
    delta = GetFrameTime();
    BeginDrawing();
    ClearBackground(BLUE);
    BeginMode3D(run.agt.cam);
    run.GameLoop(delta);

    EndMode3D();
    DrawFPS(10, 10);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}

// TODO: Frustum culling bugging out
// TODO: Multithreading
