/*****************************************************************
**  Name:       mywave.h
**	Describe:	generate wave and calculate wave force for TVC ref£ºHMCH
**  Project:	TwinsVessselControl(TVC)
**	Author:		Fu Zhenqiu
**  Date:		2020-03-12
******************************************************************/

#ifndef _MY_WAVE_
#define _MY_WAVE_
#include <math.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

//   //保存数据
//   ofstream outFile;
//   outFile.open("data.csv", ios::out);
//   outFile << omega << ',' << spec_save_tmp << endl;
//   outFile.close();

struct wave_force_RTdata {
  double wave_fx;
  double wave_fy;
  double wave_fz;
};

class mywave {
 public:
  mywave(double _Hs, double _T_0)
      : Hs(_Hs),
        T_0(_T_0){
            //
        };
  ~mywave(){};
  wave_force_RTdata _wave_force;
  void cal_wave_force(double wave_orientation, double _ship_length,
                      double vessel_speed_u, double vessel_speed_v,
                      double vessel_orientation) {
    enFre = vessel_orientation - wave_orientation;  //遭遇角度
    L = _ship_length;
    delta_omega = (max_omega - min_omega) / N_omega;
    //不规则
    // for (double omega = min_omega; omega < max_omega; omega += delta_omega) {
    //   psi = rand() % 100 * 0.0628;  //产生0-2pi的随机数
    //   //
    //   A_i = sqrt(2 * delta_omega * ITTC_Spec(Hs, T_0, omega));
    //   //波幅
    //   // amp_wave += A_i * sin(omega * time_count + psi);
    //   amp_wave += A_i;
    //   //波幅平方
    //   amp_wave_2 = amp_wave * amp_wave;
    //   //波长
    //   lambda = 9.81 * 6.28 / (omega * omega);
    //   Cx = 0.05 - 0.2 * (lambda / L) + 0.75 * pow((lambda / L), 2) -
    //        0.51 * pow((lambda / L), 3);
    //   Cy = 0.46 + 6.83 * (lambda / L) - 15.65 * pow((lambda / L), 2) +
    //        8.44 * pow((lambda / L), 3);
    //   Cz = -0.11 + 0.68 * (lambda / L) - 0.79 * pow((lambda / L), 2) +
    //        0.21 * pow((lambda / L), 3);  //波浪力
    //   // tmp_x += Cx * A_i * A_i;
    //   // tmp_y += Cy * A_i * A_i;
    //   // tmp_z += Cz * A_i * A_i;
    // }

    //规则波
    for (double omega = min_omega; omega < max_omega; omega += delta_omega) {
      A_i = sqrt(2 * delta_omega * ITTC_Spec(Hs, T_0, omega));
      amp_wave += A_i;
      //波幅平方
      amp_wave_2 = amp_wave * amp_wave;
    }

    lambda = 9.81 * T_0 * T_0 / (4 * 3.1415926);
    Cx = 0.05 - 0.2 * (lambda / L) + 0.75 * pow((lambda / L), 2) -
         0.51 * pow((lambda / L), 3);
    Cy = 0.46 + 6.83 * (lambda / L) - 15.65 * pow((lambda / L), 2) +
         8.44 * pow((lambda / L), 3);
    Cz = -0.11 + 0.68 * (lambda / L) - 0.79 * pow((lambda / L), 2) +
         0.21 * pow((lambda / L), 3);  //波浪力

    // std::cout << "*****************************" << std::endl;
    FX = 0.5 * rho * 9.81 * L * amp_wave_2 * cos(enFre);
    FY = 0.5 * rho * 9.81 * L * amp_wave_2 * sin(enFre);
    FZ = 0.5 * rho * 9.81 * L * L * amp_wave_2 * sin(enFre);
    _wave_force.wave_fx = FX;
    _wave_force.wave_fy = FY;
    _wave_force.wave_fz = FZ;
    //
    amp_wave_output = amp_wave;
    amp_wave_2 = 0;
    amp_wave = 0;
    Cx = 0;
    Cy = 0;
    Cz = 0;
    tmp_x = 0;
    tmp_y = 0;
    tmp_z = 0;
    time_count += 1;
    if (time_count > all_time) time_count = 1;
  }

  void save_spec() {
    std::vector<double> spec_save;
    std::vector<double> omega_save;
    delta_omega = (max_omega - min_omega) / N_omega;
    ofstream outFile;
    outFile.open("ITTC_Spec.csv", ios::out);
    for (double omega = min_omega; omega < max_omega;
         omega += delta_omega / 2) {
      double spec_save_tmp = ITTC_Spec(Hs, T_0, omega);
      spec_save.push_back(spec_save_tmp);
      outFile << omega << ',' << spec_save_tmp << endl;
    }
    outFile.close();
  }

  wave_force_RTdata get_wave_force() { return _wave_force; }
  double get_wave_amp() { return amp_wave_output; }

 private:
  //需要确定的参数
  double Hs = 0;                          // ITTC 有意波高
  double T_0 = 0;                         // ITTC 普风周期To
  int all_time = 200;                     //波浪仿真时间长度
  int N_omega = 30;                       //叠加个数
  double L = 1;                           //船长
  double rho = 1025;                      //海水密度
  double min_omega = 0.1, max_omega = 7;  //截至频率
  //内部
  double A_i = 0;
  double amp_wave = 0, amp_wave_2 = 0, amp_wave_output = 0;  //波浪幅值
  double tmp_x = 0, tmp_y = 0, tmp_z = 0;
  double lambda = 0;   //波长，gT2/2pi //波长 g2pi/w^2
  double enFre = 1.7;  //遭遇频率
  int time_count = 1;
  double psi = 0;  //随机数
  double delta_omega;
  //波浪力系数
  double Cx = 0, Cy = 0, Cz = 0;
  double FX = 0;
  double FY = 0;
  double FZ = 0;

  double ITTC_Spec(double Hs, double T0,
                   double omega) {  // ITTC-Modified Pierson-Moskowitz (Hs,T0)
    double A, B, Sw;
    A = 487.0 * pow(Hs, 2.0) / pow(T0, 4.0);
    B = 1949 / pow(T0, 4.0);

    Sw = A / (pow(omega, 5) * exp(B / (pow(omega, 4))));

    return Sw;
  }
};
#endif /*_MY_WAVE_*/



