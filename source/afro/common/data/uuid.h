#pragma once

#include <cstdint>

namespace afro {

using UUID = uint64_t;

[[nodiscard]] auto generate_uuid() -> UUID;

}  // namespace afro
