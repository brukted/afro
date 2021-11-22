#include "undo/undo.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <string>

#include "undo/operator.h"

using namespace afro::core;

struct UndoMock : Operator {
  std::string& buf;
  int u;
  UndoMock(std::string& output, int i) : Operator("Mock", "Mock"), buf(output), u(i) {}
  void undo() override {
    std::for_each(buf.begin(), buf.end(), [this](char& i) { i -= u; });
  }
  void redo() override {
    std::for_each(buf.begin(), buf.end(), [this](char& i) { i += u; });
  }
  void execute() override {
    std::for_each(buf.begin(), buf.end(), [this](char& i) { i += u; });
  }
};

TEST(Undo, complex) {
  constexpr int max_undo = 100;
  afro::core::UndoStack undo{max_undo};
  std::string o = {"Hello World"};
  auto op = std::unique_ptr<Operator>(new UndoMock(o, 1));
  auto op1 = std::unique_ptr<Operator>(new UndoMock(o, -1));
  op->execute();
  undo.push_operation(std::move(op));
  undo.undo();
  EXPECT_TRUE(o == "Hello World");
  undo.redo();
  EXPECT_TRUE(o == "Ifmmp!Xpsme");
  undo.undo();
  op1->execute();
  undo.push_operation(std::move(op1));
  undo.undo();
  EXPECT_TRUE(o == "Hello World");
}