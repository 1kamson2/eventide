#include <raylib.h>

#include "engine.hpp"
int main() {
  EventideEngine engine(640, 480, 60);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(engine.width, engine.height, "Test");
  SetTargetFPS(engine.fps);
  float delta{0};
  // initialize here
  engine.eventideInit(313);
  while (!WindowShouldClose()) {
    delta = GetFrameTime();

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

    for (int idx = 0; idx < 1000; ++idx) {
      if (engine.State == GameState::DEBUGGING) {
        DrawRectangleRec(engine.tree_buff[idx].rect,
                         engine.tree_buff[idx].color);
      } else {
        DrawTexture(engine.tree_buff[idx].texture, engine.tree_buff[idx].rect.x,
                    engine.tree_buff[idx].rect.y, WHITE);
      }
    }

    for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
      for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
        if (engine.State == GameState::DEBUGGING) {
          DrawRectangleRec(engine.tiles_buff[y][x].rect,
                           engine.tiles_buff[y][x].color);
        } else {
          DrawTexture(engine.tiles_buff[y][x].texture,
                      engine.tiles_buff[y][x].rect.x,
                      engine.tiles_buff[y][x].rect.y, WHITE);
        }
      }
    }
    DrawRectangleRec(engine.player->hitbox.rect, engine.player->hitbox.color);
    EndMode2D();
    EndDrawing();
    //----------------------------------------------------------------------------------
    engine.debug();
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
// Fix placing and destroying - works better, the same as down below, add
// intervals. Fix collisions - works better, add case by case if x < 0 and x >
// 0 because it is the source of problems. Add textures - done. Add
// multiplayer. Fix code. Add parallax.
