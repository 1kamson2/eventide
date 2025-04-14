#include "Environment/Environment.hpp"

#include <raylib.h>

#include <cassert>
#include <cmath>
#include <cstdint>

#include "Utils/Globals.hpp"
#include "Utils/SimplexNoise.h"
using namespace chunk_definitions;
using namespace env_definitions;
Environment::Environment() {}
void Environment::WorldInit(std::vector<Chunk>& chunks_to_render) {
  uint64_t voxels_currently_rendered = 0, chunks_currently_rendered = 0;
  std::cout << Y_BOUND << std::endl;
  std::cout << X_BOUND << std::endl;
  std::cout << Z_BOUND << std::endl;
  for (float y = 0; y < Y_BOUND; y += SIZE) {
    for (float x = 0; x < X_BOUND; x += SIZE) {
      for (float z = 0; z < Z_BOUND; z += SIZE) {
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
        float elevation = GetElevation(chunk_x, chunk_y, chunk_z, norm, f);
        Color col = AssignColor(elevation, chunk_y);
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
  float nx = x / SIZE - 0.5, ny = y / SIZE - 0.5, nz = z / SIZE - 0.5;
  float fnx = f * nx, fny = f * ny, fnz = f * nz;

  float elevation =
      (f * SimplexNoise::noise(fnx, fny, fnz) +
       0.5f * f *
           SimplexNoise::noise(2.0f * fnx - 4.312f, 2.0f * fny + 6.4312f,
                               2.0f * fnz - 12.231f) +
       0.25f * f *
           SimplexNoise::noise(4.0f * fnx + 7.523f, 4.0f * fny - 9.1231f,
                               4.0f * fnz + 8.538623f)) /
      norm;
  /* Make sure, that we are in the correct range */
  assert(elevation >= -1 && elevation <= 1);
  return elevation >= 0 ? std::pow(elevation, 1.182f)
                        : std::pow(-elevation, 1.182f);
}

Color Environment::AssignColor(const float& elevation, const float& y) const {
  if (elevation < 0.1) {
    return BLANK;
  } else if (elevation < 0.2)
    return BLUE;
  else if (elevation < 0.3)
    return GREEN;
  else if (elevation < 0.5)
    return DARKGREEN;
  else if (elevation < 0.7)
    return GREEN;
  else if (elevation < 0.9)
    return YELLOW;
  else
    return WHITE;
}
