/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <core/uuid.h>

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/image_texture.h"
#include "packed_file.h"

namespace afro::core {

/**
 * @brief Enumeration of image format. Can be casted to size_t to get the size
 * of each pixel in bytes.
 *
 */
enum class ImageTextureFormat : size_t {
  r8 = 1,
  rgb8 = 3,
  rgba8 = 4,
  r16 = 2,
  rgb16 = 6,
  rgba16 = 12,
  r16f = 2,
  rgb16f = 6,
  rgba16f = 12,
  r32f = 4,
  rgb32f = 12,
  rgba32f = 16
};

struct ImageBuffer {
  const int width, height;
  const ImageTextureFormat format;
  const std::vector<uint8_t> bytes;
  ImageBuffer(ImageTextureFormat format, int width, int height);
  ImageBuffer(ImageTextureFormat format, int width, int height,
              std::vector<uint8_t> bytes);
  ImageBuffer(ImageBuffer&& other) noexcept = default;
  ImageBuffer(const ImageBuffer& other) noexcept = default;
  auto operator=(ImageBuffer&& other) = delete;
  auto operator=(const ImageBuffer& other) = delete;
  ~ImageBuffer() = default;
};

struct ImageTexture {
  const UUID uid;
  // attrs
  std::string identifier{};
  ImageBuffer buffer;
  std::optional<PackedFile> packed_file;
  // Path to the original file
  std::optional<std::string> file_path;
  ImageTexture(UUID uid, ImageTextureFormat format, int width, int height);
  ImageTexture(UUID uid, ImageBuffer buffer);
  ImageTexture(ImageTexture&& other) = default;
  auto operator=(ImageTexture&&) -> ImageTexture& = delete;
  auto operator=(ImageTexture&) -> ImageTexture = delete;
  ImageTexture(ImageTexture&) = delete;
  ~ImageTexture() = default;
  // static auto create_from_file(std::string_view path) -> ImageTexture;
};

}  // namespace afro::core
