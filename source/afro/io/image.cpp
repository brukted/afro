/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "image.h"

#include <OpenImageIO/filesystem.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/imageio.h>
#include <fmt/format.h>

#include <array>

#include "core/image_texture.h"

namespace afro::io {
using namespace core;
auto load_image_from_file(std::string_view file_path) -> ImageBuffer {
  // TODO Implement
  return ImageBuffer(ImageTextureFormat::rgba8, 0, 0);
}

auto load_image_from_memory(std::string_view file_name, void *data, size_t size) -> ImageBuffer {
  // TODO Implement
  return ImageBuffer(ImageTextureFormat::rgba8, 0, 0);
}

}  // namespace afro::io
