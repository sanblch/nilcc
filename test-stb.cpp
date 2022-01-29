#include "stb.h"

#include <backward.hpp>
#include <repr/repr.h>
#include <toml++/toml.h>

#include <algorithm>
#include <filesystem>
#include <unordered_set>

struct StbData {
  std::string path;
  stb::Equil equilibria;
  std::vector<std::string> basis;
};

struct MainData {
  std::vector<std::string> basis;
  Eigen::MatrixXi matrix;

  void insert_basis(const std::string& el) {
    if(std::ranges::find(basis, el) == basis.end()) {
      basis.push_back(el);
    }
  }

  void insert_data(const StbData& data) {
    if(matrix.rows() == 0 || matrix.cols() == 0) {
      matrix = Eigen::MatrixXi::Zero(data.equilibria.matrix.rows(), basis.size() + 1);
      matrix(Eigen::all, insert_indexes(data.basis)) = data.equilibria.matrix;
    } else {
      std::vector<unsigned> unmatched;
      std::vector<unsigned> row_indexes;
      unsigned count = matrix.rows();
      auto indexes = insert_indexes(data.basis);
      for(unsigned i = 0; i < data.equilibria.matrix.rows(); ++i) {
        Eigen::VectorXi vec = Eigen::VectorXi::Zero(basis.size() + 1);
        vec(indexes) = data.equilibria.matrix.row(i);
        bool found = false;
        for(unsigned j = 0; j < matrix.rows(); ++j) {
          if(matrix.row(j).transpose() == vec) {
            found = true;
            break;
          }
        }
        if (!found) {
          unmatched.push_back(i);
          row_indexes.push_back(count++);
        }
      }
      if(unmatched.size() > 0) {
        std::cout << unmatched.size() << std::endl;
        matrix.conservativeResize(matrix.rows() + unmatched.size(), basis.size() + 1);
        matrix(row_indexes, Eigen::all) = Eigen::MatrixXi::Zero(row_indexes.size(), basis.size() + 1);
        matrix(row_indexes, indexes) = data.equilibria.matrix(unmatched, Eigen::all);
      }
    }
    std::cout << repr(insert_indexes(data.basis)) << std::endl;
  }

  std::vector<unsigned> insert_indexes(const std::vector<std::string>& b) {
    std::vector<unsigned> res;
    for(const auto& el : b) {
      auto it = std::ranges::find(basis, el);
      auto index = std::distance(basis.begin(), it);
      res.push_back(index);
    }
    res.push_back(basis.size());
    return res;
  }
};

int main(int argc, char *argv[]) {
  backward::SignalHandling sh;

  toml::table tbl;
  std::vector<StbData> data;
  MainData mdata;
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
          mdata.insert_basis(str);
        }
        count++;
      }
    }
  }

  std::cout << repr(mdata.basis) << std::endl;

  for (const auto& el : data) {
    mdata.insert_data(el);
  }

  std::cout << mdata.matrix << std::endl;
}
