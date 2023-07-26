#pragma once

#include <fruit/fruit.h>

#include "store/data/data.h"
#include "ui/interfaces/widget.h"

namespace afro::store {
class Outliner : public ui::Widget {
 private:
  Data *data;
  bool show = true;
  auto main_context_menu() -> void;

 public:
  INJECT(Outliner(Data *data)) : data(data) {}
  virtual auto draw() -> void override;
  virtual ~Outliner() = default;
};
}  // namespace afro::store
