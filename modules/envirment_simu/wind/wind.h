/*****************************************************************
**  Name:       wind.h
**	Describe:	calculate  wind force for TVC ref：HMCH
**  Project:	TwinsVessselControl(TVC)	    
**	Author:		Fu Zhenqiu						    
**  Date:		2020-03-12							
******************************************************************/

#ifndef _WIND_H_
#define _WINDDATA_H_

#include "winddata.h"

class wind
{

 public:
    wind(){
        init_wind();
    };
    ~wind(){};
//设置风速风向，风向为角度
void Wind::setPara( double speedTrue, double angTrue )
{
	V_T = speedTrue * M2KNOT;
	ang_T = angTrue*angToRad;
}

//设置船舶艏向,弧度
void Wind::setHead(const double head)
{
	psi = head;
}

//设置船舶速度
void Wind::setNu(const Nu nuShip)
{
	nu.u = nuShip.u;
	nu.v = nuShip.v;
	nu.w = nuShip.w;
	nu.p = nuShip.p;
	nu.q = nuShip.q;
	nu.r = nuShip.r;
}

    //calculate wind force
    wind& cal_wind(){
   //计算遭遇角度
angEnc = psi - ang_T - PI;
   //计算相对风速
V_R = sqrt(pow(nu.u-V_T*cos(ang_T-psi),2.0)+pow(nu.v-V_T*sin(ang_T-psi),2.0));
   //计算风力系数ABC
	interpA(AA, A, angEnc);
	interpBC(BB, B, angEnc);
	interpBC(CC, C, angEnc);
   //计算风力系数
	Cx = -(A[0]+A[1]*2*A_s/pow(L_oa,2.0)+A[2]*2*A_f/pow(B0,2.0)
		+A[3]*L_oa/B0+A[4]*wind_c/L_oa+A[5]*wind_e/L_oa+A[6]*wind_M );

	Cy = B[0]+B[1]*2*A_s/pow(L_oa,2.0)+B[2]*2*A_f/pow(B0,2.0)
		+B[3]*L_oa/B0+B[4]*wind_c/L_oa+B[5]*wind_e/L_oa+B[6]*A_ss/A_s;

	Cn = C[0]+C[1]*2*A_s/pow(L_oa,2.0)+C[2]*2*A_f/pow(B0,2.0)
		+C[3]*L_oa/B0+C[4]*wind_c/L_oa+C[5]*wind_e/L_oa;
	
   //计算风对船作用
	tao.xForce  = 0.5*rho_a*pow(V_R,2.0)*Cx*A_f;
	tao.yForce  = 0.5*rho_a*pow(V_R,2.0)*Cy*A_s;
	tao.zForce  = 0.0;
	tao.kMoment = 0.5*rho_a*pow(V_R,2.0)*Cy*A_s*10;
	tao.mMoment = 0.0;
	tao.nMoment = 0.5*rho_a*pow(V_R,2.0)*Cn*A_s*L_oa;


    return this;
    }

    windforceRTdata  getwindforceRTdata() const {
        return _windforce;
    }


 private:
    /* data */
    double L_oa;  			//船全长 (m)
	double B0;	            //船宽 (m)
	double H_s;             //船舶在水线以上部分侧面中心到水线距离 (m)
	double A_s;             //水线以上船舶侧投影面积 (m^2)
	double A_f;             //水线以上船舶正投影面积 (m^2)--横剖面投影
	double A_ss;            //（船）上层建筑的侧投影面积 (m^2)
	double wind_c;	        //除吃水线和船桅、通气设备等细长体外的船模型的侧投影周长 (m)
	double wind_e;			//从船首到侧投影面积的矩心的距离 (m)
	double wind_M;          // 正投影面内可见的船桅或中柱的不同组数；不包括紧贴船桥前部的中柱
	double rho_a;           //20 C 的空气密度 kg/m^3
	double V_T;             //绝对风速
	double ang_T;			//绝对风向
	double angEnc;			//遭遇角
	double psi;				//艏向角
	double V_R;				//相对风速

	Nu nu;					//船舶速度
    
