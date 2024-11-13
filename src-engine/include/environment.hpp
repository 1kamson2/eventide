#pragma once
#include "engine.hpp"
#include "resource_manager.hpp"

class Environment {
 public:
  static void envInit();
  static void treesInit();
  static bool canModifyEnv();
  static ii findEnvPos();
  static ii mouseCoords();
  static ii playerIndex();
  static void tileInit(const int& x, const int& y, const float& depth,
                       const Color& color, const ItemType& it,
                       const CanBlock& cb);
  static void treeInit(const int& pos, const Color& color, const CanBlock& cb);
  static void tileUpdate(const int& x, const int& y, const Color& color,
                         const ItemType& it, const CanBlock& cb);

 private:
  Environment() = delete;
  virtual ~Environment() = delete;
};
