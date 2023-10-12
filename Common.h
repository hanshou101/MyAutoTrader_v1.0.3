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

// USER_API����
extern CThostFtdcTraderApi* pUserApi;
// ���ò���	
extern TThostFtdcBrokerIDType	BROKER_ID;						//������
extern TThostFtdcInvestorIDType INVESTOR_ID;					//
extern TThostFtdcPasswordType	PASSWORD;						//

TThostFtdcInstrumentIDType INSTRUMENT_ID = "rb1410";			// ���׺�Լ����
int		DIRECTION = 0;											// ������������
int		MARKETState = 0;										// ��ƽ��
double	LIMIT_PRICE = 999999.0;									// ���׼۸�

// �Ự����
extern	int		RunMode;
extern	TThostFtdcFrontIDType	FRONT_ID;						//ǰ�ñ��
extern	TThostFtdcSessionIDType	SESSION_ID;						//�Ự���
extern	TThostFtdcOrderRefType	ORDER_REF;						//��������
extern	TThostFtdcOrderActionRefType	ORDERACTION_REF[20];	//��������
// ������
int		iRequestID = 0;											//������

// ����ʱ��
TThostFtdcDateExprType	TradingDay;								//��������

bool	JustRun = false;										//����������־
bool	CloseAll= false;										//���̱�־

int		FirstVolume=0;											//ǰһ�γɽ�������

string	InstrumentID_name="";	//����TICK��Լ����

int		Q_BarTime_1 = 0;		//����TICKʱ����������
double	Q_BarTime_2 = 0;		//����TICKʱ�����0.145500

