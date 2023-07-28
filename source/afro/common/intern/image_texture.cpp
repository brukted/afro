/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "core/image_texture.h"

#include <string_view>
#include <utility>

namespace afro::core {
ImageBuffer::ImageBuffer(ImageTextureFormat format, int width, int height)
    : width(width),
      height(height),
      format(format),
      bytes(std::vector<uint8_t>(width * height * (int)format)) {}

ImageBuffer::ImageBuffer(ImageTextureFormat format, int width, int height,
                         std::vector<uint8_t> bytes)
    : width(width), height(height), format(format), bytes(std::move(bytes)) {}

ImageTexture::ImageTexture(UUID uid, ImageTextureFormat format, int width,
                           int height)
    : uid(uid), buffer(ImageBuffer(format, width, height)) {}

ImageTexture::ImageTexture(UUID uid, ImageBuffer buffer)
    : uid(uid), buffer(std::move(buffer)) {}

// auto ImageTexture::create_from_file(std::string_view /**path**/) ->
// ImageTexture {}

}  // namespace afro::core
