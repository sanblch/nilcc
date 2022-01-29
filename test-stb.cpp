#include "stb.h"

#include <toml++/toml.h>

#include <filesystem>
#include <unordered_set>

struct StbData {
  std::string path;
  stb::Equil equilibria;
  std::vector<std::string> basis;
};

int main(int argc, char *argv[]) {
  toml::table tbl;
  std::vector<StbData> data;
  std::unordered_set<std::string> total_basis;
  try {
    tbl = toml::parse_file(argv[1]);
  }
  catch (const toml::parse_error &err) {
    std::cerr << "Error parsing file '" << *err.source().path << "':\n"
              << err.description() << "\n  (" << err.source().begin << ")\n";
    return 1;
  }
  auto files = tbl["files"];
  std::filesystem::path p = argv[1];
  if (toml::array *arr = files.as_array()) {
    for (const auto &el : *arr) {
      std::cout << el.value<std::string>().value() << std::endl;
      auto path = p.parent_path() / el.value<std::string>().value();
      data.push_back({path, stb::readEquil(path)});
    }
  }

  auto basis = tbl["basis"];
  if (const toml::array *arr = basis.as_array()) {
    unsigned count = 0;
    for (const auto &el : *arr) {
      if (const toml::array *arr2 = el.as_array()) {
        for (const auto &el2 : *arr2) {
          auto str = el2.value<std::string>().value();
          data[count].basis.push_back(str);
          total_basis.insert(str);
        }
        count++;
      }
    }
  }

  std::cout << total_basis << std::endl;
}
