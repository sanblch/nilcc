#include "conc.h"
#include <cmath>
#include <iostream>
#include <climits>

MatrixXd jconc(const VectorXd &A, const MatrixXd &matrix, int basis) {
  MatrixXd J;
  J.resize(basis, basis);
  for (int i = 0; i < basis; ++i)
    for (int j = i; j < basis; ++j) {
      J.coeffRef(i, j) = A.dot(matrix.col(i).cwiseProduct(matrix.col(j)));
      if (i != j)
        J.coeffRef(j, i) = J.coeff(i, j);
    }
  return J;
}

VectorXd brinkley(const MatrixXd &matrix, const VectorXd &lnk,
                  const VectorXd &B, double H, const Sysc &sysc) {
  VectorXd b(matrix.cols());
  VectorXd A;
  b << -5, -5, H;
  int count = 0;
  while (count++ < sysc.maxiter) {
    A = (matrix * b + lnk).array().exp().matrix();
    VectorXd r = B - matrix.leftCols(B.size()).transpose() * A;
    double sse = r.dot(r);
    if (sysc.verb) {
      std::cout << count - 1 << ' ' << sse << std::endl;
    }
    if(sse < sysc.tolfun)
      break;
    MatrixXd J = jconc(A, matrix, matrix.cols() - 1);
    b.head(matrix.cols() - 1) += J.ldlt().solve(r);
  }
  return A;
}

VectorXd bugaevsky(const MatrixXd &matrix, const VectorXd &lnk,
                   const VectorXd &B, double H, const Sysc &sysc) {
  VectorXd b(matrix.cols());
  b << -5, -5, H;
  VectorXd A = matrix * b + lnk;
  int count = 0;
  while (count++ < sysc.maxiter) {
    VectorXd r = B - matrix.leftCols(B.size()).transpose() * A.array().exp().matrix();
    double sse = r.dot(r);
    if (sysc.verb) {
      std::cout << count - 1 << ' ' << sse << std::endl;
    }
    if(sse < sysc.tolfun)
      break;
    MatrixXd J = jconc(A.array().exp().matrix(), matrix, matrix.cols() - 1);
    VectorXd A1 = A + matrix.leftCols(B.size()) * J.ldlt().solve(r);
    MatrixXd J1 = jconc(A1.array().exp().matrix(), matrix, matrix.cols());
    A = lnk +
        matrix * J1.ldlt().solve(
                     matrix.transpose() *
                     (A1.array().exp() * (A1.array() - lnk.array())).matrix());
  }
  return A;
}

MatrixXd nfconc(const MatrixXd &matrix, const VectorXd &lnk, const MatrixXd &B,
                double H, const Sysc &sysc) {
  MatrixXd A = MatrixXd::Zero(B.rows(), matrix.rows());
  for (int i = 0; i < B.rows(); ++i) {
    if (sysc.concAlg == BUGAEVSKY) {
      A.row(i) =
        bugaevsky(matrix, lnk, B.row(i), H, sysc).array().exp().matrix();
    }
    else if (sysc.concAlg == BRINKLEY) {
      A.row(i) = brinkley(matrix, lnk, B.row(i), H, sysc);
    }
  }
  return A;
}
