/////////////////////////////////////////////////////////////////////////
///���ƣ�AutoTrader
///�汾��v1.0.x
///���ڣ�2014xxxx
///˵������������CTP����
///���ߣ�Сƽ����
///  QQ��1396144618
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

#pragma warning(disable : 4996)

extern int		Q_BarTime_1;		//

extern	int		Q_BarTime_1n[20];	//

extern	int		RunMode;
extern	int		DebugMode;

extern char		InstrumentID_n[20][10];
extern int		InstrumentID_lots[20];
extern double	InstrumentID_minmove[20];

extern	double  Day_open[20][60];	//��K�߿�
extern	double  Day_high[20][60];	//��K�߸�
extern	double  Day_low[20][60];	//��K�ߵ�
extern	double  Day_close[20][60];	//��K����

extern	double  Mn_open[20][60];	//����K�߿�
extern	double  Mn_high[20][60];	//����K�߸�
extern	double  Mn_low[20][60];		//����K�ߵ�
extern	double  Mn_close[20][60];	//����K����

extern  double  BuyPrice[20];		//���ּ�
extern  double  SellPrice[20];		//���ּ�
extern	int		BNum[20];			//���ִ���
extern	int		SNum[20];			//���ִ���
extern	int		BNum2[20];			//���ִ���
extern	int		SNum2[20];			//���ִ���

extern double	tick_data[20][10];				//TICK��������
extern	double	tick_AskPrice1[20][60];			//��һ��
extern	double	tick_BidPrice1[20][60];			//��һ��
extern	double	tick_AskVolume1[20][60];		//��һ��
extern	double	tick_BidVolume1[20][60];		//��һ��
extern	double	tick_Volume[20][60];			//�ɽ���
extern	double	tick_OpenInterest[20][60];		//�ֲ���

extern	double	Sniffer_dataA[20][10]			;//�������
extern	double	Sniffer_dataB[20][10]			;//�������
extern	double	Sniffer_dataC[20][10]			;//�������
//������������
extern	double	Trade_dataA[20][10]			;//����
extern	double	Trade_dataB[20][10]			;//����

extern double	Trade_Stopwin[20];
extern double	Trade_Stoploss[20];

extern double	Trade_Largevol[20];
extern double	Trade_Littelvol[20];
extern double	Trade_CloseProfit[20];
extern double	Trade_Closetimes[20];
extern	int		Trade_times[20];	//

extern bool		SnifferSignalA[20];
extern bool		TradingSignalA[20];

extern	double	Day_CloseProfit[20];
extern	double	Trade_StopCloseProfit[20];


extern	double	Day_BuyPos[20];
extern	double	Day_SellPos[20];








