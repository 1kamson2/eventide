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
        DrawRectangleRec(engine.tiles_buf[y][x].rect,
                         engine.tiles_buf[y][x].color);
      }
    }
    DrawRectangleRec(engine.player->hitbox.rect, engine.player->hitbox.color);
    EndMode2D();
    EndDrawing();
    //----------------------------------------------------------------------------------
    std::cout << "FPS: " << GetFPS() << std::endl;
    std::cout << "X: " << engine.player->pos.x << std::endl;
    std::cout << "Y: " << engine.player->pos.y << std::endl;
    std::cout << "MOUSE POS: " << GetMouseX() << " " << GetMouseY()
              << std::endl;
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
