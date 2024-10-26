#include <raylib.h>

#include <iostream>

#include "engine.hpp"
int main() {
  EventideEngine engine(640, 480, 60);
  engine.eventideInit(33);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(engine.width, engine.height, "Test");
  SetTargetFPS(engine.fps);
  float delta{0};
  while (!WindowShouldClose()) {
    delta = GetFrameTime();
    engine.processInput(delta);

    // update game state
    // -----------------
    engine.update(delta);

    // Camera target follows player
    // render
    // ------
    // engine.Render();
    ii treesDist = engine.renderTrees();
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    BeginMode2D(engine.camera);
    ClearBackground(BLUE);

    for (int idx = treesDist.first; idx < treesDist.second; ++idx) {
      DrawRectangleRec(engine.wseed->trees[idx], engine.wseed->treeColors[idx]);
    }

    for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
      for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
        DrawRectangleRec(engine.tiles_buf[y][x].tile,
                         engine.tiles_buf[y][x].col);
      }
    }
    DrawRectangleRec(engine.player->hitbox, RED);
    EndMode2D();
    EndDrawing();
    //----------------------------------------------------------------------------------
    std::cout << "FPS: " << GetFPS() << std::endl;
    std::cout << "X: " << engine.player->hitbox.x << std::endl;
    std::cout << "Y: " << engine.player->hitbox.y << std::endl;
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
