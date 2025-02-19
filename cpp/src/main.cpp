#include <raylib.h>

#include <iostream>

#include "engine.hpp"

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1280, 960, "Test");
  SetTargetFPS(60);
  DisableCursor();
  float delta;
  Engine run{};
  EnvironmentObject* env = Environment::CreateDefaultEnvironment();
  while (!WindowShouldClose()) {
    delta = GetFrameTime();

    BeginDrawing();
    ClearBackground(BLUE);
    BeginMode3D(run.AgentInstance.camera);
    run.Update(delta);
    /* Remove this later */
    EnvironmentObject cursor = run.AgentInstance.cursor;
    DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f},
              GRAY);  // Draw ground
    DrawCube((Vector3){-16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
             BLUE);  // Draw a blue wall
    DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
             LIME);  // Draw a green wall
    DrawCube((Vector3){0.0f, 2.5f, 16.0f}, 32.0f, 5.0f, 1.0f,
             GOLD);  // Draw a yellow wall
    DrawCube(cursor.position, cursor.width, cursor.height, cursor.length, RED);

    EndMode3D();
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
