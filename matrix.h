#pragma once

#include "stb.h"

#include <Eigen/Dense>
#include <repr/repr.h>

#include <algorithm>
#include <vector>

struct StbData {
  std::string path;
  stb::Equil equilibria;
  std::vector<std::string> basis;
};

struct MainData {
  std::vector<std::string> basis;
  Eigen::VectorXd lgk;
  Eigen::MatrixXd matrix;

  void insert_basis(const std::string& el) {
    if(std::ranges::find(basis, el) == basis.end()) {
      basis.push_back(el);
    }
  }

  void insert_data(const StbData& data) {
    if(matrix.rows() == 0 || matrix.cols() == 0) {
      matrix = Eigen::MatrixXd::Zero(data.equilibria.matrix.rows(), basis.size() + 1);
      matrix(Eigen::all, insert_indexes(data.basis)) = data.equilibria.matrix;
      lgk = data.equilibria.lgk;
    } else {
      std::vector<unsigned> unmatched;
      std::vector<unsigned> row_indexes;
      unsigned count = matrix.rows();
      auto indexes = insert_indexes(data.basis);
      for(unsigned i = 0; i < data.equilibria.matrix.rows(); ++i) {
        Eigen::VectorXd vec = Eigen::VectorXd::Zero(basis.size() + 1);
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
        matrix(row_indexes, Eigen::all) = Eigen::MatrixXd::Zero(unmatched.size(), basis.size() + 1);
        matrix(row_indexes, indexes) = data.equilibria.matrix(unmatched, Eigen::all);

        lgk.conservativeResize(lgk.size() + unmatched.size());
        lgk(row_indexes) = Eigen::VectorXd::Zero(unmatched.size());
        lgk(row_indexes) = data.equilibria.lgk(unmatched);
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
