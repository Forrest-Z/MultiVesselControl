/*****************************************************************
**  Name:       winddata.h
**	Describe:	data structure of calculating  wind force for TVC 
**  Project:	TwinsVessselControl(TVC)	    
**	Author:		Fu Zhenqiu						    
**  Date:		2020-03-12							
******************************************************************/

#ifndef _WINDDATA_H_
#define _WINDDATA_H_

struct windRTdata{
      double windS;
      double windD;
      double R_windS;
      double R_windD;
   
};

struct windforceRTdata
{
      double windforce_x;
      double windforce_y;
      double windforce_z;//0
      double windmoment_k;
      double windmoment_m;//0
      double windmoment_n;
};


#endif /*_WINDDATA_H_*/