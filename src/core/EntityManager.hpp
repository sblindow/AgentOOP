#pragma once

#include <cstdint>
#include <vector>

class EntityManager {
  private:
    std::vector<uint32_t> freeIds;
    std::vector<bool> idState;

  public:
    uint32_t registerEntity();
    void deleteEntity(uint32_t deleteId);

    bool isAlive(uint32_t id) const;
};
