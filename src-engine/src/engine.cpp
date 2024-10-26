#include "engine.hpp"

#include <raylib.h>

EventideEngine::EventideEngine(unsigned int width, unsigned int height,
                               unsigned int fps)
    : width{width},
      height{height},
      fps{fps},
      player{new PlayerObject},
      wseed{new WorldSeed},
      State{GameState::GAME_ACTIVE} {
  this->camera = {0};
  camera.target = (Vector2){player->hitbox.x + 20.0f, player->hitbox.y + 20.0f};
  camera.offset = (Vector2){this->width / 2.0f, this->height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
}

void EventideEngine::eventideInit(unsigned int seed) {
  SetRandomSeed(seed);
  int depth{0};
  // SMALL_WORLD_DEPTH
  // SMALL_WORLD_STRIDE
  for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
    depth = y * TILE_SZ;
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      if (depth == 0) {
        // GRASS ENUM
        this->tiles_buf[y][x].tile =
            (Rectangle){(float)SMALL_WORLD_START + x * TILE_SZ, (float)depth,
                        TILE_SZ, TILE_SZ};
        this->tiles_buf[y][x].col = (Color){GREEN};
      } else if (depth > 0 && depth <= 60) {
        // DIRT ENUM
        this->tiles_buf[y][x].tile =
            (Rectangle){(float)SMALL_WORLD_START + x * TILE_SZ, (float)depth,
                        TILE_SZ, TILE_SZ};
        this->tiles_buf[y][x].col = (Color){BROWN};
      } else if (depth > 60) {
        int isStone = GetRandomValue(0, 100);

        // STONE ENUM
        this->tiles_buf[y][x].tile =
            (Rectangle){(float)SMALL_WORLD_START + x * TILE_SZ, (float)depth,
                        TILE_SZ, TILE_SZ};
        if (isStone >= 0 && isStone <= 77) {
          this->tiles_buf[y][x].col = (Color){GRAY};
        } else if (isStone > 77 && isStone <= 100) {
          this->tiles_buf[y][x].col = (Color){BROWN};
        }
      }
    }
  }
  int pos = 0;
  while (pos < 1000) {
    pos += GetRandomValue(0, TILE_SZ);

    wseed->trees[pos].width = (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ);
    wseed->trees[pos].height = (float)GetRandomValue(5 * TILE_SZ, 15 * TILE_SZ);
    wseed->trees[pos].y = -100.0f + (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ);
    wseed->trees[pos].x = SMALL_WORLD_START + pos * TILE_SZ;
    wseed->treeColors[pos] =
        (Color){(unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200), 255};
    pos += 2 * TILE_SZ;
  }
}

void EventideEngine::processInput(float dt) {
  if (IsKeyDown(KEY_RIGHT)) {
    player->hitbox.x += 100;
  } else if (IsKeyDown(KEY_LEFT)) {
    player->hitbox.x -= 100;
  } else if (IsKeyDown(KEY_DOWN)) {
    player->hitbox.y += 100;
  } else if (IsKeyDown(KEY_UP)) {
    player->hitbox.y -= 100;
  }
}
void EventideEngine::update(float dt) {
  camera.target = (Vector2){player->hitbox.x + 20, player->hitbox.y + 20};
  // this->renderTiles();
}
ii EventideEngine::getPlayerIndex() const {
  return ii(ceil((player->hitbox.x - SMALL_WORLD_START) / TILE_SZ),
            ceil(player->hitbox.y / TILE_SZ));
}
ii EventideEngine::renderTrees() const {
  int currIdx = this->getPlayerIndex().first;
  return ii(std::max((int)(currIdx - RENDER_DISTANCE / 2), 0),
            std::min(1000, (int)(currIdx + RENDER_DISTANCE / 2)));
}

void EventideEngine::renderTiles() { std::cout << "Do nothing" << std::endl; }

EventideEngine::~EventideEngine() {
  delete player;
  delete wseed;
  player = nullptr;
  wseed = nullptr;
}

// put getPlayerIndex into update function, and as class attribute
