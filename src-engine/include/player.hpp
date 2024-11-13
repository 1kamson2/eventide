#pragma once
#include "resource_manager.hpp"
class PlayerObject {
 public:
  Texture2D texture;
  mutable Vector2 pos, oldpos;
  mutable EnvTile hitbox, hotbar[9], inventory[3][8], health;
  mutable float speedY, speedX;
  mutable int lastPressed;
  mutable bool canJump, canGoFaster, eastCollision, westCollision;
  PlayerObject();
  virtual ~PlayerObject();
  void updatePlayerState(float dt);
  bool checkCollision(const EnvTile& tempET);
};
