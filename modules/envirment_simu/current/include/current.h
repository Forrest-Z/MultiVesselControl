/*
***********************************************************************
* current.h: compute the current load on USV using wind
* coefficient
* This header file can be read by C++ compilers

***********************************************************************
*/

#ifndef _CURRENT_H_
#define _CURRENT_H_

#include <cmath>
#include <common/math/eigen/Eigen/Core>
#include <common/math/eigen/Eigen/Dense>

class current {
 public:
  current(const Eigen::Vector3d &_currentloadRTdata)
      : water_rho(1025),
        C_L(0.04),
        C_D(0.05),
        C_N(0.02),
        ATC(0.2),
        ALS(0.8),
        T(0.2),  //吃水
        LOA(3.2),
        currentloadRTdata(_currentloadRTdata){};
  ~current(){};

  current &computecurrentload(double raw_windspeed, double raw_windorientation,
                              double vessel_speed_u, double vessel_speed_v,
                              double vessel_orientation) {
    //转成相对风速风向
    double u_w =
        raw_windspeed * std::cos(raw_windorientation - vessel_orientation);
    double v_w =
        raw_windspeed * std::sin(raw_windorientation - vessel_orientation);
    // double u = vessel_speed * std::cos(vessel_orientation);
    // double v = vessel_speed * std::sin(vessel_orientation);
    double u_rw = vessel_speed_u - u_w, v_rw = vessel_speed_v - v_w;
    double V_rw = std::sqrt(u_rw * u_rw + v_rw * v_rw);
    double gamma_rw = -std::atan2(v_rw, u_rw);

    // double a_windorientation = orientation_lowpass.movingaverage(gamma_rw);
    // double a_windspeed = speed_lowpass.movingaverage(V_rw);

    computecurrentforce(V_rw, gamma_rw, currentloadRTdata(0),
                        currentloadRTdata(1), currentloadRTdata(2));
    // if (SealoadRTdata.windstatus == WINDCOMPENSATION::WINDON)
    //   computewindforce(a_windspeed, a_windorientation,
    //                    SealoadRTdata.windload(0), SealoadRTdata.windload(1),
    //                    SealoadRTdata.windload(2));
    // else
    //   SealoadRTdata.windload.setZero();
    return *this;
  }
  auto getcurrentloadRTdata() const noexcept { return currentloadRTdata; }

 private:
  const double water_rho;  // water density
  const double C_L;        // 船体阻力系数
  const double C_D;        // 升力系数
  const double C_N;        // 转矩系数
                           //
  // C L 、 C D 、 C N 分别是船体阻力系数、升力系数以及转矩系数
  const double ATC;  // 船舶水下部分横向投影面积
  const double ALS;  // 船舶水下部分纵向投影面积
  const double T;    // 吃水深度
  const double LOA;  // 船长，其实应该垂线间长Lbp
  Eigen::Vector3d currentloadRTdata;

  // compute CURRENT load on vessel using relative CURRENT load speed and
  // orientation
  void computecurrentforce(double current_speed, double current_orientation,
                           double &Fx, double &Fy, double &Mz) {
    // double Cx = C_L * std::sin(current_orientation) -
    //             C_D * std::cos(current_orientation);  // x方向的风力系数
    // double Cy = C_L * std::cos(current_orientation) +
    //             C_D * std::sin(current_orientation);  // y方向的风力系数
    // double Cn = C_N;
    double Cx = -C_L * std::cos(current_orientation);  // x方向的风力系数
    double Cy = C_D * std::sin(current_orientation);   // y方向的风力系数
    double Cn = C_N * std::sin(2 * current_orientation);

    double temp_force = 0.5 * water_rho * std::pow(current_speed, 2);
    Fx = temp_force * Cx * ATC ;  // Fx
    Fy = temp_force * Cy * ALS ;  // Fy
    Mz = temp_force * Cn * ALS *LOA;

  }    // computeCURRENTload
};     // end CURRENT
#endif /*_CURRENT_H_*/

