#include <raylib.h>

#include <iostream>

#include "Engine/Engine.hpp"

int main() {
  Engine run{};
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(run.width, run.height, "Test");
  SetTargetFPS(1 << 31);
  DisableCursor();
  float delta;
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

// TODO: No colors, prob we lose somewhere our reference to the color.
// TODO: Make rendering faster
//        - BFS? DFS?
//        - There is no need to render unseen voxels
