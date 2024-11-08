#include "engine.hpp"
EventideEngine::EventideEngine(unsigned int width, unsigned int height,
                               unsigned int fps)
    : width{width},
      height{height},
      fps{fps},
      player{new Player},
      State{GameState::ACTIVE},
      textures{
          {ItemType::GRASS, ResourceManager::textureInit(GRASS_TEXTURE, true)},
          {ItemType::STONE, ResourceManager::textureInit(STONE_TEXTURE, true)},
          {ItemType::DIRT, ResourceManager::textureInit(DIRT_TEXTURE, true)},
          {ItemType::OAK_TREE,
           ResourceManager::textureInit(OAK_TEXTURE_PATH, false)},
          {ItemType::PINE_TREE,
           ResourceManager::textureInit(PINE_TEXTURE_PATH, false)}} {
  // make another function to initialize player
  player->pos = (Vector2){SMALL_WORLD_START + 100, -5 * TILE_SZ};
  player->speedY = 0;
  player->canJump = false;
  player->hitbox.rect =
      (Rectangle){player->pos.x, player->pos.y, TILE_SZ, 3 * TILE_SZ};
  player->hitbox.type = ItemType::PLAYER;
  player->hitbox.blocking = CanBlock::YES;
  player->hitbox.color = (Color){RED};
  // health related
  player->health.rect.width = 100;
  player->health.rect.height = 10;
  player->health.color = RED;
  player->health.quantity = 100;

  this->camera = {0};
  camera.target = (Vector2){player->pos.x + 20.0f, player->pos.y + 20.0f};
  camera.offset = (Vector2){this->width / 2.0f, this->height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;
}

void EventideEngine::eventideInit(unsigned int seed) {
  SetRandomSeed(seed);
  float depth{0};

  // SMALL_WORLD_DEPTH
  // SMALL_WORLD_STRIDE
  for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
    depth = -80 + y * TILE_SZ;
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      if (depth < 0) {
        // AIR
        this->tileInit(x, y, depth, BLANK, ItemType::AIR, CanBlock::NO);
      }
      if (depth == 0) {
        // GRASS ENUM
        this->tileInit(x, y, depth, GREEN, ItemType::GRASS, CanBlock::YES);
      } else if (depth > 0 && depth <= 60) {
        // DIRT ENUM
        this->tileInit(x, y, depth, BROWN, ItemType::DIRT, CanBlock::YES);
      } else if (depth > 60) {
        int rndBlock = GetRandomValue(0, 100);
        if (rndBlock >= 0 && rndBlock <= 77) {
          this->tileInit(x, y, depth, GRAY, ItemType::STONE, CanBlock::YES);
        } else if (rndBlock > 77 && rndBlock <= 100) {
          this->tileInit(x, y, depth, GRAY, ItemType::DIRT, CanBlock::YES);
        }
      }
    }
  }
  // maybe unload textures?
  // load trees
  int pos = 0;
  while (pos < 1000) {
    pos += GetRandomValue(0, TILE_SZ / 2);
    this->treeInit(pos, BROWN, CanBlock::NO);
    pos += TILE_SZ;
  }
}

void EventideEngine::tileInit(const int& x, const int& y, const float& depth,
                              const Color& color, const ItemType& it,
                              const CanBlock& cb) {
  this->tiles_buff[y][x].rect =
      (Rectangle){SMALL_WORLD_START + x * TILE_SZ, depth, TILE_SZ, TILE_SZ};
  this->tiles_buff[y][x].color = color;
  this->tiles_buff[y][x].type = it;
  this->tiles_buff[y][x].blocking = cb;
  if (it != ItemType::AIR) {
    this->tiles_buff[y][x].texture = this->textures[it];
  }
}

void EventideEngine::tileUpdate(const int& x, const int& y, const Color& color,
                                const ItemType& it, const CanBlock& cb) {
  this->tiles_buff[y][x].type = it;
  this->tiles_buff[y][x].blocking = cb;
  this->tiles_buff[y][x].color = color;
  if (it != ItemType::AIR) {
    this->tiles_buff[y][x].texture = this->textures[it];
  } else {
    this->tiles_buff[y][x].texture = (Texture2D){0};
  }
}

