/*****************************************************************
**  Name:       testwave.cpp
**	Describe:	test generate wave and calculate wave force for TVC
*   ref:HMCH
**  Project:	TwinsVessselControl(TVC)
**	Author:		Fu Zhenqiu
**  Date:		2020-03-12
******************************************************************/
#include "wave.h"

int main() {
  // save_spec();
  mywave _mywave(5, 9);

  //输出波浪普
  _mywave.save_spec();

  //计算波浪力
  ofstream outFile;
  outFile.open("2ndWaveForce.csv", ios::out);
  // wave_force_RTdata my_data;
  for (int i = 0; i < 400; i++) {
    _mywave.cal_wave_force(1.57, 100);  //遭遇频率和船长
    outFile << i << ',' << _mywave.get_wave_force().wave_fx << ','
            << _mywave.get_wave_force().wave_fy << ','
            << _mywave.get_wave_force().wave_fz << ',' << _mywave.get_wave_amp()
            << endl;
  }

  outFile.close();
}