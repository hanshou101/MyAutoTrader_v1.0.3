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
extern	double	Trade_dataB[20][10]			;//����


extern double	Trade_Stopwin[20];
extern double	Trade_Stoploss[20];

extern double	Trade_Largevol[20];
extern double	Trade_Littelvol[20];
extern double	Trade_CloseProfit[20];
extern double	Trade_Closetimes[20];
extern	int		Trade_times[20];	//

extern bool		SnifferSignalB[20];
extern bool		TradingSignalB[20];

extern	double	Day_CloseProfitB[20];
extern	double	Trade_StopCloseProfit[20];
extern double	Day_TradeNumb[20];








void TraderB12(double system_times,int i)
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b,int md,int i);
	void WriteTradeConfiguration();
	double GetLocalTimeSec1();
	void _record1(char *txt1, char *txt2,double m,int n, int i);

	bool TradingTimeB = (tick_data[i][2]>0.0920 && tick_data[i][2]<0.1450) || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2350) ;
	bool TradingTimeS = (tick_data[i][2]>0.0920 && tick_data[i][2]<0.1450) || (tick_data[i][2]>0.2105 && tick_data[i][2]<0.2350) ;

		if (Sniffer_dataB[i][0] > 0 && TradingSignalB[i]==false)
		{
				
				SnifferSignalB[i] = false;
				TradingSignalB[i] = true;

				bool fanen=true;		//������
				bool duowen=true;		//������
				bool kongen=true;		//������
				
				//����뷴�ֲ���
				if ( (Sniffer_dataB[i][0] == 5 || Sniffer_dataB[i][0] == 98) && Trade_dataB[i][2]==-1 && Trade_dataB[i][3] == 6)
				{
					SendOrder(InstrumentID_n[i],0,3,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;	
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_dataB[i][5] = tick_AskPrice1[i][0];					
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
					
					if( TradingTimeB && fanen && duowen && Day_CloseProfitB[i]>-1*Trade_StopCloseProfit[i] )
					{
					SendOrder(InstrumentID_n[i],0,0,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][1] = tick_data[i][2];
					Trade_times[i]=Q_BarTime_1n[i];
					Trade_dataB[i][2] = 1;
					Trade_dataB[i][3] = 5;
					Trade_dataB[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_dataB[i][3]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
					}

				}
				else if ( (Sniffer_dataB[i][0] == 6 || Sniffer_dataB[i][0] == 98) && Trade_dataB[i][2]==1 && Trade_dataB[i][3] == 5)
				{
					SendOrder(InstrumentID_n[i],1,3,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_dataB[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
					
					if( TradingTimeS && fanen   && kongen && Day_CloseProfitB[i]>-1*Trade_StopCloseProfit[i] )
					{
					SendOrder(InstrumentID_n[i],1,0,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][1] = tick_data[i][2];
					Trade_times[i]=Q_BarTime_1n[i];
					Trade_dataB[i][2] = -1;
					Trade_dataB[i][3] = 6;
					Trade_dataB[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "����"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_dataB[i][3]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","����",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
					}

				}
				else if ((Sniffer_dataB[i][0] == 5 || Sniffer_dataB[i][0] == 98) && Trade_dataB[i][2] == 0 && TradingTimeB  && duowen  && Day_CloseProfitB[i]>-1*Trade_StopCloseProfit[i] )
				{
					
					SendOrder(InstrumentID_n[i],0,0,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][1] = tick_data[i][2];
					Trade_times[i]=Q_BarTime_1n[i];
					Trade_dataB[i][2] = 1;
					Trade_dataB[i][3] = 5;
					Trade_dataB[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_dataB[i][3]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);

				}
				else if ((Sniffer_dataB[i][0] == 6 || Sniffer_dataB[i][0] == 98) && Trade_dataB[i][2] == 0 && TradingTimeS  && kongen  && Day_CloseProfitB[i]>-1*Trade_StopCloseProfit[i] )
				{
					
					SendOrder(InstrumentID_n[i],1,0,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][1] = tick_data[i][2];
					Trade_times[i]=Q_BarTime_1n[i];
					Trade_dataB[i][2] = -1;
					Trade_dataB[i][3] = 6;
					Trade_dataB[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "����"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_dataB[i][3]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","����",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);

				}
				else
				{
					TradingSignalB[i] = false;
				}
				
		}

}


void StopLossB12(double system_times,int i)
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b,int md,int i);
	void WriteTradeConfiguration();
	int GetLocalTimeSec2();
	void _record1(char *txt1, char *txt2,double m,int n, int i);
	bool stopwinen=true;		//������
	bool stoplossen=false;		//������
	bool stopfanen=false;		//������


		//ֹӮƽ��
		if (Trade_dataB[i][2]==1 && Trade_dataB[i][3]==5 && (tick_BidPrice1[i][0] - Trade_dataB[i][5]) >= Trade_Stopwin[i] && stopwinen)
			{
					SendOrder(InstrumentID_n[i], 1, 3,0,i);
					TradingSignalB[i]=false;

					//Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_dataB[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹӮ"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹӮ",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
			}
		if (Trade_dataB[i][2]==-1 && Trade_dataB[i][3]==6 && (Trade_dataB[i][5] - tick_AskPrice1[i][0]) >= Trade_Stopwin[i] && stopwinen)
			{
					SendOrder(InstrumentID_n[i], 0, 3,0,i);
					TradingSignalB[i]=false;

					//Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_dataB[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹӮ"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹӮ",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
			}



		//ֹ��ƽ��
		if (Trade_dataB[i][2]==1 && Trade_dataB[i][3]==5 && (Trade_dataB[i][5] - tick_BidPrice1[i][0])>=Trade_Stoploss[i] && stoplossen)
			{
					SendOrder(InstrumentID_n[i], 1, 3,0,i);
					TradingSignalB[i]=false;

					//Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_dataB[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹ��"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹ��",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);					

			}
		if (Trade_dataB[i][2]==-1 && Trade_dataB[i][3]==6 && (tick_AskPrice1[i][0] - Trade_dataB[i][5])>=Trade_Stoploss[i] && stoplossen)
			{
					SendOrder(InstrumentID_n[i], 0, 3,0,i);
					TradingSignalB[i]=false;

					//Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_dataB[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽֹ��"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽֹ��",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);

			}



}



void StopEndTime_B(double system_times,int i)
{
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int a,int b,int md,int i);
	void WriteTradeConfiguration();
	int GetLocalTimeSec2();
	void _record1(char *txt1, char *txt2,double m,int n, int i);

	bool stopdingshien=false;	//������
	int n=10;					//10���Ӷ�ʱƽ��
	int nowtime=GetLocalTimeSec2();

		//��ʱƽ��
		if (Trade_dataB[i][2]==1  && (Q_BarTime_1n[i]-Trade_times[i])>n*60 && stopdingshien)
			{
					SendOrder(InstrumentID_n[i], 1, 3,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_dataB[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ��ʱ"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ��ʱ",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
			}
		if (Trade_dataB[i][2]==-1 && (Q_BarTime_1n[i]-Trade_times[i])>n*60 && stopdingshien)
			{
					SendOrder(InstrumentID_n[i], 0, 3,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_dataB[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ��ʱ"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					if (RunMode) {WriteTradeConfiguration();}
					_record1("����:","��ƽ��ʱ",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
			}


		//����ƽ��

		if (Trade_dataB[i][2]==1 && ( (system_times>0.1455 && system_times<0.1456) || (system_times>0.2350 && system_times<=0.2356) ) )
			{
					SendOrder(InstrumentID_n[i], 1, 3,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+tick_BidPrice1[i][0]-Trade_dataB[i][5];
					Trade_dataB[i][5] = tick_BidPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ����"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					_record1("����:","��ƽ����",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
			}
		if (Trade_dataB[i][2]==-1 && ((system_times>0.1455 && system_times<=0.1456) || (system_times>0.2350 && system_times<=0.2356) ) )
			{
					SendOrder(InstrumentID_n[i], 0, 3,0,i);
					TradingSignalB[i]=false;

					Trade_dataB[i][0] = 0;
					Trade_dataB[i][2] = 0;
					Trade_dataB[i][3] = 0;
					Trade_Closetimes[i]=Trade_Closetimes[i]+1;
					Day_CloseProfitB[i]=Day_CloseProfitB[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_CloseProfit[i]=Trade_CloseProfit[i]+Trade_dataB[i][5]-tick_AskPrice1[i][0];
					Trade_dataB[i][5] = tick_AskPrice1[i][0];
					cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<tick_data[i][2]<<"_"<< "��ƽ����"<<"_"<<Trade_dataB[i][5]<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i]<< endl;
					_record1("����:","��ƽ����",Trade_dataB[i][5],int(Trade_dataB[i][3]),i);
			}



	if( (system_times>0.1455 && system_times<0.1459) || (system_times>0.2350 && system_times<=0.2359) )
	{
		SnifferSignalB[i] = false;
		Day_CloseProfitB[i] =0 ;
	}

}
