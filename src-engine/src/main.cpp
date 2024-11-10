#include <raylib.h>

#include "engine.hpp"
#include "resource_manager.hpp"
int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(640, 480, "Test");
  SetTargetFPS(60);
  float delta{0};
  EventideEngine engine(640, 480, 60);
  engine.eventideInit(313);
  float scrollingBack = 0.0f;
  float scrollingMid = 0.0f;
  float scrollingFore = 0.0f;
  while (!WindowShouldClose()) {
    delta = GetFrameTime();
    engine.update(delta);

    BeginDrawing();
    BeginMode2D(engine.camera);
    ClearBackground(BLUE);

    for (int idx = 11; idx >= 0; --idx) {
      if (scrollingBack < -1000) {
        scrollingBack = 0;
      }
      scrollingBack -= 0.001f;
      DrawTextureEx(engine.bgTexture[idx],
                    (Vector2){-2048 + scrollingBack, -200}, 0, 1,
                    WHITE);  //
      // DrawTexture(engine.bgTexture[idx], -2048, -200, WHITE);
      DrawTextureEx(engine.bgTexture[idx],
                    (Vector2){-1948 + scrollingBack, -200}, 0, 1,
                    WHITE);  //
    }
    for (int idx = 0; idx < 1000; ++idx) {
      if (engine.State == GameState::DEBUGGING) {
        DrawRectangleRec(engine.tree_buff[idx].rect,
                         engine.tree_buff[idx].color);
      } else if (engine.State == GameState::ACTIVE ||
                 engine.State == GameState::MENU) {
        DrawTexture(engine.tree_buff[idx].texture, engine.tree_buff[idx].rect.x,
                    engine.tree_buff[idx].rect.y, WHITE);
      }
    }
    for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
      for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
        if (engine.State == GameState::DEBUGGING) {
          DrawRectangleRec(engine.tiles_buff[y][x].rect,
                           engine.tiles_buff[y][x].color);

        } else if (engine.State == GameState::ACTIVE ||
                   engine.State == GameState::MENU) {
          DrawTexture(engine.tiles_buff[y][x].texture,
                      engine.tiles_buff[y][x].rect.x,
                      engine.tiles_buff[y][x].rect.y, WHITE);
        }
      }
    }
    if (engine.State == GameState::MENU) {
      DrawRectangleRec(engine.player->health.rect, engine.player->health.color);
      for (int i = 0; i < 9; ++i) {
        DrawRectangleRec(engine.player->hotbar[i].rect,
                         engine.player->hotbar[i].color);
      }
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 8; ++j) {
          DrawRectangleRec(engine.player->inventory[i][j].rect,
                           engine.player->inventory[i][j].color);
        }
      }
      DrawText(TextFormat("HEALTH: %d / %d", engine.player->health.quantity,
                          MAX_HEALTH_TEXT),
               engine.player->health.rect.x, engine.player->health.rect.y - 20,
               10, BLACK);
    }

    DrawRectangleRec(engine.player->hitbox.rect, engine.player->hitbox.color);

    EndMode2D();
    EndDrawing();
    engine.debug();
  }
  CloseWindow();

  return 0;
}
// bind ui to camera not the player...
// Make Inventory - done.
// Add ores.
// Infinite world generation.
// Fix placing and destroying - works better, the same as down below, add
// intervals. Fix collisions - works better, add case by case if x < 0 and x >
// 0 because it is the source of problems. Add textures - done. Add
// Parallax ----------------------------------------
// This should be bound to the camera, to create ilusion of depth of the forest.
// Multiplayer -------------------------------------
// I would do a world seed structure. There will be seed, so each player could
// generate his own world and connect. Such a thing will work, because it is
// pseudorandom, therefore it will yield the same result each time with the same
// seed.
