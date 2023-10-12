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

#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcTraderApi* pUserApi;
// 配置参数	
extern TThostFtdcBrokerIDType	BROKER_ID;						//经纪商
extern TThostFtdcInvestorIDType INVESTOR_ID;					//
extern TThostFtdcPasswordType	PASSWORD;						//

TThostFtdcInstrumentIDType INSTRUMENT_ID = "rb1410";			// 交易合约代码
int		DIRECTION = 0;											// 交易买卖方向
int		MARKETState = 0;										// 开平仓
double	LIMIT_PRICE = 999999.0;									// 交易价格

// 会话参数
extern	int		RunMode;
extern	TThostFtdcFrontIDType	FRONT_ID;						//前置编号
extern	TThostFtdcSessionIDType	SESSION_ID;						//会话编号
extern	TThostFtdcOrderRefType	ORDER_REF;						//报单引用
extern	TThostFtdcOrderActionRefType	ORDERACTION_REF[20];	//撤单引用
// 请求编号
int		iRequestID = 0;											//请求编号

// 交易时间
TThostFtdcDateExprType	TradingDay;								//交易日期

bool	JustRun = false;										//正在启动标志
bool	CloseAll= false;										//收盘标志

int		FirstVolume=0;											//前一次成交量数据

string	InstrumentID_name="";	//缓存TICK合约名称

int		Q_BarTime_1 = 0;		//缓存TICK时间戳：秒计算
double	Q_BarTime_2 = 0;		//缓存TICK时间戳：0.145500

