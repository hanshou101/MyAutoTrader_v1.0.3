/////////////////////////////////////////////////////////////////////////
///名称：AutoTrader
///版本：v1.0.x
///日期：2014xxxx
///说明：基于上期CTP技术
///作者：小平常心
///  QQ：1396144618
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <iomanip>
#include <vector>
#include <math.h>
#include <windows.h>
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"
#include "DataSniffer_A.h"
#include "DataSniffer_B.h"


#pragma warning(disable : 4996)

#define EPSILON_E4 (float)(1E-2) 
#define EPSILON_E5 (float)(1E-3)

extern	int		RunMode;
extern double	tick_data[20][10];				//TICK基本数据
extern	bool	CloseAll;					//收盘标志

 
void Sniffer()	//监听Tick数据已经指标计算 实盘用
{
	double GetLocalTimeSec1();
	string dbtoch(double nums) ;

	void Sniffer_A12(int i);
	void Sniffer_B12(int i);
	//void Sniffer_C12(int i);
	//void Sniffer_D12(int i);

	void WriteMdConfiguration();
	void ErasingTradeConfiguration();

	double Nowtime=GetLocalTimeSec1();

	if (RunMode && (tick_data[18][2]>=0.151459 || tick_data[3][2]>=0.145959) && Nowtime>0.1518 && Nowtime<0.1520 && CloseAll==false)
	{	
		cerr << "--->>> " <<dbtoch(tick_data[1][1])<<"准备收盘!" << endl;
		cerr << "--->>> " <<"WriteMdConfiguration!" << endl;
		WriteMdConfiguration();				//备份数据
		Sleep(3000);
		ErasingTradeConfiguration();
		cerr << "--->>> " <<dbtoch(tick_data[1][1])<<"收盘!" << endl;
		CloseAll=true;
	}

	for (int i = 0; i < 20; i++)
	{
		if(RunMode)
		{

			if ( fabs(tick_data[i][0]-1)<0.01 && ((tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i<=17)) )//i>17合约为IF
			{
				Sniffer_A12(i);		
				Sniffer_B12(i);
				//Sniffer_C12(i);
				//Sniffer_D12(i);

				tick_data[i][0]=0;
			}
		}
		else
		{

			if(fabs(tick_data[i][0]-1)<0.01)
			{
				Sniffer_A12(i);					
				Sniffer_B12(i);
				//Sniffer_C12(i);
				//Sniffer_D12(i);

				tick_data[i][0]=0;
			}
		}



	}

}

