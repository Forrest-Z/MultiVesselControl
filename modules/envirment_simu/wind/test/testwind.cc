#include <iostream>

#include "../include/windcompensation.h"
using namespace ASV;
int main() {
  Eigen::Vector3d windloadRTdata = Eigen::Vector3d::Zero();
  ASV::localization::windcompensation _windcompensation(windloadRTdata);
  for (int i = 0; i < 100; i++) {
    windloadRTdata = _windcompensation.computewindload(2, 1.57, 0.2, 0.3, 1)
                         .getsealoadRTdata();
    std::cout << windloadRTdata(0) << "  " << windloadRTdata(1) << "  "
              << windloadRTdata(2) << std::endl;
  }
}
