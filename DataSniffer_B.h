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

extern	double	Sniffer_dataB[20][10]			;//�������
extern	bool	SnifferSignalB[20];


 


void Sniffer_B12(int i)	//����Tick�����Լ�ָ�����
{
		

		if (!ReceiveTick[i] && fabs(tick_data[i][0]-1)<0.01 )//ru
		{	
			SnifferSignalB[i] = true;

			bool TradingTimeA = (tick_data[i][2]>0.0910 && tick_data[i][2]<0.1450) || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2359) ;
			bool TradingTimeB = (tick_data[i][2]>0.0910 && tick_data[i][2]<0.1450) || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2359) ;

			bool condtion1=tick_AskPrice1[i][0]>tick_AskPrice1[i][1];
			bool condtion2=tick_AskPrice1[i][0]<tick_AskPrice1[i][1];

			bool condtion3=tick_AskPrice1[i][1]>tick_AskPrice1[i][2];
			bool condtion4=tick_AskPrice1[i][1]<tick_AskPrice1[i][2];

			bool condtion5=tick_AskPrice1[i][2]>tick_AskPrice1[i][3];
			bool condtion6=tick_AskPrice1[i][2]<tick_AskPrice1[i][3];


			if(condtion1 && 1 && 1 && TradingTimeA )
			{
				Sniffer_dataB[i][0] = 5;
			}
			else if(condtion2 && 1 && 1 && TradingTimeA )
			{		
				Sniffer_dataB[i][0] = 6;
			}
			else
			{
				Sniffer_dataB[i][0] = 0;
				SnifferSignalB[i] = false;
			}


		}


}