int		Q_BarTime_1n[20]	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��Լʱ����������
int		Trade_times[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����ʱ����������

char    LogFilePaths[80]="";			//������־
char	TickFileWritepaths[20][80]	= { "", "", "", "", "", "", "", "", "", "" ,"", "", "", "", "", "", "", "", "", "" };	//TICK���ݱ����ļ����θ�ʽ����Լ����_����.txt

//											0		1			2		  3			4		 5		  6		   7		8			9		10		11		12		13		14		15		  16		17	   18		 19
char	InstrumentID_n[20][10]		= {  "i1409", "jm1409", "j1409", "rb1410","rb1501", "TA409", "l1409","ru1409", "ru1501", "jd1409", "RM409", "m1409","y1501", "p1501","ag1506","ag1412","cu1408","cu1409","IF1409","IF1407"};//���׺�Լ

int		InstrumentID_En[20]			= { 0       ,        0,       0,        0,       0,       0,       0,       0,        0,        0,       0,       0,      0,       0,       0,       0,       0,      0,	 0,       0  };	//����ʹ�� =1����ʵ���µ�
int		InstrumentID_lots[20]		= { 1       ,        1,       1,        1,       1,       1,       1,       1,        1,        1,       1,       1,      1,       1,       1,       1,       1,      1,	 1,       1  };	//������

double  InstrumentID_minmove[20]	= { 1       ,        1,       1,        1,       1,       2,       5,       5,        5,        1,       1,       1,      2,       2,       1,       1,      10,     10,   0.2,     0.2, };	//��С�䶯��λ

double	Trade_Stopwin[20]			= { 30       ,      30,      30,       20,      20,      30,      30,     120,      120,       30,      30,      30,     30,      30,      30,      30,      30,     30,    30,      30  };	//���ο���ֹӮ��
double	Trade_Stoploss[20]			= { 30       ,      30,      30,       20,      20,      30,      30,     180,      180,       30,      30,      30,     30,      30,      30,      30,      30,     30,    30,      30  };	//���ο���ֹ���
double	Trade_StopCloseProfit[20]	= { 30       ,      30,      30,       10,      10,      30,      30,     110,      110,       30,      30,      30,     30,      30,      30,      30,      30,     30,    30,      30  };	//>ֹ��,�����¿�


//tick����
bool	FristTick[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�յ����յ�һ����ЧTICK���
bool	LastTick[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�յ��������һ����ЧTICK���
bool	ReceiveTick[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//TICK���ݽ��ձ�ǣ���δʹ��

double	tick_data[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:������Ϣ

double	tick_AskPrice1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����
double	tick_BidPrice1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����
double	tick_AskVolume1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����
double	tick_BidVolume1[20][60]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����
double	tick_Volume[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����
double	tick_OpenInterest[20][60]	= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������:����60��TICK����

double	Sniffer_dataA[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�������
double	Sniffer_dataB[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�������
double	Sniffer_dataC[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�������
double	Sniffer_dataD[20][10]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�������

//----------------------
double  Day_open[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K�����ݿ� 
double  Day_high[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K�����ݸ�
double  Day_low[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K�����ݵ�
double  Day_close[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��K��������

bool	MnKlinesig[20]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����K�ߵ�һ��TICK���
double  Mn_open[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����K�����ݿ� 
double  Mn_high[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����K�����ݸ�
double  Mn_low[20][60]				= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����K�����ݵ�
double  Mn_close[20][60]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����K��������

//-----------------------
bool	SnifferSignalA[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//ָ�����������
bool	TradingSignalA[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�µ����
bool	SnifferSignalB[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//ָ�����������
bool	TradingSignalB[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//�µ����

//������������
double	Trade_dataA[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������
double	Trade_dataB[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������
double	Trade_dataC[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������
double	Trade_dataD[20][10]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//��������


//���Կ�ƽ��ͳ������
double	Trade_CloseProfit[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//ƽ��ӯ�������Բ�����
double	Trade_Closetimes[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//ƽ�ִ��������Բ�����
double	Day_CloseProfit[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����ƽ�����棬���Բ�����
double	Day_CloseProfitA[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����ƽ�����棬���Բ�����
double	Day_CloseProfitB[20]		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//����ƽ�����棬���Բ�����
double	Day_TradeNumb[20]			= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		//���ִ���ͳ��




void _record0(char *txt)
{
	ofstream o_file(LogFilePaths, ios::app);
	o_file <<  txt << endl; //������д�뵽�ı��ļ���
	o_file.close();						//�ر��ļ�
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
			o_file <<txt1<< dbtoch(tick_data[i][1]) << "_" << tick_data[i][2] << "_" << InstrumentID_n[i] << "_" << InstrumentID_lots[i] <<"_"<< txt2 << "_"<< Trade_dataA[i][5] <<"_"<<Trade_dataA[i][3]<< endl; //������д�뵽�ı��ļ���
		}
		else if (n>4 && n<=6)
		{
			o_file <<txt1<< dbtoch(tick_data[i][1]) << "_" << tick_data[i][2] << "_" << InstrumentID_n[i] << "_" << InstrumentID_lots[i] <<"_"<< txt2 << "_"<< Trade_dataB[i][5] <<"_"<<Trade_dataB[i][3]<< endl; //������д�뵽�ı��ļ���
		}
		
	}
	else
	{
		//																																					--���޸ģ�����
		o_file <<txt1<< dbtoch(tick_data[i][1]) << "_" << tick_data[i][2] << "_" << InstrumentID_n[i] << "_" << InstrumentID_lots[i] <<"_"<< txt2 << "_"<< m<<"_"<<Trade_Closetimes[i]<<"_"<<Trade_CloseProfit[i] << endl; //������д�뵽�ı��ļ���
	}
	o_file.close();						//�ر��ļ�
}

double GetLocalTimeSec1()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	double system_times;
	system_times=(double)((sys_time.wHour)/10e1)+(double)((sys_time.wMinute)/10e3)+(double)((sys_time.wSecond)/10e5);	//��ʽʱ��0.145100
	return system_times;
}
int GetLocalTimeSec2()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	int system_times;
	system_times=(int)((sys_time.wHour)*60*60)+(int)((sys_time.wMinute)*60)+(int)((sys_time.wSecond));					//��ʽʱ��int sec
	return system_times;
}

double GetLocalTimeMs1()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	double system_times;
	system_times=(double)((sys_time.wHour)/10e1)+(double)((sys_time.wMinute)/10e3)+(double)((sys_time.wSecond)/10e5)+(double)((sys_time.wMilliseconds)/10e8);	//��ʽʱ��0.145100500
	return system_times;
}
int GetLocalTimeMs2()
{
	SYSTEMTIME sys_time;  
	GetLocalTime(&sys_time); 
	int system_times;
	system_times=((int)((sys_time.wHour)*60*60)+(int)((sys_time.wMinute)*60)+(int)((sys_time.wSecond)))*1000+(int)((sys_time.wMilliseconds));	//��ʽʱ��int ms
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
		//cout << line << endl;	//��ӡ��������
		//Sleep(100);
		double Day_fprice_t[9];

		for (int j = 0; j < 9; j++)
		{
			word >> Day_fprice_t[j];
			//cout << Day_fprice_t[i][j] << endl;	//��ӡ��������
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
			cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<Trade_dataA[i][1]<<"_"<<Trade_dataA[i][3]<<"_"<< "��"<<"_"<<Trade_dataA[i][5]<< endl;
			}
			else if (Trade_dataA[i][2]<-0.5)
			{
			cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<Trade_dataA[i][1]<<"_"<<Trade_dataA[i][3]<<"_"<< "����"<<"_"<<Trade_dataA[i][5]<< endl;
			}		
		}
		else if (temp1>4 && temp1<7)
		{
			if (Trade_dataB[i][2]>0.5)
			{
			cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<Trade_dataB[i][1]<<"_"<<Trade_dataB[i][3]<<"_"<< "��"<<"_"<<Trade_dataB[i][5]<< endl;
			}
			else if (Trade_dataB[i][2]<-0.5)
			{
			cerr << "--->>> ����: " <<InstrumentID_n[i]<<"_"<<Trade_dataB[i][1]<<"_"<<Trade_dataB[i][3]<<"_"<< "����"<<"_"<<Trade_dataB[i][5]<< endl;
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

	ofstream o_file("./AutoTrader.dat",ios::trunc);		//�����2�����������
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

		//o_file << dbtoch(tick_data[0][1]) << "\t" <<tick_data[i][2] <<"\t"<< Mn_open << " " << Mn_high << " " << Mn_low << " " << Mn_close << endl; //������д�뵽�ı��ļ���
		o_file << dbtoch(tick_data[i][1]) <<"\t"<< Day_open[i][2] << "\t" << Day_high[i][2] << "\t" << Day_low[i][2] << "\t" << Day_close[i][2] <<"\t"<< Day_open[i][1] << "\t" << Day_high[i][1] << "\t" << Day_low[i][1] << "\t" <<Day_close[i][1] << endl; //������д�뵽�ı��ļ���
	}
	o_file.close();						//�ر��ļ�

}

void WriteTradeConfiguration()
{
	string dbtoch(double nums);
	int	tn=0;

	ofstream o_file("./AutoTrader.cfg",ios::trunc);
	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<< Trade_dataA[i][3]<<"\t"<< Trade_dataA[i][1] << "\t" << Trade_dataA[i][2] << "\t" << Trade_dataA[i][3] << "\t" << Trade_dataA[i][4] <<"\t"<< Trade_dataA[i][5] << "\t" << Trade_Closetimes[i] << "\t" << Trade_CloseProfit[i]<< endl; //������д�뵽�ı��ļ���
	}

	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<< Trade_dataB[i][3]<<"\t"<< Trade_dataB[i][1] << "\t" << Trade_dataB[i][2] << "\t" << Trade_dataB[i][3] << "\t" << Trade_dataB[i][4] <<"\t"<< Trade_dataB[i][5] << "\t" << Trade_Closetimes[i] << "\t" << Trade_CloseProfit[i]<< endl; //������д�뵽�ı��ļ���
	}


	o_file.close();						//�ر��ļ�

}

void ErasingTradeConfiguration()
{
	string dbtoch(double nums);
	int	tn=0;
	ofstream o_file("./AutoTrader.cfg",ios::trunc);

	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< endl; //������д�뵽�ı��ļ���
	}

	for (int i = 0; i < 20; i++)
	{
		o_file << dbtoch(tick_data[i][1]) <<"\t"<<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<<"\t"<<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< "\t" <<tn<< endl; //������д�뵽�ı��ļ���
	}

	o_file.close();						//�ر��ļ�

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


string dbtoch(double nums)		//��double ת��Ϊ string ���б���
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


string dbtoch2(double nums)		//��double ת��Ϊ string ���б���
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

//����-�޼�
void SendOrder(TThostFtdcInstrumentIDType FuturesId,int BuySell,int OpenClose,int md,int i)
{

	strcpy(INSTRUMENT_ID,InstrumentID_n[i]);
	//cerr << "--->>> ����¼������: " <<INSTRUMENT_ID<< endl;

	DIRECTION = BuySell;
	MARKETState = OpenClose;
	
	//Sleep(050);

	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy_s(req.BrokerID, BROKER_ID);
	///Ͷ���ߴ���
	strcpy_s(req.InvestorID, INVESTOR_ID);
	///��Լ����											//INSTRUMENT_ID
	strcpy_s(req.InstrumentID, INSTRUMENT_ID);
	///��������
	//strcpy_s(req.OrderRef, ORDER_REF);				//�µ�ǰ�޸�
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;

	///��������:										//THOST_FTDC_D_Buy,THOST_FTDC_D_Sell
	if (BuySell==0)
	{
	req.Direction = THOST_FTDC_D_Buy;
	}
	else if (BuySell==1)
	{
	req.Direction = THOST_FTDC_D_Sell;
	}

	///��Ͽ�ƽ��־: ����								//THOST_FTDC_OF_Open,THOST_FTDC_OF_Close,THOST_FTDC_OF_CloseToday
	if (OpenClose==0)
	{
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	}
	else if(OpenClose==1)
	{
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;		//����������ƽ��ƽ������ƽ��
	}
	else if(OpenClose==3)
	{
	req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;	//����ƽ��
	}

	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	//Ͷ��

	///�۸�
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

	///����: 1											//     ��ƽ������
	req.VolumeTotalOriginal = InstrumentID_lots[i];						
	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 1;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	req.IsAutoSuspend = 0;
	///ҵ��Ԫ
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
	req.UserForceClose = 0;
	
	if ( RunMode && InstrumentID_En[i]==1 )
	{
	///��������
	int iNextOrderRef = atoi(ORDER_REF);
	iNextOrderRef++;
	sprintf(ORDER_REF,"%d",iNextOrderRef);
	strcpy_s(req.OrderRef, ORDER_REF);
	//sprintf(ORDERACTION_REF[i], ORDER_REF);	//����

	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);		//ʵ�̣�����ʽ�µ�
	cerr << "--->>> ����¼������: " <<InstrumentID_n[i]<< ((iResult == 0) ? " �ɹ�" : " ʧ��") << endl;
	}
	else
	{
	int iResult = 0;	//���ԣ��������µ�
	//cerr << "--->>> ����¼������: " <<InstrumentID_n[i]<< ((iResult == 0) ? " �ɹ�" : " ʧ��") << endl;
	}

}
