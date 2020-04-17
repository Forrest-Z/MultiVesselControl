#include <iostream>

#include "../include/current.h"

int main() {
  Eigen::Vector3d currentloadRTdata = Eigen::Vector3d::Zero();
  current _currentcompensation(currentloadRTdata);
  for (int i = 0; i < 100; i++) {
    currentloadRTdata =
        _currentcompensation.computecurrentload(2, 1.57, 0.2, 0.3, 1)
            .getcurrentloadRTdata();
    std::cout << currentloadRTdata(0) << "  " << currentloadRTdata(1) << "  "
              << currentloadRTdata(2) << std::endl;
  }
}
