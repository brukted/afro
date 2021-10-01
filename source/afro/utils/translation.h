/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

//#define _(text) (text)
#define N_(text) text

namespace afro {

template <typename type>
constexpr auto translate(const type text) -> const char* {
  return text;
};

template <typename type>
constexpr auto translate(const type text, const char* /*context*/) -> const char* {
  return text;
};

}  // namespace afro
