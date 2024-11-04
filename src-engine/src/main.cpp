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
    ii mousePos = engine.mouseWorldCoords();
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    BeginMode2D(engine.camera);
    ClearBackground(BLUE);

    for (int idx = treesDist.first; idx < treesDist.second; ++idx) {
      DrawRectangleRec(engine.tree_buff[idx].rect, engine.tree_buff[idx].color);
    }

    for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
      for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
        DrawRectangleRec(engine.tiles_buff[y][x].rect,
                         engine.tiles_buff[y][x].color);
      }
    }
    DrawRectangleRec(engine.player->hitbox.rect, engine.player->hitbox.color);
    EndMode2D();
    EndDrawing();
    //----------------------------------------------------------------------------------
    engine.debug();
    std::cout << "MOUSE POS: " << mousePos.first << " " << mousePos.second
              << std::endl;
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

// Make Inventory.
// Add ores.
// Infinite world generation.
// Fix placing and destroying.
// Fix collisions.
// Add textures.
// Add multiplayer.
// Fix code.
// Add parallax.
