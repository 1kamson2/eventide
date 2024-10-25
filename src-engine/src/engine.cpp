#include "engine.hpp"

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
  std::ofstream world_conf{"world-conf.txt"};
  if (!world_conf) {
    std::cerr << "Uh oh, Sample.txt could not be opened for writing!\n";
    exit(-1);
  }
  SetRandomSeed(seed);
  for (unsigned int chunkX = 0; chunkX < SMALL_WORLD_WIDTH; ++chunkX) {
    for (unsigned int chunkY = 0; chunkY < SMALL_WORLD_DEPTH; ++chunkY) {
      std::cout << "Currently generating chunk: #" << chunkX << ", " << chunkY
                << std::endl;
      for (unsigned int y = 0; y < CHUNK_SZ; ++y) {
        for (unsigned int x = 0; x < CHUNK_SZ; ++x) {
          // this is function to access it later.
          int currentdepth = x + y * CHUNK_SZ + chunkY * CHUNK_SZ * CHUNK_SZ +
                             SMALL_WORLD_STRIDE * chunkX;
          if (currentdepth > 0 && currentdepth < 60) {
            world_conf << GRASS;
          } else if (currentdepth > 60 && currentdepth < 120) {
            world_conf << DIRT;
          } else if (currentdepth > 120) {
            world_conf << STONE;
          }
        }
        world_conf << '\n';
      }
    }
  }
  int idx = 0;
  while (idx < 1000) {
    // real position is -6000 + idx * TILE_SZ
    // add checking terrain height on given index, then create trees.
    idx += GetRandomValue(0, TILE_SZ);

    wseed->trees[idx].width = (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ);
    wseed->trees[idx].height = (float)GetRandomValue(5 * TILE_SZ, 15 * TILE_SZ);
    wseed->trees[idx].y = 2 * TILE_SZ + wseed->trees[idx].height;
    wseed->trees[idx].x = SMALL_WORLD_START + idx * TILE_SZ;
    wseed->treeColors[idx] =
        (Color){(unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200), 255};
    // add atleast 2 * TILE_SZ to next
    idx += 2 * TILE_SZ;
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
}
int EventideEngine::getPlayerIndex() const {
  return floor((player->hitbox.x - SMALL_WORLD_START) / TILE_SZ);
}
ii EventideEngine::renderTrees() const {
  // cap to i > 0 and i < 1000, -6000 is start of the world, 1000 because a 1000
  // trees
  int currIdx = this->getPlayerIndex();
  return ii(std::max((int)(currIdx - RENDER_DISTANCE / 2), 0),
            std::min(1000, (int)(currIdx + RENDER_DISTANCE / 2)));
}
// those functions are wrong
ii EventideEngine::renderTiles() const {
  int currIdx = this->getPlayerIndex();

  int chunkStart = currIdx - (currIdx % SMALL_WORLD_STRIDE);
  int chunkEnd =
      chunkStart + SMALL_WORLD_STRIDE;  // End rendering after the player

  // Clamp the indices to valid ranges
  chunkStart = std::max(0, chunkStart);
  chunkEnd =
      std::min(SMALL_WORLD_END,
               chunkEnd);  // <-- i guess? meant to be the last index of buffer

  return ii(chunkStart, chunkEnd);
}

EventideEngine::~EventideEngine() {
  delete player;
  delete wseed;
  player = nullptr;
  wseed = nullptr;
}
