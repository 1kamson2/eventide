#include "Environment/Environment.hpp"

#include <raylib.h>

#include <cstdint>

#include "Utils/Globals.hpp"
#include "Utils/SimplexNoise.h"
using namespace chunk_definitions;
Environment::Environment() {}
void Environment::WorldInit(std::vector<Chunk>& chunks_to_render) {
  uint64_t voxels_currently_rendered = 0, chunks_currently_rendered = 0;
  constexpr float f = 1.0f, norm = 1.75f, max_chunks = 128;
  for (float y = 0; y < SIZE; y += SIZE) {
    for (float x = 0; x < 8 * SIZE; x += SIZE) {
      for (float z = 0; z < 8 * SIZE; z += SIZE) {
        if (chunks_currently_rendered >= max_chunks) {
          std::cout << "[INFO] Rendered the following:" << std::endl;
          std::cout << "       Voxels: " << voxels_currently_rendered
                    << std::endl;
          std::cout << "       Chunks: " << chunks_currently_rendered
                    << std::endl;
          return;
        }
        ChunkRender(chunks_to_render, f, norm, x, y, z);
        ++chunks_currently_rendered;
        voxels_currently_rendered += SIZE * SIZE * SIZE;
      }
    }
  }
}

void Environment::ChunkRender(std::vector<Chunk>& chunks_to_render,
                              const float& f, const float& norm, const int& x,
                              const int& y, const int& z) {
  Vector3 mid_point = (Vector3){x + SIZE / 2, y + SIZE / 2, z + SIZE / 2};
  Chunk current_chunk(mid_point);
  for (float chunk_x = x; chunk_x < x + SIZE; ++chunk_x) {
    for (float chunk_y = y; chunk_y < y + SIZE; ++chunk_y) {
      for (float chunk_z = z; chunk_z < z + SIZE; ++chunk_z) {
        float elevation = GetElevation(chunk_x, chunk_y, chunk_z, f, norm);
        Color col = GetColor(elevation);
        Vector3 position_in_chunk = (Vector3){chunk_x, -chunk_y, chunk_z};
        std::shared_ptr<Voxel> vox_node =
            std::make_shared<Voxel>(position_in_chunk, col);
        current_chunk.Update(vox_node);
      }
    }
  }
  chunks_to_render.push_back(current_chunk);
}

float Environment::GetElevation(const float& x, const float& y, const float& z,
                                const float& norm, const float& f) {
  float nx = x / SIZE, ny = y / SIZE, nz = z / SIZE;
  float fnx = f * nx, fny = f * ny, fnz = f * nz;

  float elevation =
      (f * SimplexNoise::noise(fnx, fny, fnz) +
       0.5f * f * SimplexNoise::noise(2.0f * fnx, 2.0f * fny, 2.0f * fnz) +
       0.25f * f * SimplexNoise::noise(4.0f * fnx, 4.0f * fny, 4.0f * fnz)) /
      norm;
  return elevation;
}

Color Environment::AssignColor(const float& elevation) const {
  return elevation >= 0.4 ? RED : BLANK;
}
