#include "uuid.h"

#include <random>

namespace afro {

[[maybe_unused]] auto generate_uuid() -> UUID {
  static std::random_device device;
  static std::mt19937_64 engine(device());
  static std::uniform_int_distribution<uint64_t> dist;
  return dist(engine);
}

}  // namespace afro