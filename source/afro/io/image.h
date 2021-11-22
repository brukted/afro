/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "core/image_texture.h"

namespace afro::io {
auto load_image_from_file(std::string_view file_path) -> core::ImageBuffer;

auto load_image_from_memory(std::string_view file_name, void *data, size_t size)
    -> core::ImageBuffer;
}  // namespace afro::io