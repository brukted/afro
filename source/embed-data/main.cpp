/**
 * @ingroup Tools
 * @file main.cpp
 * @date 2020-10-11
 * @brief This file contains the source for embed-data tool(executable) which
 * helps to embed files into executable by phrasing them as a cpp source file.
 * Embed-data was inspired by blender's datatoc.
 *
 */

#include <fmt/format.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#include "fmt/core.h"

namespace fs = std::filesystem;
using std::string;
using std::span;

auto main(int argc, char *argv[]) -> int {
  auto args = span(argv, size_t(argc));
  if (argc < 2) {
    exit(1);
  }

  auto in_file(args[1]);
  auto out_file(args[2]);

  uintmax_t size = 0;

  std::ifstream ifs(in_file, std::ios_base::binary | std::ios_base::in);
  if (!ifs.is_open()) {
    std::cout << fmt::format("Unable to open input {}\n", in_file) << std::endl;
  }
  string file_name;
  fs::path in_path(in_file);
  in_path = fs::absolute(in_path);
  size = fs::file_size(in_path);

  file_name = in_path.filename().string();
  std::replace(file_name.begin(), file_name.end(), '.', '_');
  std::replace(file_name.begin(), file_name.end(), '-', '_');

  std::ofstream ofs(out_file);
  if (!ofs.is_open()) {
    std::cout << fmt::format("Unable to open output {}", in_file) << std::endl;
  }
  std::cout << fmt::format("Making source file for {}", in_file) << std::endl;

  ofs << "#include <array>\n\n"
         "#if defined(_MSC_VER)\n"
         "#pragma warning(push )\n"
         "#pragma warning(disable : 4309 )\n"
         "#pragma warning(disable : 4838 )\n"
         "#elif defined(__GNUC__) || defined(__clang__)\n"
         "  _Pragma(GCC diagnostic push)\n"
         "  _Pragma(GCC diagnostic ignored #Wnarrowing)\n"
         "  _Pragma(GCC diagnostic ignored #Wformat-truncation)\n"
         "#endif\n\n";

  ofs << fmt::format("/* embed data of file {} */\n\n", in_file);

  uintmax_t size_with_null_term = size;
  if (*(args[3]) != '0') {
    size_with_null_term = size + 1;
  }

  ofs << fmt::format(
      "extern const int embed_data_{0}_size;\n"
      "extern const char embed_data_{0}[];\n\n",
      file_name.c_str());

  ofs << fmt::format(
             "const int embed_data_{0}_size = {1};\n"
             "const char embed_data_{0}[]",
             file_name.c_str(), (int)size)
      << " = {\n";

  // clang-format off
  while (size-- != 0U) {
    // inserts new line every 32nd character
    if (size % 32 == 31) {  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      ofs << "\n";
    }
    ofs << ifs.get() << ",";
  }
  // clang-format on
  ifs.close();

  // null terminator for string data
  if (*(args[3]) != '0') {
    ofs << "0\n};\n\n";
  } else {
    std::streamoff pos = ofs.tellp();
    ofs.seekp(pos - 1);
    ofs << "\n};\n\n";
  }
  ofs << "#if defined(_MSC_VER)\n"
         "#pragma warning( pop )\n"
         "#elif defined(__GNUC__) || defined(__clang__)\n"
         "_Pragma(GCC diagnostic pop)\n"
         "#endif\n";
  ofs.close();
  return 0;
}