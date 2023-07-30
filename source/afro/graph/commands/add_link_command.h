/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>

#include "common/interfaces/command.h"
#include "graph/data/graph.h"

namespace afro::graph {

class AddLinkCommand : public Command {
 private:
  std::shared_ptr<Graph> graph_;
  Link link;

 public:
  AddLinkCommand(std::shared_ptr<Graph> graph, Link link)
      : Command("ADD_LINK"), graph_(std::move(graph)), link(std::move(link)) {}

  auto execute() -> void override { graph_->add_link(link); }
  auto undo() -> void override { graph_->remove_link(link); }
  ~AddLinkCommand() override = default;
};

}  // namespace afro::graph