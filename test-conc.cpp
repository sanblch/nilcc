#include "conc.h"
#include <iostream>

struct ConcData {
  VectorXd lnk;
  MatrixXd B, matrix;
  ConcData() {
    int basis = 2;
    int species = 11;
    B.resize(1, basis);
    lnk.resize(species);
    matrix.resize(species, basis + 1);
    B << 0.00481452, 0.01000039;
    lnk << 0, 0, -4.5729, -18.9986, -40.1893, -6.9054, -17.0345, -16.1181,
        -24.9370, -38.9666, -65.7388;
    // clang-format off
    matrix << 1, 0, 0,
              0, 1, 0,
              0, 1, -1,
              0, 1, -2,
              0, 1, -3,
              1, 1, -2,
              1, 1, -3,
              1, 2, -4,
              1, 2, -5,
              1, 2, -6,
              1, 2, -7;
    // clang-format on
  }
} data;

int main() {
  Sysc sysc;
  sysc.concAlg = ConcAlg::BUGAEVSKY;
  sysc.verb = true;

  double h = 1.0;
  while(h < 14) {
    std::cout << "************************* " << h
              << " *******************************" << std::endl;
    nfconc(data.matrix, data.lnk, data.B, std::exp(std::pow(10.0, -h)), sysc);
    h += 0.5;
  }
}
