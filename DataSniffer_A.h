/////////////////////////////////////////////////////////////////////////
///���ƣ�AutoTrader
///�汾��v1.0.x
///���ڣ�2014xxxx
///˵������������CTP����
///���ߣ�Сƽ����
///  QQ��1396144618
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

#pragma warning(disable : 4996)

#define EPSILON_E4 (float)(1E-2) 
#define EPSILON_E5 (float)(1E-3)

extern	double  Mn_open[20][60];	//����K�߿�
extern	double  Mn_high[20][60];	//����K�߸�
extern	double  Mn_low[20][60];		//����K�ߵ�
extern	double  Mn_close[20][60];	//����K����

extern double	tick_data[20][10];				//TICK��������
extern	double	tick_AskPrice1[20][60];			//��һ��
extern	double	tick_BidPrice1[20][60];			//��һ��
extern	double	tick_AskVolume1[20][60];		//��һ��
extern	double	tick_BidVolume1[20][60];		//��һ��
extern	double	tick_Volume[20][60];			//�ɽ���
extern	double	tick_OpenInterest[20][60];		//�ֲ���

extern	double	Sniffer_dataA[20][10]			;//�������
extern	bool	SnifferSignalA[20];



void Sniffer_A12(int i)	//����Tick�����Լ�ָ�����
{
	void Sniffer_56(int i);	//

	if (!ReceiveTick[i] && fabs(tick_data[i][0]-1)<0.01 )
	{
		SnifferSignalA[i] = true;

		bool TradingTimeA = (tick_data[i][2]>0.0910 && tick_data[i][2]<0.1450) || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2359) ;
		bool TradingTimeB = (tick_data[i][2]>0.0910 && tick_data[i][2]<0.1450) || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2359) ;

		if ( RunMode && Mn_close[i][1]> Mn_open[i][1]  && TradingTimeA)
		{
			Sniffer_dataA[i][0] = 1;
		}
		else if ( RunMode && Mn_close[i][1]< Mn_open[i][1]  && TradingTimeA)
		{
			Sniffer_dataA[i][0] = 2;
		}
		else
		{
			Sniffer_dataA[i][0] = 0;
			SnifferSignalA[i] = false;

		}

	}


}
