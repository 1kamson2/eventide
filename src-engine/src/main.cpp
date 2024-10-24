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

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    BeginMode2D(engine.camera);
    ClearBackground(BLUE);
    ii render = engine.renderDistance();
    for (int i = render.first; i < render.second; i++) {
      DrawRectangleRec(engine.wseed->trees[i], engine.wseed->treeColors[i]);
    }
    for (int i = 0; i < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LEN; ++i) {
      DrawRectangleRec(engine.tiles[i], BLACK);
    }
    // DrawRectangle(-6000, 320, 13000, 8000, GREEN);
    DrawRectangleRec(engine.player->hitbox, RED);
    EndMode2D();
    EndDrawing();
    //----------------------------------------------------------------------------------
    // Simple debug:
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
