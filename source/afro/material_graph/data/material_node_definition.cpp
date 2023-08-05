/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_node_definition.h"

#include "material_graph/engine/material_engine.h"
#include "material_node.h"
#include "utils/assert.h"

namespace afro::graph::material {
// bind static function default value
MaterialNodeExecFun MaterialNodeDefinition::def_exec_fun =
    [](MaterialEngine* engine, MaterialGraph* graph, MaterialNode* node) {
      auto processor = engine->create_or_get_processor(node->get_definition());
      std::optional<property::Property*> output_prop;
      auto links = graph->get_links_to_node(node->get_uuid());

      auto bind_input_property = [&](property::Property& prop) {
        if (prop.get_property_definition().is_socket) {
          // Socket properties
          auto link =
              std::find_if(links.begin(), links.end(), [&prop](Link& link) {
                return link.get_to_property() == prop.get_uuid();
              });
          if (link != links.end()) {
            auto& buffer = engine->get_buffer(link->get_from_property());
            processor->set_texture(prop.get_property_definition().id,
                                   buffer.texture_id);
          } else {
            // TODO: Generate a 1x1 texture with the default value or prop value
          }
        } else {
          // Props that begin with _ are common properties
          if (prop.get_property_definition().id[0] != '_') {
            // Non-socket properties
            processor->set_prop(prop);
          }
        }
      };

      // Bind properties
      for (auto& prop : node->get_properties()) {
        switch (prop.get_property_definition().type) {
          case property::Type::INPUT:
            bind_input_property(prop);
            break;
          case property::Type::OUTPUT: {
            AF_ASSERT_MSG(!output_prop.has_value(),
                          "Node has multiple output properties")
            output_prop = &prop;
            break;
          }
          default:
            break;
        }
      }

      if (output_prop.has_value()) {
        auto& buffer = engine->create_or_get_buffer(
            output_prop.value()->get_uuid(), node->get_buffer_size().x,
            node->get_buffer_size().y, node->get_buffer_format());
        auto buffer_size = node->get_buffer_size();
        processor->execute(buffer.frame_buffer_id, buffer.texture_id,
                           buffer_size.x, buffer_size.y);
      }
    };
}  // namespace afro::graph::material