/////////////////////////////////////////////////////////////////////////
///名称：AutoTrader
///版本：v1.0.x
///日期：2014xxxx
///说明：基于上期CTP技术
///作者：小平常心
///  QQ：1396144618
/////////////////////////////////////////////////////////////////////////
#pragma once
//
#include <string>
#include <iomanip>
#include <vector>

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"
#include "strategy_A.h"
#include "strategy_B.h"


#pragma warning(disable : 4996)

extern int		Q_BarTime_1;		//

extern	int		Q_BarTime_1n[20];	//

extern	int	    RunMode;

extern char		InstrumentID_n[20][10];
extern int		InstrumentID_lots[20];

extern	double  Day_open[20][60];	//日K线开
extern	double  Day_high[20][60];	//日K线高
extern	double  Day_low[20][60];	//日K线低
extern	double  Day_close[20][60];	//日K线收

extern	double  Mn_open[20][60];	//分钟K线开
extern	double  Mn_high[20][60];	//分钟K线高
extern	double  Mn_low[20][60];		//分钟K线低
extern	double  Mn_close[20][60];	//分钟K线收

extern  double  BuyPrice[20];		//开仓价
extern  double  SellPrice[20];		//开仓价

extern double	tick_data[20][10];				//TICK基本数据
extern	double	tick_AskPrice1[20][60];			//买一价
extern	double	tick_BidPrice1[20][60];			//卖一价
extern	double	tick_AskVolume1[20][60];		//买一量
extern	double	tick_BidVolume1[20][60];		//卖一量
extern	double	tick_Volume[20][60];			//成交量
extern	double	tick_OpenInterest[20][60];		//持仓量

extern	double	Sniffer_dataA[20][10]			;//监测数据
extern	double	Sniffer_dataB[20][10]			;//监测数据
extern	double	Sniffer_dataC[20][10]			;//监测数据
extern	double	Sniffer_dataD[20][10]			;//监测数据
//基本订单数据

extern double	Trade_Stopwin[20];
extern double	Trade_Stoploss[20];

extern double	Trade_Largevol[20];
extern double	Trade_Littelvol[20];
extern double	Trade_CloseProfit[20];
extern double	Trade_Closetimes[20];
extern	int		Trade_times[20];	//

extern bool		SnifferSignal[20];
extern bool		TradingSignal[20];

extern	double	Day_CloseProfit[20];
extern	double	Trade_StopCloseProfit[20];
extern double	Day_TradeNumb[20];


void Trading()	//策略，下单
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b,int md,int i);
	double GetLocalTimeSec1();
	void StopEndTime_A(double system_times,int i);
	void StopEndTime_B(double system_times,int i);

	void StopLossA12(double system_times,int i);	
	void TraderA12(double system_times,int i);

	void StopLossB12(double system_times,int i);
	void TraderB12(double system_times,int i);

	void _record1(char *txt1, char *txt2,double m,int n, int i);

	for (int i = 0; i < 20; i++)
	{
	  if(1)//i!=8 && i!=9)	//	测试用，暂时不交易ag，cu,对仿真，实盘均有效
	  {

		StopEndTime_A(tick_data[i][2],i);
		StopEndTime_B(tick_data[i][2],i);
		
		if(RunMode)
		{
			if ( (tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i<=17) )
			{			
				StopLossA12(tick_data[i][2],i);		//Master
				if ( Sniffer_dataA[i][0] > 0 )
				{			
				TraderA12(tick_data[i][2],i);		//Master
				Sniffer_dataA[i][0]=0;
				}

				StopLossB12(tick_data[i][2],i);				
				if ( Sniffer_dataB[i][0] > 0 )
				{			
				TraderB12(tick_data[i][2],i);
				Sniffer_dataB[i][0]=0;
				}


			}
		}
		else	//测试模式
		{
				StopLossA12(tick_data[i][2],i);		//Master
				if ( Sniffer_dataA[i][0] > 0 )
				{			
				TraderA12(tick_data[i][2],i);		//Master
				Sniffer_dataA[i][0]=0;
				}

				StopLossB12(tick_data[i][2],i);				
				if ( Sniffer_dataB[i][0] > 0 )
				{			
				TraderB12(tick_data[i][2],i);
				Sniffer_dataB[i][0]=0;
				}


		}

	  }
	}

}