    	//xyn方向风压力系数矩阵
	double AA[rowPara][colPara], BB[rowPara][colPara], CC[rowPara][colPara];
	double A[rowPara], B[rowPara], C[rowPara];
	double Cx, Cy, Cn;
    //
    void init(){
         //初始化船体参数
    L_oa   = 175;	
	B0	   = 25.4;   
	H_s    = 10;      
	A_s    = 1700;    
	A_f    = 250;     
	A_ss   = 175;     
	wind_c = 150;	
	wind_e = 80;      
	wind_M = 1;       
	rho_a  = 1.224;   
	V_T    = 15;
	ang_T  = 0.0;
	angEnc = 0.0;
	psi	   = 0.0;
	//----------------------x方向风压力系数矩阵----------------------------------
		double a[] = {
		2.152, -5, 0.243, -0.164, 0, 0, 0, 
		1.714, -3.33, 0.145, -0.121, 0, 0, 0, 
		1.818, -3.97, 0.211, -0.143, 0, 0, 0.033, 
		1.965, -4.81, 0.243, -0.154, 0, 0, 0.041, 
		2.333, -5.99, 0.247, -0.19, 0, 0, 0.042, 
		1.726, -6.54, 0.189, -0.173, 0.348, 0, 0.048, 
		0.913, -4.68, 0, -0.104, 0.482, 0, 0.052, 
		0.457, -2.88, 0, -0.068, 0.346, 0, 0.043, 
		0.341, -0.91, 0, -0.031, 0, 0, 0.032, 
		0.355, 0, 0, 0, -0.247, 0, 0.018, 
		0.601, 0, 0, 0, -0.372, 0, -0.02, 
		0.651, 1.29, 0, 0, -0.582, 0, -0.031, 
		0.564, 2.54, 0, 0, -0.748, 0, -0.024, 
		-0.142, 3.58, 0, 0.047, -0.7, 0, -0.028, 
		-0.677, 3.64, 0, 0.069, -0.529, 0, -0.032, 
		-0.723, 3.14, 0, 0.064, -0.475, 0, -0.032, 
		-2.148, 2.56, 0, 0.081, 0, 1.27, -0.027, 
		-2.707, 3.97, -0.175, 0.126, 0, 1.81, 0, 
		-2.529, 3.76, -0.174, 0.128, 0, 1.55, 0 };
		
	//---------------------------y方向风压力系数矩阵-----------------------------
		double b[] = {
			0, 0, 0, 0, 0, 0, 0, 
				0.096, 0.22, 0, 0, 0, 0, 0, 
				0.176, 0.71, 0, 0, 0, 0, 0, 
				0.225, 1.38, 0, 0.023, 0, -0.29, 0, 
				0.329, 1.82, 0, 0.043, 0, -0.59, 0, 
				1.164, 1.26, 0.121, 0, -0.242, -0.95, 0, 
				1.163, 0.96, 0.101, 0, -0.177, -0.88, 0, 
				0.916, 0.53, 0.069, 0, 0, -0.65, 0, 
				0.844, 0.55, 0.082, 0, 0, -0.54, 0, 
				0.889, 0, 0.138, 0, 0, -0.66, 0, 
				0.799, 0, 0.155, 0, 0, -0.55, 0, 
				0.797, 0, 0.151, 0, 0, -0.55, 0, 
				0.996, 0, 0.184, 0, -0.212, -0.66, 0.34, 
				1.014, 0, 0.191, 0, -0.28, -0.69, 0.44, 
				0.784, 0, 0.166, 0, -0.209, -0.53, 0.38, 
				0.536, 0, 0.176, -0.029, -0.163, 0, 0.27, 
				0.251, 0, 0.106, -0.022, 0, 0, 0, 
				0.125, 0, 0.046, -0.012, 0, 0, 0, 
				0, 0, 0, 0, 0, 0, 0
};
	//-------------------------------风压力矩系数-------------------------------
		double c[] = {
			0, 0, 0, 0, 0, 0, 0, 
			0.0596, 0.061, 0, 0, 0, -0.074, 0, 
			0.1106, 0.204, 0, 0, 0, -0.17, 0, 
			0.2258, 0.245, 0, 0, 0, -0.38, 0, 
			0.2017, 0.457, 0, 0.0067, 0, -0.472, 0, 
			0.1759, 0.573, 0, 0.0118, 0, -0.523, 0, 
			0.1925, 0.48, 0, 0.0115, 0, -0.546, 0, 
			0.2133, 0.315, 0, 0.0081, 0, -0.526, 0, 
			0.1827, 0.254, 0, 0.0053, 0, -0.443, 0, 
			0.2627, 0, 0, 0, 0, -0.508, 0, 
			0.2102, 0, -0.0195, 0, 0.0335, -0.492, 0, 
			0.1567, 0, -0.0258, 0, 0.0497, -0.457, 0, 
			0.0801, 0, -0.0311, 0, 0.074, -0.396, 0, 
			-0.0189, 0, -0.0488, 0.0101, 0.1128, -0.42, 0, 
			0.0256, 0, -0.0422, 0.01, 0.0889, -0.463, 0, 
			0.0552, 0, -0.0381, 0.0109, 0.0689, -0.476, 0, 
			0.0881, 0, -0.0306, 0.0091, 0.0366, -0.415, 0, 
			0.0851, 0, -0.0122, 0.0025, 0, -0.22, 0, 
			0, 0, 0, 0, 0, 0, 0
		};

		for (int i = 0; i < rowPara; i ++)
		{
			for (int j = 0; j < colPara; j ++)
			{
				AA[i][j] = a[i*colPara+j];
				BB[i][j] = b[i*colPara+j];
				CC[i][j] = c[i*colPara+j];
			}
		}
		for (int i = 0; i < colPara; i ++)
		{
			A[i] = 0.0;
			B[i] = 0.0;
			C[i] = 0.0;
		}
		Cx = 0.0;
		Cy = 0.0;
		Cn = 0.0;

   }

   //差值函数 遭遇角为弧度
void interpA( const double valInit[rowPara][colPara], double result[colPara], double ang )
{
	int index = 0;
	ang *= radToAng;
	while (ang > 180.0)
	{
		ang -= 360.0;
	}
	while (ang < -180.0)
	{
		ang += 360.0;
	}
	ang = fabs(ang/10.0);
	index = static_cast<int>(ang);

	for (int i = 0; i < colPara; i ++)
	{
		result[i] = (valInit[index+1][i]-valInit[index][i])*(ang - static_cast<double>(index))
			+ valInit[index][i];
	}
}

//差值函数 遭遇角为弧度
void interpBC( const double valInit[rowPara][colPara], double result[colPara], double ang )
{
	int index = 0;
	double coe = 1.0;
	ang *= radToAng;
	while (ang > 180.0)
	{
		ang -= 360.0;
	}
	while (ang < -180.0)
	{
		ang += 360.0;
	}

	if (0.0 < ang)
	{
		coe = 1.0;
	} else {
		coe = -1.0;
	}

	ang = fabs(ang/10.0);
	index = static_cast<int>(ang);

	for (int i = 0; i < colPara; i ++)
	{
		result[i] = coe*((valInit[index+1][i]-valInit[index][i])*(ang - static_cast<double>(index))
			+ valInit[index][i]);
	}
}

};






#endif /*_WINDDATA_H_*/