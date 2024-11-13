#include "player.hpp"

#include "engine.hpp"
#include "resource_manager.hpp"
// pos should be dependant on world type
PlayerObject::PlayerObject()
    : texture{ResourceManager::textureInit(PLAYER_TEXTURE, 3 * TILE_SZ,
                                           TILE_SZ)},
      // pos{(Vector2){(float)GetRandomValue(-2048, 2048), -5 * TILE_SZ}},
      pos{(Vector2){-1900, -5 * TILE_SZ}},
      oldpos{pos},
      lastPressed{0},
      speedX{0},
      speedY{0},
      canJump{false},
      canGoFaster{false},
      eastCollision{false},
      westCollision{false} {
  this->hitbox.rect =
      (Rectangle){this->pos.x, this->pos.y, TILE_SZ, 3 * TILE_SZ};
  this->hitbox.type = ItemType::PLAYER;
  this->hitbox.blocking = CanBlock::YES;
  this->hitbox.color = RED;
  this->health.rect.width = 100;
  this->health.rect.height = 10;
  this->health.color = RED;
  this->health.quantity = 100;
}

bool PlayerObject::checkCollision(const EnvTile& tempET) {
  bool collisionX = this->pos.x + this->hitbox.rect.width >= tempET.rect.x &&
                    tempET.rect.x + tempET.rect.width >= this->pos.x;
  bool collisionY = this->pos.y + this->hitbox.rect.height >= tempET.rect.y &&
                    tempET.rect.y + tempET.rect.height >= this->pos.y;
  return collisionY && collisionX;
}

void PlayerObject::updatePlayerState(float dt) {
  this->oldpos = pos;
  bool hitObstacle = false;
  float closestGroundY = SMALL_WORLD_END;  // Initialize to world bottom

  // First pass: Find closest ground collision
  for (int y = 0; y < 3 * RENDER_DISTANCE; y++) {
    for (int x = 0; x < 3 * RENDER_DISTANCE; x++) {
      EnvTile& tempET = EventideEngine::tiles_buff[y][x];
      if (tempET.blocking != CanBlock::YES) continue;

      // Check if we're within horizontal bounds
      bool collisionX = pos.x + hitbox.rect.width >= tempET.rect.x &&
                        tempET.rect.x + tempET.rect.width >= pos.x;

      if (collisionX) {
        // Only check tiles above current position or closest found ground
        if (tempET.rect.y < closestGroundY &&
            tempET.rect.y > pos.y - hitbox.rect.height) {
          // Check if this would be a valid ground tile
          bool collisionY = pos.y + hitbox.rect.height >= tempET.rect.y &&
                            tempET.rect.y + tempET.rect.height >= pos.y;

          if (collisionY) {
            closestGroundY = tempET.rect.y;
            hitObstacle = true;
          }
        }
      }
    }
  }

  // Second pass: Handle wall collisions and final position
  bool westCollision = false;
  bool eastCollision = false;

  for (int y = 0; y < 3 * RENDER_DISTANCE; y++) {
    for (int x = 0; x < 3 * RENDER_DISTANCE; x++) {
      EnvTile& tempET = EventideEngine::tiles_buff[y][x];
      if (tempET.blocking != CanBlock::YES) continue;

      // Check horizontal collisions for walls
      if (pos.y + hitbox.rect.height > tempET.rect.y + tempET.rect.height) {
        if (pos.x - tempET.rect.x - tempET.rect.width < 0) {
          westCollision = true;
        }
        if (tempET.rect.x - pos.x - hitbox.rect.width < 0) {
          eastCollision = true;
        }
      }
    }
  }

  if (!hitObstacle) {
    // Adjust player's vertical speed based on gravity or jumping state
    this->speedY = PLAYER_SPEED_Y;  // You can tweak this value
    this->pos.y += this->speedY * dt;
    this->canJump = false;
    this->canGoFaster = true;
  } else {
    // When hitting the ground or an obstacle, stop downward movement
    this->pos.y = closestGroundY - hitbox.rect.height;
    this->speedY = 0.0f;
    this->canJump = true;
    this->canGoFaster = false;
  }

  /*
  // Apply collision response
  if (hitObstacle) {
    // Snap to closest ground
      } else {
    // Apply gravity
    speedY = PLAYER_SPEED_Y;
    pos.y += speedY * dt;
    canJump = false;
    canGoFaster = true;
  }
 */
  // Update collision flags
  this->westCollision = westCollision;
  this->eastCollision = eastCollision;

  // Update hitbox position
  hitbox.rect.x = pos.x;
  hitbox.rect.y = pos.y;

  // Update camera if within bounds
  if (pos.x > SMALL_WORLD_START + 10 * TILE_SZ &&
      pos.x < SMALL_WORLD_END - 10 * TILE_SZ) {
    EventideEngine::camera.target = {pos.x + hitbox.rect.width / 2,
                                     pos.y + hitbox.rect.height / 2};
  }

  // Update UI elements position
}
PlayerObject::~PlayerObject() {};
