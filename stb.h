#pragma once

#include <Eigen/Dense>
#include <ctre.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace stb {

struct Equil {
  Eigen::VectorXd lgk;
  Eigen::MatrixXd matrix;
};

Equil readEquil(const std::string &filename) {
  std::ifstream ifs(filename);
  std::map<std::string, std::vector<std::string>> stb_blocks;
  std::string line;

  while (std::getline(ifs, line)) {
    if (auto m = ctre::match<"^\\s+\\$(\\w+)\\s+$">(line)) {
      std::string block = m.get<1>().to_string();
      while (std::getline(ifs, line) &&
             !ctre::match<"^\\s+\\$end\\s+$">(line)) {
        stb_blocks[block].push_back(line);
      }
    }
  }

  Equil res;

  if (auto it = stb_blocks.find("matrix"); it != stb_blocks.end()) {
    bool is_matrix_read = false;
    unsigned basis = -1;
    size_t size = it->second.size();
    Eigen::MatrixXd &matrix = res.matrix;
    Eigen::VectorXd &lnk = res.lgk;
    for (const auto &l : it->second) {
      if (!is_matrix_read) {
        --size;
        if (auto m = ctre::match<"^\\s*(\\w+)=(\\d+)\\s+$">(l)) {
          if (m.get<1>() == "basis") {
            basis = std::stoi(m.get<2>().to_string());
          }
        }
        else if (auto m = ctre::match<"^\\s*([A-Z]\\s+)+$">(l)) {
          is_matrix_read = true;
          matrix.resize(size, basis);
          lnk.resize(size);
          size = 0;
          continue;
        }
      }
      else {
        std::stringstream ss(l);
        for (unsigned i = 0; i < basis; ++i) {
          ss >> matrix.coeffRef(size, i);
        }
        ss >> lnk(size++);
      }
    }
  }

  return res;
}

} // namespace stb