void TraderA12(double system_times,int i)
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b,int md,int i);
	void WriteTradeConfiguration();
	double GetLocalTimeSec1();
	void _record1(char *txt1, char *txt2,double m,int n, int i);

	bool TradingTimeB = (tick_data[i][2]>0.1014 && tick_data[i][2]<0.1400);// || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2350) ;
	bool TradingTimeS = (tick_data[i][2]>0.0905 && tick_data[i][2]<0.1400);// || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2350) ;

	bool Condtion1	= (tick_BidPrice1[i][0]-tick_data[i][9])>20*InstrumentID_minmove[i] && (tick_data[i][8]-tick_AskPrice1[i][0])>10*InstrumentID_minmove[i] ;
	bool Condtion2	= (tick_data[i][8]-tick_AskPrice1[i][0])>20*InstrumentID_minmove[i] && (tick_BidPrice1[i][0]-tick_data[i][9])>10*InstrumentID_minmove[i] ;

		if (Sniffer_dataA[i][0] > 0 && TradingSignalA[i]==false)
		{
				SnifferSignalA[i] = false;
				TradingSignalA[i] = true;

				if ( (Sniffer_dataA[i][0] == 1 || Sniffer_dataA[i][0] == 98) && TradingTimeB)
				{
					if (Trade_dataA[i][2] <-0.5)//���з��򵥣���ƽ�ִ���
					{
					SendOrder(InstrumentID_n[i],0,3,1,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;	
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_dataA[i][5] = tick_AskPrice1[i][0];					
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
					}


					if ( (fabs(Trade_dataA[i][2]))<0.5 && Day_CloseProfit[i]>-1.6*Trade_StopCloseProfit[i] &&  Condtion1)
					{
					SendOrder(InstrumentID_n[i],0,0,1,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][1] = tick_data[i][2];
					Trade_times[i]=Q_BarTime_1n[i];
					Trade_dataA[i][2] = 1;
					Trade_dataA[i][3] = 1;
					Trade_dataA[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_dataA[i][3]<< endl;

					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);

					}
				}
				else if ( (Sniffer_dataA[i][0] == 2 || Sniffer_dataA[i][0] == 98) && TradingTimeS)
				{
					if (Trade_dataA[i][2] > 0.5)//���з��򵥣���ƽ�ִ���
					{
					SendOrder(InstrumentID_n[i],1,3,1,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_dataA[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
					}


					if ((fabs(Trade_dataA[i][2]))<0.5 &&  Day_CloseProfit[i]>-1.6*Trade_StopCloseProfit[i] && Condtion2)
					{
					SendOrder(InstrumentID_n[i],1,0,1,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][1] = tick_data[i][2];
					Trade_times[i]=Q_BarTime_1n[i];
					Trade_dataA[i][2] = -1;
					Trade_dataA[i][3] = 2;
					Trade_dataA[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "����"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_dataA[i][3]<< endl;
					
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","����",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
					}
				}
				else
				{
					TradingSignalA[i] = false;
				}
		}



}



void StopLossA12(double system_times,int i)
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b,int md,int i);
	void WriteTradeConfiguration();
	int GetLocalTimeSec2();
	void _record1(char *txt1, char *txt2,double m,int n, int i);
	bool stopwinen=true;		//������
	bool stoplossen=true;		//������

	double Mn_D2=(Mn_open[i][1] + Mn_close[i][1])/2;

		//ֹӮƽ��
		if (Trade_dataA[i][2]==1 && Trade_dataA[i][3]==1 && (Mn_D2 - Trade_dataA[i][5]) >= Trade_Stopwin[i] && stopwinen)
			{
					SendOrder(InstrumentID_n[i], 1, 3,0,i);
					TradingSignalA[i]=false;

					//Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_dataA[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹӮ"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹӮ",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
			}
		if (Trade_dataA[i][2]==-1 && Trade_dataA[i][3]==2 && (Trade_dataA[i][5] - Mn_D2) >= Trade_Stopwin[i] && stopwinen)
			{
					SendOrder(InstrumentID_n[i], 0, 3,0,i);
					TradingSignalA[i]=false;

					//Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_dataA[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹӮ"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹӮ",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
			}



		//ֹ��ƽ��
		if (Trade_dataA[i][2]==1 && Trade_dataA[i][3]==1 && (Trade_dataA[i][5] - Mn_D2)>=Trade_Stoploss[i] && stoplossen)
			{
					SendOrder(InstrumentID_n[i], 1, 3,1,i);
					TradingSignalA[i]=false;

					//Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_dataA[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹ��"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;

					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹ��",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);

			}
		if (Trade_dataA[i][2]==-1 && Trade_dataA[i][3]==2 && (Mn_D2 - Trade_dataA[i][5])>=Trade_Stoploss[i] && stoplossen)
			{
					SendOrder(InstrumentID_n[i], 0, 3,1,i);
					TradingSignalA[i]=false;

					//Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_dataA[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹ��"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;

					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹ��",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);

			}


}

void StopEndTime_A(double system_times,int i)
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b,int md,int i);
	void WriteTradeConfiguration();
	int GetLocalTimeSec2();
	void _record1(char *txt1, char *txt2,double m,int n, int i);

	bool stopdingshien=false;	//������
	int n=10;					//10���Ӷ�ʱƽ��
	int nowtime=GetLocalTimeSec2();

		//��ʱƽ��
		if (Trade_dataA[i][2]==1  && (Q_BarTime_1n[i]-Trade_times[i])>n*60 && stopdingshien)
			{
					SendOrder(InstrumentID_n[i], 1, 3,0,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_dataA[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ��ʱ"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ��ʱ",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
			}
		if (Trade_dataA[i][2]==-1 && (Q_BarTime_1n[i]-Trade_times[i])>n*60 && stopdingshien)
			{
					SendOrder(InstrumentID_n[i], 0, 3,0,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_dataA[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ��ʱ"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ��ʱ",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
			}


		//����ƽ��

		if (Trade_dataA[i][2]==1 && ( (system_times>0.1455 && system_times<0.1456) || (system_times>0.2350 && system_times<=0.2356) ) )
			{
					SendOrder(InstrumentID_n[i], 1, 3,0,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataA[i][5];
					Trade_dataA[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ����"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					_record1("����:","��ƽ����",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
			}
		if (Trade_dataA[i][2]==-1 && ((system_times>0.1455 && system_times<=0.1456) || (system_times>0.2350 && system_times<=0.2356) ) )
			{
					SendOrder(InstrumentID_n[i], 0, 3,0,i);
					TradingSignalA[i]=false;

					Trade_dataA[i][0] = 0;
					Trade_dataA[i][2] = 0;
					Trade_dataA[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfit[i]=Day_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataA[i][5]-tick_AskPrice1[i][0];
					Trade_dataA[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ����"<<"_"<<Trade_dataA[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					_record1("����:","��ƽ����",Trade_dataA[i][5],int(Trade_dataA[i][3]),i);
			}


	if( (system_times>0.1455 && system_times<0.1459) || (system_times>0.2350 && system_times<=0.2359) )
	{
		SnifferSignalA[i] = false;
	}

}

