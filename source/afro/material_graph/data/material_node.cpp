/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_node.h"

namespace afro::graph::material {
auto MaterialNode::get_buffer_format() const -> gl::GLenum {
  return gl::GLenum::GL_RGBA;
}
auto MaterialNode::get_buffer_size() const -> IVec2 {
  return afro::IVec2(1024, 1024);
}
auto MaterialNode::get_property(std::string_view prop_id)
    -> property::Property& {
  for (auto& prop : get_properties()) {
    if (prop.get_property_definition().id == prop_id) {
      return prop;
    }
  }
  throw std::runtime_error("Property not found");
}
}  // namespace afro::graph::material