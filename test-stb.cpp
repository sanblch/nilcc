#include "conc.h"
#include "matplotlibcpp/matplotlibcpp.h"
#include "matrix.h"
#include "stb.h"

#include <backward.hpp>
#include <toml++/toml.h>
#include <xlnt/xlnt.hpp>

#include <filesystem>
#include <span>

namespace plt = matplotlibcpp;

const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVYWXYZ";

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
    unsigned row;
    unsigned h;
    double k;
  };
  std::vector<BisComplex> complexes;
  if (toml::array *arr = additional.as_array()) {
    for (const auto &el : *arr) {
      auto ligand = el.value<std::string>().value();
      auto it = std::ranges::find(mdata.basis, ligand);
      auto col = std::distance(mdata.basis.begin(), it);
      Eigen::VectorXd vec = Eigen::VectorXd::Zero(mdata.basis.size());
      vec(0) = 1;
      vec(col) = 2;
      for (unsigned i = 0; i < mdata.matrix.rows(); ++i) {
        if (mdata.matrix.row(i).transpose().head(mdata.basis.size()) == vec) {
          unsigned h = mdata.matrix(i, Eigen::last);
          if (h == 4) {
            complexes.push_back({ligand, col, i, h, mdata.lgk(i)});
            std::cout << repr(*complexes.rbegin()) << std::endl;
          }
          else if (h == 6 && ligand == "His") {
            complexes.push_back({"His", col, i, h, mdata.lgk(i)});
            std::cout << repr(*complexes.rbegin()) << std::endl;
          }
        }
      }
    }
  }

  unsigned index = mdata.matrix.rows();
  std::cout << "Generated forms start from: " << index << std::endl;
  unsigned n = complexes.size() * (complexes.size() - 1) / 2 - 1;
  mdata.matrix.conservativeResize(mdata.matrix.rows() + n, Eigen::NoChange);
  mdata.lgk.conservativeResize(mdata.lgk.size() + n);
  for (unsigned i = 0; i < complexes.size(); ++i) {
    for (unsigned j = i + 1; j < complexes.size(); ++j) {
      if (complexes[i].ligand == complexes[j].ligand) {
        continue;
      }
      mdata.matrix.row(index).setZero();
      mdata.matrix(index, 0) = 1;
      mdata.matrix(index, complexes[i].col) = 1;
      mdata.matrix(index, complexes[j].col) = 1;
      unsigned h = mdata.matrix(complexes[i].row, Eigen::last) / 2 +
                   mdata.matrix(complexes[j].row, Eigen::last) / 2;
      mdata.matrix(index, mdata.basis.size()) = h;
      double k =
          (mdata.lgk(complexes[i].row) + mdata.lgk(complexes[j].row)) / 2 + 0.3;
      mdata.lgk(index) = k;
      index++;
    }
  }

  mdata.display();

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
  plt::legend();
  plt::save("./result.png");

  xlnt::workbook wb;
  xlnt::worksheet ws = wb.active_sheet();
  std::vector<std::string> basis_comp;
  for (const auto &el : mdata.basis) {
    basis_comp.push_back(el);
    if (el == "His") {
      basis_comp.push_back("HisH-1");
    }
  }
  for (unsigned i = 0; i < basis_comp.size(); ++i) {
    ws.cell(std::string(1, alphabet[i]) + "1").value(basis_comp[i]);
    ws.cell("A" + std::to_string(i + 1)).value(basis_comp[i]);
  }
  auto it = std::ranges::find(x, 6.0);
  auto ph6 = std::distance(x.begin(), it);
  for (unsigned i = 1; i < basis_comp.size(); ++i) {
    auto i_basis_comp = basis_comp[i] == "HisH-1" ? "His" : basis_comp[i];
    unsigned i_index = mdata.index(i_basis_comp);
    unsigned i_h = basis_comp[i] == "HisH-1" ? 3 : 2;
    for (unsigned j = i; j < basis_comp.size(); ++j) {
      auto j_basis_comp = basis_comp[j] == "HisH-1" ? "His" : basis_comp[j];
      unsigned j_index = mdata.index(j_basis_comp);
      unsigned j_h = basis_comp[j] == "HisH-1" ? 3 : 2;
      for (unsigned k = 0; k < mdata.matrix.rows(); ++k) {
        Eigen::VectorXd vec = Eigen::VectorXd::Zero(mdata.basis.size() + 1);
        vec(0) = 1;
        ++vec(i_index);
        ++vec(j_index);
        vec(mdata.basis.size()) = i_h + j_h;
        if (mdata.matrix.row(k).transpose() == vec) {
          std::cout << std::string(1, alphabet[j]) + std::to_string(i + 1)
                    << ": " << S(ph6, k) << std::endl;
          std::string cell_index =
              std::string(1, alphabet[j]) + std::to_string(i + 1);
          ws.cell(cell_index).value(std::to_string(S(ph6, k)));
        }
      }
    }
  }
  for (unsigned i = 0; i < mdata.matrix.rows(); ++i) {
    std::cout << i << ": " << S(ph6, i) << std::endl;
  }
  wb.save("./result.xlsx");
}
