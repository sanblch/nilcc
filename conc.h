#pragma once

#include <Eigen/Dense>

using namespace Eigen;

enum ConcAlg { BRINKLEY, BUGAEVSKY };

struct Sysc {
  bool conseq{true};
  bool verb{false};
  ConcAlg concAlg{BRINKLEY};
  int maxiter{500};
  double tolfun{1e-30};
};

MatrixXd nfconc(const MatrixXd &matrix,
                const VectorXd &lnk,
                const MatrixXd &basis,
                const VectorXd &H,
                const VectorXd &lgc,
                const Sysc &sysc);
