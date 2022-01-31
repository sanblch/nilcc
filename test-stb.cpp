#include "conc.h"
#include "matplotlibcpp/matplotlibcpp.h"
#include "matrix.h"
#include "stb.h"

#include <backward.hpp>
#include <toml++/toml.h>

#include <filesystem>
#include <span>

namespace plt = matplotlibcpp;

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

  auto basis_str = tbl["basis"];
  if (const toml::array *arr = basis_str.as_array()) {
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
  for (const auto &el : data) {
    mdata.insert_data(el);
  }

  auto additional = tbl["additional"];
  struct BisComplex {
    std::string ligand;
    unsigned col;
    std::vector<std::pair<unsigned, unsigned>> row;
  };
  std::vector<BisComplex> complexes;
  if (toml::array *arr = additional.as_array()) {
    for (const auto &el : *arr) {
      auto ligand = el.value<std::string>().value();
      auto it = std::ranges::find(mdata.basis, ligand);
      auto col = std::distance(mdata.basis.begin(), it);
      std::vector<std::pair<unsigned, unsigned>> row;
      Eigen::VectorXd vec = Eigen::VectorXd::Zero(mdata.basis.size());
      vec(0) = 1;
      vec(col) = 2;
      for (unsigned i = 0; i < mdata.matrix.rows(); ++i) {
        if (mdata.matrix.row(i).transpose().head(mdata.basis.size()) == vec) {
          row.push_back({i, mdata.matrix(i, Eigen::last)});
        }
      }
      if (row.size()) {
        complexes.push_back({ligand, col, row});
        std::cout << repr(*complexes.rbegin()) << std::endl;
      }
    }
  }

  // std::cout << mdata.matrix << std::endl;
  // std::cout << mdata.lgk << std::endl;

  auto basis = mdata.matrix.cols() - 1;
  Eigen::MatrixXd B;
  B.setConstant(1, basis, 0.1);
  B(0, 0) = 0.001;
  B(0, 1) = 0.001;
  Eigen::VectorXd b;
  b.setConstant(basis, -1);

  Sysc sysc;
  sysc.concAlg = ConcAlg::BRINKLEY;
  // sysc.verb = true;

  std::vector<double> x;
  x.push_back(1.0);
  while (x[x.size() - 1] < 14)
    x.push_back(x[x.size() - 1] + 0.5);
  Map<VectorXd> h(x.data(), x.size());

  MatrixXd A(x.size(), mdata.matrix.rows());
  for (unsigned i = 0; i < x.size(); ++i) {
    std::cout << "************************* " << x[i]
              << " *******************************" << std::endl;
    VectorXd h(1);
    h.coeffRef(0) = std::log(std::pow(10.0, -x[i]));
    A.row(i) = nfconc(mdata.matrix, mdata.lgk, B, h, b, sysc);
  }

  // std::cout << A << std::endl;

  MatrixXd S = A;

  for (unsigned i = 0; i < A.cols(); ++i) {
    if (mdata.matrix(i, 0) != 0) {
      S.col(i) = mdata.matrix(i, 0) * A.col(i) / B(0, 0);
    }
    else {
      S.col(i).setZero();
    }
  }

  plt::figure_size(1200, 700);
  for (unsigned i = 0; i < S.cols(); ++i) {
    plt::plot(x, std::span(S.col(i).data(), S.rows()));
  }
  plt::save("./result.png");
}