void EventideEngine::treeInit(const int& pos, const Color& color,
                              const CanBlock& cb) {
  this->tree_buff[pos].rect =
      (Rectangle){SMALL_WORLD_START + pos * TILE_SZ,
                  (float)GetRandomValue(-10 * TILE_SZ, -9 * TILE_SZ),
                  (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ),
                  (float)GetRandomValue(5 * TILE_SZ, 15 * TILE_SZ)};
  this->tree_buff[pos].color = color;
  int rndTree = GetRandomValue(0, 1);
  switch (rndTree) {
    case 0:
      tree_buff[pos].texture = this->textures[ItemType::OAK_TREE];
      break;
    case 1:
      tree_buff[pos].texture = this->textures[ItemType::PINE_TREE];
      break;
  }
}
bool EventideEngine::checkCollision(const EnvTile& tempET) {
  bool collisionX =
      player->pos.x + player->hitbox.rect.width >= tempET.rect.x &&
      tempET.rect.x + tempET.rect.width >= player->pos.x;
  bool collisionY =
      player->pos.y + player->hitbox.rect.height >= tempET.rect.y &&
      tempET.rect.y + tempET.rect.height >= player->pos.y;
  return collisionX && collisionY;
}
bool EventideEngine::canModifyEnv() {
  return (abs(mouseCoords.first - (int)this->player->pos.x) < 5 * TILE_SZ &&
          abs(mouseCoords.second - (int)this->player->pos.y) < 5 * TILE_SZ);
}
ii EventideEngine::findEnvPositions() {
  return ii(
      std::max(0.0, floor((SMALL_WORLD_END + mouseCoords.first) / TILE_SZ)),
      std::max(0.0, floor((96 + mouseCoords.second) / TILE_SZ)));
}
void EventideEngine::processInput(float dt) {
  if (IsKeyDown(KEY_RIGHT)) {
    player->pos.x += PLAYER_SPEED_X * dt;
  } else if (IsKeyDown(KEY_LEFT)) {
    player->pos.x -= PLAYER_SPEED_X * dt;
  } else if (IsKeyDown(KEY_DOWN) && this->player->canGoFaster) {
    player->pos.y += PLAYER_SPEED_Y * dt;
  } else if (IsKeyDown(KEY_UP) && this->player->canJump) {
    player->pos.y = -PLAYER_JUMP;
    player->canJump = false;
  } else if (IsKeyReleased(KEY_TAB)) {
    switch (this->State) {
      case GameState::ACTIVE:
        this->State = GameState::MENU;
        std::cout << "[DEBUG] Game state is MENU" << std::endl;
        break;
      case GameState::MENU:
        this->State = GameState::ACTIVE;
        std::cout << "[DEBUG] Game state is ACTIVE" << std::endl;
        break;
      default:
        this->State = GameState::EXIT;
        break;
    }
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    // bad cast is probable
    if (this->canModifyEnv()) {
      ii pos = this->findEnvPositions();
      if (this->tiles_buff[pos.second][pos.first].type != ItemType::AIR) {
        this->tileUpdate(pos.first, pos.second, BLANK, ItemType::AIR,
                         CanBlock::NO);
        std::cout << "Block destroyed. " << std::endl;
        if (this->removedRecently.size() >= 10) {
          this->removedRecently.clear();
        }
        this->removedRecently.push_back(
            (Vector2){(float)pos.first, (float)pos.second});
      }
    }
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    if (this->canModifyEnv()) {
      ii pos = this->findEnvPositions();
      if (this->tiles_buff[pos.second][pos.first].type == ItemType::AIR) {
        this->tileUpdate(pos.first, pos.second, BROWN, ItemType::DIRT,
                         CanBlock::YES);
        std::cout << "Block placed. " << std::endl;
        if (this->placedRecently.size() >= 10) {
          this->placedRecently.clear();
        }
        this->placedRecently.push_back(
            (Vector2){(float)pos.first, (float)pos.second});
      }
    }
  }
}

