#pragma once

#include <Eigen/Dense>

#include <vector>

struct Matrix {
  using Eigen::MatrixXd;
  using Eigen::VectorXi;
  static VectorXi submatrixRowIndex(const MatrixXd &matrix,
                                    const VectorXi &vec) {
    VectorXd r = VectorXd::Ones(matrix.cols());
    r(vec) = VectorXd::Zero(vec.size());
    VectorXi index(matrix.rows());
    int count = 0;
    for (int i = 0; i < matrix.rows(); ++i)
      if (0 == matrix.row(i).dot(r))
        index(count++) = i;
    index.conservativeResize(count);
    return index;
  }

  static MatrixXd submatrix(const MatrixXd &matrix, const VectorXi &vec) {
    return matrix(submatrixRowIndex(matrix, vec), vec);
  }

  static std::vector<int> nums(const MatrixXd &matrix, const VectorXi &mlh,
                               BASIS b) {
    int count = 0;
    std::vector<int> nums;
    for (auto i : mlh) {
      if (i == b) {
        VectorXi vec({{count, static_cast<int>(matrix.cols() - 1)}});
        nums.push_back(Matrix::submatrixRowIndex(matrix, vec).rows());
      }
      count++;
    }
    return nums;
  }
};
