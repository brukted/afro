/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>

#include "common/interfaces/command.h"
#include "graph/data/graph.h"
#include "graph/data/link.h"

namespace afro::graph {

class DeleteLinks : public Command {
 private:
  std::shared_ptr<Graph> graph_;
  std::vector<Link> links_;

 public:
  DeleteLinks(std::shared_ptr<Graph> graph, std::vector<Link> links)
      : Command("DELETE_LINKS"),
        graph_(std::move(graph)),
        links_(std::move(links)) {}

  auto execute() -> void override { graph_->remove_links(links_); }
  auto undo() -> void override { graph_->add_links(links_); }
  ~DeleteLinks() override = default;
};

}  // namespace afro::graph