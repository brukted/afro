/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include <array>

// clang-format-off

#define EMBEDDED_DATA(file_name)              \
  extern const int embed_data##_##file_name##_##size; \
  extern const char embed_data##_##file_name[]; /*NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)*/

// clang-format-on
