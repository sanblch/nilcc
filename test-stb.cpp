#include "conc.h"
#include "matrix.h"
#include "stb.h"

#include <backward.hpp>
#include <toml++/toml.h>

#include <filesystem>

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

  std::cout << mdata.matrix << std::endl;
  std::cout << mdata.lgk << std::endl;

  auto basis = mdata.matrix.cols() - 1;
  Eigen::MatrixXd B;
  B.setConstant(1, basis, 0.005);
  Eigen::VectorXd b;
  b.setConstant(basis, -1);

  Sysc sysc;
  sysc.concAlg = ConcAlg::BRINKLEY;
  sysc.verb = true;

  std::vector<double> x;
  x.push_back(1.0);
  while (x[x.size() - 1] < 14)
    x.push_back(x[x.size() - 1] + 0.5);
  Map<VectorXd> h(x.data(), x.size());

  for (unsigned i = 0; i < x.size(); ++i) {
    std::cout << "************************* " << x[i]
              << " *******************************" << std::endl;
    VectorXd h(1);
    h.coeffRef(0) = std::log(std::pow(10.0, -x[i]));
    std::cout << nfconc(mdata.matrix, mdata.lgk, B, h, b, sysc) << std::endl;
  }
}
