#include "environment.hpp"

void Environment::envInit() {
  float depth{0};
  // SMALL_WORLD_DEPTH
  // SMALL_WORLD_STRIDE
  for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
    depth = -80 + y * TILE_SZ;
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      int rndBlock = GetRandomValue(0, 100);
      if (depth < 0) {
        // AIR
        Environment::tileInit(x, y, depth, BLANK, ItemType::AIR, CanBlock::NO);
      } else if (depth == 0) {
        // GRASS ENUM
        Environment::tileInit(x, y, depth, GREEN, ItemType::GRASS,
                              CanBlock::YES);
      } else if (depth > 0 && depth <= 20 * TILE_SZ) {
        if (rndBlock >= 0 && rndBlock <= 98) {
          // DIRT ENUM
          Environment::tileInit(x, y, depth, BROWN, ItemType::DIRT,
                                CanBlock::YES);
        } else if (rndBlock > 98 && rndBlock <= 100) {
          // COAL ENUM
          Environment::tileInit(x, y, depth, BLACK, ItemType::COAL,
                                CanBlock::YES);
        }
      } else if (depth > 20 * TILE_SZ) {
        if (rndBlock >= 0 && rndBlock <= 60) {
          // STONE ENUM
          Environment::tileInit(x, y, depth, GRAY, ItemType::STONE,
                                CanBlock::YES);
        } else if (rndBlock > 60 && rndBlock <= 70) {
          // DIRT ENUM
          Environment::tileInit(x, y, depth, BROWN, ItemType::DIRT,
                                CanBlock::YES);
        } else if (rndBlock > 70 && rndBlock <= 90) {
          // COAL ORE
          Environment::tileInit(x, y, depth, BLACK, ItemType::COAL,
                                CanBlock::YES);
        } else if (rndBlock > 90 && rndBlock <= 97) {
          // IRON ENUM
          Environment::tileInit(x, y, depth, GRAY, ItemType::IRON,
                                CanBlock::YES);
        } else if (rndBlock > 97 && rndBlock <= 100) {
          // DIAMOND ORE
          Environment::tileInit(x, y, depth, BLUE, ItemType::DIAMOND,
                                CanBlock::YES);
        }
      }
    }
  }
}

void Environment::treesInit() {
  int pos = 0;
  while (pos < 1000) {
    pos += GetRandomValue(0, TILE_SZ / 2);
    Environment::treeInit(pos, BROWN, CanBlock::NO);
    pos += TILE_SZ;
  }
}

void Environment::tileInit(const int& x, const int& y, const float& depth,
                           const Color& color, const ItemType& it,
                           const CanBlock& cb) {
  EnvTile tile = EventideEngine::tiles_buff[y][x];
  EventideEngine::tiles_buff[y][x].rect =
      (Rectangle){SMALL_WORLD_START + x * TILE_SZ, depth, TILE_SZ, TILE_SZ};
  EventideEngine::tiles_buff[y][x].color = color;
  EventideEngine::tiles_buff[y][x].type = it;
  EventideEngine::tiles_buff[y][x].blocking = cb;
  if (it != ItemType::AIR) {
    EventideEngine::tiles_buff[y][x].texture = EventideEngine::envTextures[it];
  }
}

void Environment::tileUpdate(const int& x, const int& y, const Color& color,
                             const ItemType& it, const CanBlock& cb) {
  EventideEngine::tiles_buff[y][x].color = color;
  EventideEngine::tiles_buff[y][x].type = it;
  EventideEngine::tiles_buff[y][x].blocking = cb;
  if (it != ItemType::AIR) {
    EventideEngine::tiles_buff[y][x].texture = EventideEngine::envTextures[it];
  } else {
    EventideEngine::tiles_buff[y][x].texture = (Texture2D){0};
  }
}

void Environment::treeInit(const int& pos, const Color& color,
                           const CanBlock& cb) {
  EventideEngine::tree_buff[pos].rect =
      (Rectangle){SMALL_WORLD_START + pos * TILE_SZ,
                  (float)GetRandomValue(-10 * TILE_SZ, -9 * TILE_SZ),
                  (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ),
                  (float)GetRandomValue(5 * TILE_SZ, 15 * TILE_SZ)};
  EventideEngine::tree_buff[pos].color = color;

  int rndTree = GetRandomValue(0, 1);
  switch (rndTree) {
    case 0:
      EventideEngine::tree_buff[pos].texture =
          EventideEngine::envTextures[ItemType::OAK_TREE];
      break;
    case 1:
      EventideEngine::tree_buff[pos].texture =
          EventideEngine::envTextures[ItemType::PINE_TREE];
      break;
  }
}
ii Environment::mouseCoords() {
  return ii(EventideEngine::player->pos.x + GetMouseX() - 300,
            EventideEngine::player->pos.y + GetMouseY() - 280);
}

ii Environment::playerIndex() {
  return ii(ceil((EventideEngine::player->pos.x - SMALL_WORLD_START) / TILE_SZ),
            ceil(EventideEngine::player->pos.y / TILE_SZ));
}

bool Environment::canModifyEnv() {
  ii mousePos = Environment::mouseCoords();
  return (
      abs(mousePos.first - (int)EventideEngine::player->pos.x) < 5 * TILE_SZ &&
      abs(mousePos.second - (int)EventideEngine::player->pos.y) < 5 * TILE_SZ);
}

ii Environment::findEnvPos() {
  ii mousePos = Environment::mouseCoords();
  return ii(std::max(0.0, floor((SMALL_WORLD_END + mousePos.first) / TILE_SZ)),
            std::max(0.0, floor((6 * TILE_SZ + mousePos.second) / TILE_SZ)));
}