void EventideEngine::updatePlayer(float dt) {
  // update health ui
  player->hitbox.rect.y = player->pos.y;
  player->hitbox.rect.x = player->pos.x;
  player->health.rect.x = player->pos.x - 125;
  player->health.rect.y = player->pos.y - 70;
  // update camera
  camera.target = (Vector2){player->pos.x + player->hitbox.rect.width / 2,
                            player->pos.y + player->hitbox.rect.height / 2};
  // update hotbar
  for (int i = 0; i < 9; ++i) {
    this->player->hotbar[i].rect =
        (Rectangle){player->pos.x - 62.5f + 1.2f * TILE_SZ * i,
                    player->pos.y + 125, TILE_SZ, TILE_SZ};
    this->player->hotbar[i].color = WHITE;
  }
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 8; ++j) {
      this->player->inventory[i][j].rect = (Rectangle){
          player->pos.x - 125.0f + 1.2f * TILE_SZ * i,
          player->pos.y - 50.0f + 1.2f * j * TILE_SZ, TILE_SZ, TILE_SZ};
      this->player->inventory[i][j].color = WHITE;
    }
  }

  bool hitObstacle = false;
  for (int y = 0; y < 3 * RENDER_DISTANCE; y++) {
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      EnvTile tempET = this->tiles_buff[y][x];
      Vector2 playerPos = player->pos;
      if (tempET.blocking == CanBlock::YES && this->checkCollision(tempET)) {
        std::cout << "[DEBUG] Collision detected on: " << tempET.rect.x << " "
                  << tempET.rect.y << std::endl;
        hitObstacle = true;
        player->speedY = 0.0f;
        playerPos.y = tempET.rect.y;
        break;
      }
    }
  }
  // jumping works weird...
  if (!hitObstacle) {
    player->speedY = PLAYER_SPEED_Y;
    player->pos.y += this->player->speedY * dt;
    player->canJump = false;
    player->canGoFaster = true;
  } else {
    player->canJump = true;
    player->canGoFaster = false;
  }
}
void EventideEngine::update(float dt) {
  this->processInput(dt);
  this->updatePlayer(dt);
  this->mouseCoords = ii(this->player->pos.x + GetMouseX() - 300,
                         this->player->pos.y + GetMouseY() - 280);
  // this->renderTiles();
  this->playerIndex = ii(ceil((player->pos.x - SMALL_WORLD_START) / TILE_SZ),
                         ceil(player->pos.y / TILE_SZ));
  this->treeRDist =
      ii(std::max((int)(this->playerIndex.first), 0),
         std::min(3 * RENDER_DISTANCE, (int)(this->playerIndex.first)));
}

void EventideEngine::renderTiles() {
  std::cout << "Not implemented, this function will tell how to render tiles"
            << std::endl;
}
void EventideEngine::debug() {
  std::cout << "[DEBUG] SCREEN SIZE [W, H]: " << this->width << " "
            << this->height << std::endl;
  std::cout << "[DEBUG] FPS: " << GetFPS() << std::endl;
  std::cout << "[DEBUG] PLAYER POS [X, Y]: " << this->player->pos.x << " "
            << this->player->pos.y << std::endl;
  std::cout << "[DEBUG] MOUSE POS: " << this->mouseCoords.first << " "
            << this->mouseCoords.second << std::endl;
  std::cout << "[DEBUG] Recently placed blocks: " << std::endl;
  std::cout << "[DEBUG] ARRAY[y][x] REAL COORDS: [x, y] " << std::endl;
  for (const Vector2 el : this->placedRecently) {
    std::cout << "[" << el.x << ", " << el.y << "]";
    std::cout << " [" << el.x * TILE_SZ + SMALL_WORLD_START << ", "
              << el.y * TILE_SZ << "]" << std::endl;
  }
  std::cout << "[DEBUG] Recently removed blocks: " << std::endl;
  std::cout << "[DEBUG] ARRAY[y][x] REAL COORDS: [x, y] " << std::endl;
  for (const Vector2 el : this->removedRecently) {
    std::cout << "[" << el.x << ", " << el.y << "]";
    std::cout << " [" << el.x * TILE_SZ + SMALL_WORLD_START << ", "
              << el.y * TILE_SZ << "]" << std::endl;
  }
}

EventideEngine::~EventideEngine() {
  delete player;
  player = nullptr;
}
