#include "engine.hpp"

#include <raylib.h>

#include <iostream>

#include "resource_manager.hpp"

EventideEngine::EventideEngine(unsigned int width, unsigned int height,
                               unsigned int fps)
    : width{width},
      height{height},
      fps{fps},
      player{new Player},
      State{GameState::GAME_ACTIVE} {
  player->pos = (Vector2){SMALL_WORLD_START + 100, -5 * TILE_SZ};
  player->speed = 0;
  player->canJump = false;
  player->hitbox.rect =
      (Rectangle){player->pos.x, player->pos.y, TILE_SZ, 3 * TILE_SZ};
  player->hitbox.type = ItemType::PLAYER;
  player->hitbox.blocking = CanBlock::YES;
  player->hitbox.color = (Color){RED};
  this->camera = {0};
  camera.target = (Vector2){player->pos.x + 20.0f, player->pos.y + 20.0f};
  camera.offset = (Vector2){this->width / 2.0f, this->height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;
}

void EventideEngine::eventideInit(unsigned int seed) {
  SetRandomSeed(seed);
  float depth{0};
  this->grassTexture = ResourceManager::textureInit(GRASS_TEXTURE, true);
  this->stoneTexture = ResourceManager::textureInit(STONE_TEXTURE, true);
  this->dirtTexture = ResourceManager::textureInit(DIRT_TEXTURE, true);
  this->pineTexture = ResourceManager::textureInit(PINE_TEXTURE_PATH, false);
  this->oakTexture = ResourceManager::textureInit(OAK_TEXTURE_PATH, false);

  // SMALL_WORLD_DEPTH
  // SMALL_WORLD_STRIDE
  for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
    depth = -80 + y * TILE_SZ;
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      if (depth < 0) {
        this->tiles_buff[y][x].rect = (Rectangle){
            SMALL_WORLD_START + x * TILE_SZ, depth, TILE_SZ, TILE_SZ};
        this->tiles_buff[y][x].type = ItemType::AIR;
        this->tiles_buff[y][x].blocking = CanBlock::NO;
      }
      if (depth == 0) {
        // GRASS ENUM
        this->tiles_buff[y][x].rect = (Rectangle){
            SMALL_WORLD_START + x * TILE_SZ, depth, TILE_SZ, TILE_SZ};
        this->tiles_buff[y][x].color = (Color){GREEN};
        this->tiles_buff[y][x].type = ItemType::GRASS;
        this->tiles_buff[y][x].blocking = CanBlock::YES;
        this->tiles_buff[y][x].texture = grassTexture;
      } else if (depth > 0 && depth <= 60) {
        // DIRT ENUM
        this->tiles_buff[y][x].rect = (Rectangle){
            SMALL_WORLD_START + x * TILE_SZ, depth, TILE_SZ, TILE_SZ};
        this->tiles_buff[y][x].color = (Color){BROWN};
        this->tiles_buff[y][x].type = ItemType::DIRT;
        this->tiles_buff[y][x].blocking = CanBlock::YES;
        this->tiles_buff[y][x].texture = dirtTexture;
      } else if (depth > 60) {
        int isStone = GetRandomValue(0, 100);
        this->tiles_buff[y][x].rect = (Rectangle){
            SMALL_WORLD_START + x * TILE_SZ, depth, TILE_SZ, TILE_SZ};
        this->tiles_buff[y][x].blocking = CanBlock::YES;
        if (isStone >= 0 && isStone <= 77) {
          this->tiles_buff[y][x].color = (Color){GRAY};
          this->tiles_buff[y][x].type = ItemType::STONE;
          this->tiles_buff[y][x].texture = stoneTexture;
        } else if (isStone > 77 && isStone <= 100) {
          this->tiles_buff[y][x].color = (Color){BROWN};
          this->tiles_buff[y][x].type = ItemType::GRASS;
          this->tiles_buff[y][x].texture = dirtTexture;
        }
      }
    }
  }
  // maybe unload textures?
  // load trees
  int pos = 0;
  while (pos < 1000) {
    pos += GetRandomValue(0, TILE_SZ);
    // change later according to tree
    tree_buff[pos].rect.width = (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ);
    tree_buff[pos].rect.height =
        (float)GetRandomValue(5 * TILE_SZ, 15 * TILE_SZ);
    tree_buff[pos].rect.y = -20 * TILE_SZ;
    tree_buff[pos].rect.x = SMALL_WORLD_START + pos * TILE_SZ;
    tree_buff[pos].color =
        (Color){(unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200), 255};
    int isOak = GetRandomValue(0, 1);
    switch (isOak) {
      case 0:
        tree_buff[pos].texture = oakTexture;
        break;
      case 1:
        tree_buff[pos].texture = pineTexture;
        ;
        break;
    }
    pos += 2 * TILE_SZ;
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

void EventideEngine::updatePlayer(float dt) {
  if (IsKeyDown(KEY_RIGHT)) {
    player->pos.x += PLAYER_HORIZONTAL_SPEED;
  } else if (IsKeyDown(KEY_LEFT)) {
    player->pos.x -= PLAYER_HORIZONTAL_SPEED;
  } else if (IsKeyDown(KEY_DOWN) && this->player->canGoFaster) {
    player->pos.y += PLAYER_VERTICAL_SPEED * dt;
  } else if (IsKeyDown(KEY_UP) && this->player->canJump) {
    player->pos.y -= PLAYER_VERTICAL_SPEED;
    player->canJump = false;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    // bad cast is probable
    if (abs(mouseCoords.first - (int)this->player->pos.x) < 5 * TILE_SZ &&
        abs(mouseCoords.second - (int)this->player->pos.y) < 5 * TILE_SZ) {
      int arrX =
          std::max(0.0, floor((SMALL_WORLD_END + mouseCoords.first) / TILE_SZ));
      int arrY = std::max(0.0, floor((96 + mouseCoords.second) / TILE_SZ));
      if (this->tiles_buff[arrY][arrX].type != ItemType::AIR) {
        this->tiles_buff[arrY][arrX].type = ItemType::AIR;
        this->tiles_buff[arrY][arrX].blocking = CanBlock::NO;
        this->tiles_buff[arrY][arrX].color = (Color){WHITE};
        this->tiles_buff[arrY][arrX].texture = {0};
        std::cout << "Block destroyed. " << std::endl;
        if (this->removedRecently.size() >= 10) {
          this->removedRecently.clear();
        }
        this->removedRecently.push_back((Vector2){(float)arrX, (float)arrY});
      }
    }
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    if (abs(mouseCoords.first - (int)this->player->pos.x) < 5 * TILE_SZ &&
        abs(mouseCoords.second - (int)this->player->pos.y) < 5 * TILE_SZ) {
      int arrX =
          std::max(0.0, floor((SMALL_WORLD_END + mouseCoords.first) / TILE_SZ));
      int arrY = std::max(0.0, floor((96 + mouseCoords.second) / TILE_SZ));

      if (this->tiles_buff[arrY][arrX].type == ItemType::AIR) {
        this->tiles_buff[arrY][arrX].type = ItemType::DIRT;
        this->tiles_buff[arrY][arrX].blocking = CanBlock::YES;
        this->tiles_buff[arrY][arrX].color = (Color){BROWN};
        this->tiles_buff[arrY][arrX].texture =
            this->dirtTexture;  // <-- change it to engine texture.
        std::cout << "Block placed. " << std::endl;
        if (this->placedRecently.size() >= 10) {
          this->placedRecently.clear();
        }
        this->placedRecently.push_back((Vector2){(float)arrX, (float)arrY});
      }
    }
  }
  player->hitbox.rect.y = player->pos.y;
  player->hitbox.rect.x = player->pos.x;
  camera.target = (Vector2){player->pos.x + player->hitbox.rect.width / 2,
                            player->pos.y + player->hitbox.rect.height / 2};
  bool hitObstacle = false;
  for (int y = 0; y < 3 * RENDER_DISTANCE; y++) {
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      EnvTile tempET = this->tiles_buff[y][x];
      Vector2 playerPos = player->pos;
      if (tempET.blocking == CanBlock::YES && this->checkCollision(tempET)) {
        std::cout << "[DEBUG] Collision detected on: " << tempET.rect.x << " "
                  << tempET.rect.y << std::endl;
        hitObstacle = true;
        player->speed = 0.0f;
        playerPos.y = tempET.rect.y;
        break;
      }
    }
  }
  // jumping works weird...
  if (!hitObstacle) {
    player->speed = PLAYER_VERTICAL_SPEED;
    player->pos.y += this->player->speed * dt;
    player->canJump = false;
    player->canGoFaster = true;
  } else {
    player->canJump = true;
    player->canGoFaster = false;
  }
}
void EventideEngine::update(float dt) {
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