int		Q_BarTime_1n[20]	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//合约时间戳：秒计算
int		Trade_times[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//开仓时间戳：秒计算

char    LogFilePaths[80]="";			//交易日志
char	TickFileWritepaths[20][80]	= { "", "", "", "", "", "", "", "", "", "" ,"", "", "", "", "", "", "", "", "", "" };	//TICK数据保存文件名次格式，合约名称_日期.txt

//											0		1			2		  3			4		 5		  6		   7		8			9		10		11		12		13		14		15		  16		17	   18		 19
char	InstrumentID_n[20][10]		= {  "i1409", "jm1409", "j1409", "rb1410","rb1501", "TA409", "l1409","ru1409", "ru1501", "jd1409", "RM409", "m1409","y1501", "p1501","ag1506","ag1412","cu1408","cu1409","IF1409","IF1407"};//交易合约

int		InstrumentID_En[20]			= { 0       ,        0,       0,        0,       0,       0,       0,       0,        0,        0,       0,       0,      0,       0,       0,       0,       0,      0,	 0,       0  };	//交易使能 =1，会实盘下单
int		InstrumentID_lots[20]		= { 1       ,        1,       1,        1,       1,       1,       1,       1,        1,        1,       1,       1,      1,       1,       1,       1,       1,      1,	 1,       1  };	//开仓量

double  InstrumentID_minmove[20]	= { 1       ,        1,       1,        1,       1,       2,       5,       5,        5,        1,       1,       1,      2,       2,       1,       1,      10,     10,   0.2,     0.2, };	//最小变动价位

double	Trade_Stopwin[20]			= { 30       ,      30,      30,       20,      20,      30,      30,     120,      120,       30,      30,      30,     30,      30,      30,      30,      30,     30,    30,      30  };	//单次开仓止赢点
double	Trade_Stoploss[20]			= { 30       ,      30,      30,       20,      20,      30,      30,     180,      180,       30,      30,      30,     30,      30,      30,      30,      30,     30,    30,      30  };	//单次开仓止损点
double	Trade_StopCloseProfit[20]	= { 30       ,      30,      30,       10,      10,      30,      30,     110,      110,       30,      30,      30,     30,      30,      30,      30,      30,     30,    30,      30  };	//>止损,限制新开


//tick数据
bool	FristTick[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//收到当日第一个有效TICK标记
bool	LastTick[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//收到当日最后一个有效TICK标记
bool	ReceiveTick[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//TICK数据接收标记，暂未使用

double	tick_data[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:基本信息

double	tick_AskPrice1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据
double	tick_BidPrice1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据
double	tick_AskVolume1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据
double	tick_BidVolume1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据
double	tick_Volume[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据
double	tick_OpenInterest[20][60]	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//行情数据:保存60个TICK数据

double	Sniffer_dataA[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//监测数据
double	Sniffer_dataB[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//监测数据
double	Sniffer_dataC[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//监测数据
double	Sniffer_dataD[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//监测数据

//----------------------
double  Day_open[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//日K线数据开 
double  Day_high[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//日K线数据高
double  Day_low[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//日K线数据低
double  Day_close[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//日K线数据收

bool	MnKlinesig[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//分钟K线第一个TICK标记
double  Mn_open[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//分钟K线数据开 
double  Mn_high[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//分钟K线数据高
double  Mn_low[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//分钟K线数据低
double  Mn_close[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//分钟K线数据收

//-----------------------
bool	SnifferSignalA[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//指标策略运算标记
bool	TradingSignalA[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//下单标记
bool	SnifferSignalB[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//指标策略运算标记
bool	TradingSignalB[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//下单标记

//基本订单数据
double	Trade_dataA[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//订单数据
double	Trade_dataB[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//订单数据
double	Trade_dataC[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//订单数据
double	Trade_dataD[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//订单数据


//测试开平仓统计数据
double	Trade_CloseProfit[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//平仓盈亏，测试策略用
double	Trade_Closetimes[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//平仓次数，测试策略用
double	Day_CloseProfit[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//当日平仓收益，测试策略用
double	Day_CloseProfitA[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//当日平仓收益，测试策略用
double	Day_CloseProfitB[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//当日平仓收益，测试策略用
double	Day_TradeNumb[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//开仓次数统计




void _record0(char *txt)
{
	ofstream o_file(LogFilePaths, ios::app);
	o_file <<  txt << endl; //将内容写入到文本文件中
	o_file.close();						//关闭文件
}

void _record1(char *txt1, char *txt2,double m,int n, int i)
{
	string dbtoch(double nums);
	//string dbtoch2(double nums);

	ofstream o_file(LogFilePaths, ios::app);
	if (n>0)
	{
		if (n>0 && n<=4)
		{
			o_file <<txt1<< dbtoch(tick_data[i][1]) << "_" << tick_data[i][2] << "_" << InstrumentID_n[i] << "_" << InstrumentID_lots[i] <<"_"<< txt2 << "_"<< Trade_dataA[i][5] <<"_"<<Trade_dataA[i][3]<< endl; //将内容写入到文本文件中
		}
		else if (n>4 && n<=6)
		{
			o_file <<txt1<< dbtoch(tick_data[i][1]) << "_" << tick_data[i][2] << "_" << InstrumentID_n[i] << "_" << InstrumentID_lots[i] <<"_"<< txt2 << "_"<< Trade_dataB[i][5] <<"_"<<Trade_dataB[i][3]<< endl; //将内容写入到文本文件中
		}
		
	}
	else
	{
		//																																					--待修改！！！
		o_file <<txt1<< dbtoch(tick_data[i][1]) << "_" << tick_data[i][2] << "_" << InstrumentID_n[i] << "_" << InstrumentID_lots[i] <<"_"<< txt2 << "_"<< m<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i] << endl; //将内容写入到文本文件中
	}
	o_file.close();						//关闭文件
}

double GetLocalTimeSec1()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	double system_times;
	system_times=(double)((sys_time.wHour)/10e1)+(double)((sys_time.wMinute)/10e3)+(double)((sys_time.wSecond)/10e5);	//格式时间0.145100
	return system_times;
}
int GetLocalTimeSec2()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	int system_times;
	system_times=(int)((sys_time.wHour)*60*60)+(int)((sys_time.wMinute)*60)+(int)((sys_time.wSecond));					//格式时间int sec
	return system_times;
}

double GetLocalTimeMs1()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	double system_times;
	system_times=(double)((sys_time.wHour)/10e1)+(double)((sys_time.wMinute)/10e3)+(double)((sys_time.wSecond)/10e5)+(double)((sys_time.wMilliseconds)/10e8);	//格式时间0.145100500
	return system_times;
}
int GetLocalTimeMs2()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	int system_times;
	system_times=((int)((sys_time.wHour)*60*60)+(int)((sys_time.wMinute)*60)+(int)((sys_time.wSecond)))*1000+(int)((sys_time.wMilliseconds));	//格式时间int ms
	return system_times;
}


bool ReadMdConfiguration()
{
	ifstream config("./AutoTrader.dat");

	if (!config)
	{
		cerr << "--->>> " << "MdConfiguration File is missing!" << endl;
		return false;
	}
	else
	{
		cerr << "--->>> " << "Read MdConfiguration File!" << endl;
	}

	char line[1024]={0};
	vector < double > data(10);

	ifstream fin("./AutoTrader.dat", std::ios::in);
	int i = 0;

	while(fin.getline(line, sizeof(line)))
	{
		std::stringstream word(line);	
		//cout << line << endl;	//打印整行数据
		//Sleep(100);
		double Day_fprice_t[9];

		for (int j = 0; j < 9; j++)
		{
			word >> Day_fprice_t[j];
			//cout << Day_fprice_t[i][j] << endl;	//打印整行数据
		}
		
		Day_open[i][1]	=Day_fprice_t[5];
		Day_high[i][1]	=Day_fprice_t[6];
		Day_low[i][1]	=Day_fprice_t[7];
		Day_close[i][1]	=Day_fprice_t[8];

		Day_open[i][2]	=Day_fprice_t[1];
		Day_high[i][2]	=Day_fprice_t[2];
		Day_low[i][2]	=Day_fprice_t[3];
		Day_close[i][2]	=Day_fprice_t[4];

		i++;

	}

	config.close();
	return true;
}

bool ReadTradeConfiguration()
{
	ifstream config("./AutoTrader.cfg");

	if (!config)
	{
		cerr << "--->>> " << "TradeConfiguration File is missing!" << endl;
		return false;
	}
	else
	{
		cerr << "--->>> " << "Read TradeConfiguration File!" << endl;
	}

	char line[1024]={0};
	vector < double > data(10);
	double temp0=0;
	double temp1=0;

	ifstream fin("./AutoTrader.cfg", std::ios::in);
	int i = 0;
	int it = 0;

	while(fin.getline(line, sizeof(line)))
	{
		i=int(it%20);
		std::stringstream word(line);

		for (int j = 0; j < 9; j++)
		{
			if (j==0)
			{
				word >> temp0;
			}
			else if(j==1)
			{
				word >> temp1;
			}
			else if(j>1 && j<=6)
			{
				if (it<20)
				{word >> Trade_dataA[i][j-1];}
				else if (it>=20 && it<40)
				{word >> Trade_dataB[i][j-1];}
				else if (it>=40 && it<60)
				{word >> Trade_dataC[i][j-1];}

			}
			else if(j==7)
			{
				word >> Trade_Closetimes[i];
			}
			else if(j==8)
			{
				word >> Trade_CloseProfit[i];
			}

		}
		
		if (temp1>0 && temp1<5)
		{
			if (Trade_dataA[i][2]>0.5)
			{
			cerr << "--->>> 报单: " <<InstrumentID_n[i]<<"_"<<Trade_dataA[i][1]<<"_"<<Trade_dataA[i][3]<<"_"<< "买开"<<"_"<<Trade_dataA[i][5]<< endl;
			}
			else if (Trade_dataA[i][2]<-0.5)
			{
			cerr << "--->>> 报单: " <<InstrumentID_n[i]<<"_"<<Trade_dataA[i][1]<<"_"<<Trade_dataA[i][3]<<"_"<< "卖开"<<"_"<<Trade_dataA[i][5]<< endl;
			}		
		}
		else if (temp1>4 && temp1<7)
		{
			if (Trade_dataB[i][2]>0.5)
			{
			cerr << "--->>> 报单: " <<InstrumentID_n[i]<<"_"<<Trade_dataB[i][1]<<"_"<<Trade_dataB[i][3]<<"_"<< "买开"<<"_"<<Trade_dataB[i][5]<< endl;
			}
			else if (Trade_dataB[i][2]<-0.5)
			{
			cerr << "--->>> 报单: " <<InstrumentID_n[i]<<"_"<<Trade_dataB[i][1]<<"_"<<Trade_dataB[i][3]<<"_"<< "卖开"<<"_"<<Trade_dataB[i][5]<< endl;
			}			
		}

		it++;

	}

	config.close();
	return true;
}

void WriteMdConfiguration()
{
	string dbtoch(double nums);

	ofstream o_file("./AutoTrader.dat",ios::trunc);		//保存近2天的日线数据
	for (int i = 0; i < 20; i++)
	{
		Day_open[i][2]	=Day_open[i][1];
		Day_high[i][2]	=Day_high[i][1];
		Day_low[i][2]	=Day_low[i][1];
		Day_close[i][2]	=Day_close[i][1];

		Day_open[i][1]	=Day_open[i][0];
		Day_high[i][1]	=Day_high[i][0];
		Day_low[i][1]	=Day_low[i][0];
		Day_close[i][1]	=Day_close[i][0];

		//o_file << dbtoch(tick_data[0][1]) << "\t" <<tick_data[i][2] <<"\t"<< Mn_open << " " << Mn_high << " " << Mn_low << " " << Mn_close << endl; //将内容写入到文本文件中
		o_file << dbtoch(tick_data[i][1]) <<"\t"<< Day_open[i][2] << "\t" << Day_high[i][2] << "\t" << Day_low[i][2] << "\t" << Day_close[i][2] <<"\t"<< Day_open[i][1] << "\t" << Day_high[i][1] << "\t" << Day_low[i][1] << "\t" <<Day_close[i][1] << endl; //将内容写入到文本文件中
	}
	o_file.close();						//关闭文件

}

void WriteTradeConfiguration()
{
	string dbtoch(double nums);
	int	tn=0;

	ofstream o_file("./AutoTrader.cfg",ios::trunc);
	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<< Trade_dataA[i][3]<<"\t"<< Trade_dataA[i][1] << "\t" << Trade_dataA[i][2] << "\t" << Trade_dataA[i][3] << "\t" << Trade_dataA[i][4] <<"\t"<< Trade_dataA[i][5] << "\t" << Trade_Closetimes[i] << "\t" << Trade_CloseProfit[i]<< endl; //将内容写入到文本文件中
	}

	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<< Trade_dataB[i][3]<<"\t"<< Trade_dataB[i][1] << "\t" << Trade_dataB[i][2] << "\t" << Trade_dataB[i][3] << "\t" << Trade_dataB[i][4] <<"\t"<< Trade_dataB[i][5] << "\t" << Trade_Closetimes[i] << "\t" << Trade_CloseProfit[i]<< endl; //将内容写入到文本文件中
	}


	o_file.close();						//关闭文件

}

void ErasingTradeConfiguration()
{
	string dbtoch(double nums);
	int	tn=0;
	ofstream o_file("./AutoTrader.cfg",ios::trunc);

	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< endl; //将内容写入到文本文件中
	}

	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< endl; //将内容写入到文本文件中
	}

	o_file.close();						//关闭文件

}

void Erasefiles()
{
	system("del .\\thostmduserapi.dllDialogRsp.con");
	system("del .\\thostmduserapi.dllQueryRsp.con");
	system("del .\\thostmduserapi.dllTradingDay.con");

	system("del .\\thosttraderapi.dllDialogRsp.con");
	system("del .\\thosttraderapi.dllPrivate.con");
	system("del .\\thosttraderapi.dllPublic.con");
	system("del .\\thosttraderapi.dllQueryRsp.con");
	system("del .\\thosttraderapi.dllTradingDay.con");

}


string dbtoch(double nums)		//将double 转换为 string 进行保存
{
	stringstream ss;
	string str;
	ss<<nums;
	ss>> str;

	char *chr = new char[str.length()];

	sprintf(chr, "%.0f", nums);
	string tt=chr;

	return tt;
}


string dbtoch2(double nums)		//将double 转换为 string 进行保存
{
	stringstream ss;
	string str;
	ss<<nums;
	ss>> str;

	char *chr = new char[str.length()];

	sprintf(chr, "%.6f", nums);
	string tt=chr;

	return tt;
}

//报单-限价
void SendOrder(TThostFtdcInstrumentIDType FuturesId,int BuySell,int OpenClose,int md,int i)
{

	strcpy(INSTRUMENT_ID,InstrumentID_n[i]);
	//cerr << "--->>> 报单录入请求: " <<INSTRUMENT_ID<< endl;

	DIRECTION = BuySell;
	MARKETState = OpenClose;
	
	//Sleep(050);

	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy_s(req.BrokerID, BROKER_ID);
	///投资者代码
	strcpy_s(req.InvestorID, INVESTOR_ID);
	///合约代码											//INSTRUMENT_ID
	strcpy_s(req.InstrumentID, INSTRUMENT_ID);
	///报单引用
	//strcpy_s(req.OrderRef, ORDER_REF);				//下单前修改
	///用户代码
	//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;

	///买卖方向:										//THOST_FTDC_D_Buy,THOST_FTDC_D_Sell
	if (BuySell==0)
	{
	req.Direction = THOST_FTDC_D_Buy;
	}
	else if (BuySell==1)
	{
	req.Direction = THOST_FTDC_D_Sell;
	}

	///组合开平标志: 开仓								//THOST_FTDC_OF_Open,THOST_FTDC_OF_Close,THOST_FTDC_OF_CloseToday
	if (OpenClose==0)
	{
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	}
	else if(OpenClose==1)
	{
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;		//其他交易所平今，平昨，上期平昨
	}
	else if(OpenClose==3)
	{
	req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;	//上期平今
	}

	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	//投机

	///价格
	if (md)
	{
		if (BuySell==0)
		{LIMIT_PRICE = tick_data[i][8];}
		else if (BuySell==1)
		{LIMIT_PRICE = tick_data[i][9];}
	}
	else
	{
		if (BuySell==0)
		{LIMIT_PRICE = tick_AskPrice1[i][0];}
		else if (BuySell==1)
		{LIMIT_PRICE = tick_BidPrice1[i][0];}
	}


	req.LimitPrice = LIMIT_PRICE;

	///数量: 1											//     开平仓数量
	req.VolumeTotalOriginal = InstrumentID_lots[i];						
	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 1;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///业务单元
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	req.UserForceClose = 0;
	
	if ( RunMode && InstrumentID_En[i]==1 )
	{
	///报单引用
	int iNextOrderRef = atoi(ORDER_REF);
	iNextOrderRef++;
	sprintf(ORDER_REF,"%d",iNextOrderRef);
	strcpy_s(req.OrderRef, ORDER_REF);
	//sprintf(ORDERACTION_REF[i], ORDER_REF);	//保存

	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);		//实盘，会正式下单
	cerr << "--->>> 报单录入请求: " <<InstrumentID_n[i]<< ((iResult == 0) ? " 成功" : " 失败") << endl;
	}
	else
	{
	int iResult = 0;	//测试，不真正下单
	//cerr << "--->>> 报单录入请求: " <<InstrumentID_n[i]<< ((iResult == 0) ? " 成功" : " 失败") << endl;
	}

}
