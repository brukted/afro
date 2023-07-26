#pragma once

#include <cstdint>

namespace afro::core {

using UUID = uint64_t;

auto generate_uuid() -> UUID;

}  // namespace afro::core
