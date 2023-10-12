#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <cmath>
#include <io.h> 
using namespace std;

#pragma warning(disable : 4996)
extern	bool	ReceiveTick[20];
extern char		InstrumentID_n[20][10];
extern	double  InstrumentID_diff[20];			//
extern	double  InstrumentID_minmove[20];	

extern	double  Day_open[20][60];	//分钟K线开
extern	double  Day_high[20][60];	//分钟K线高
extern	double  Day_low[20][60];		//分钟K线低
extern	double  Day_close[20][60];	//分钟K线收

extern double	tick_data[20][10];				//TICK基本数据
extern	double	tick_AskPrice1[20][60];			//买一价
extern	double	tick_BidPrice1[20][60];			//卖一价
extern	double	tick_AskVolume1[20][60];		//买一量
extern	double	tick_BidVolume1[20][60];		//卖一量
extern	double	tick_Volume[20][60];			//成交量
extern	double	tick_OpenInterest[20][60];		//持仓量

extern	string	InstrumentID_name;	//
extern	char    LogFilePaths[80];	//交易日志

extern	double  Day_open[20][60];	//日K线开
extern	double  Day_high[20][60];	//日K线高
extern	double  Day_low[20][60];	//日K线低
extern	double  Day_close[20][60];	//日K线收

extern	double	Day_CloseProfit[20];
extern	double	Day_CloseProfitA[20];
extern	double	Day_CloseProfitB[20];

extern  double	Day_TradeNumb[20];

extern	int		Q_BarTime_1n[20];	//
bool testfile=false;

void test()
{
	void Simulation_onefile(char  *pathstt);
	
	_finddata_t file;  
    long lf;  
	int n=0;
	

    //输入文件夹路径  
	if( (lf = _findfirst("..\\Bin\\testdata\\*.*", &file)) ==-1 )  
		cout<<"Not Found!"<<endl;  
	else
	{ 
		while(_findnext( lf, &file)==0)
		{  
			cout<<file.name<<endl;
			Sleep(1000);

			if (n>0)
			{
			cout<<"准备测试第"<<n<<"个文件！"<<endl;
		
		    string str=file.name;
			string str0="";
			string str2=".txt";

			size_t iPos = str.find(".");
			str0 = str.substr(iPos,4);

				if (str0==str2.c_str())
				{
					testfile=true;
				}
				else
				{
					testfile=false;
				}

			char *str1=file.name;
			Simulation_onefile(str1);

			}
			n=n+1;
		}

	}
	_findclose(lf);  

}



void Simulation_onefile(char  *pathstt)
{

	void Sniffer();
	void Sniffer_test();
	void Trading();
	void _record0(char *txt);	
	//double GetLocalTimeMs1();
	//string dbtoch(double nums);

	char Readfilepaths[50]="";
	strcpy(Readfilepaths, "./testdata/");
	strcat(Readfilepaths,pathstt);
	//cout << Readfilepaths << endl;	//打印整行数据

	ifstream fin(Readfilepaths, std::ios::in);

	string str=pathstt;
	string str0="";
	size_t iPos = str.find("_");
	str0 = str.substr(0, iPos);

	strcpy(LogFilePaths, "./Simulation/Simulation_");
	strcat(LogFilePaths,str0.c_str());
	strcat(LogFilePaths, ".txt");

	Sleep(3000);
	char line[1024]={0};
	vector < double > data(16);

	_record0("开始测试！！！");
	int t0=0;	

	while(fin.getline(line, sizeof(line)))
	{
		std::stringstream word(line);
		//cout << line << endl;	//打印整行数据
		//Sleep(100);

		t0=t0+1;
		
		for (int i = 0; i < 20; i++)
		{	
			
			if (InstrumentID_n[i]==str0 && testfile)
			{
				
				InstrumentID_name=InstrumentID_n[i];
				tick_data[i][0]= 1;//设置标志位
				ReceiveTick[i] = true;

				for (int j = 0; j < 10; j++)
				{
					word >> data[j];
					//cout << "Configuration:" << data[j] << endl;	//打印某个数据 
				}
				

				Q_BarTime_1n[i]=(int(data[1]*100))*60*60 + (int((data[1]*100-int(data[1]*100))*100))*60 + (int((data[1]*10000-int(data[1]*10000))*100));

				tick_data[i][1]=data[0];	
				tick_data[i][2]=data[1];

				if ( (tick_data[i][2]>0.0856 && tick_data[i][2]<0.0900 && i<=13) || (tick_data[i][2]>0.2056 && tick_data[i][2]<0.2100 && i>13 && i<=17) || (tick_data[i][2]>0.0913 && tick_data[i][2]<0.0915 && i>17) )
				{
					Day_open[i][0]=data[3];
					Day_CloseProfit[i]=0;
					Day_CloseProfitA[i]=0;
					Day_CloseProfitB[i]=0;

				}

			for (int j = 59; j > 0; j--)
			{
			tick_AskPrice1[i][j]	= tick_AskPrice1[i][j-1];
			tick_BidPrice1[i][j]	= tick_BidPrice1[i][j-1];
			tick_AskVolume1[i][j]	= tick_AskVolume1[i][j-1];
			tick_BidVolume1[i][j]	= tick_BidVolume1[i][j-1];
			tick_Volume[i][j]		= tick_Volume[i][j-1];
			tick_OpenInterest[i][j] = tick_OpenInterest[i][j-1];
			}

			tick_AskPrice1[i][0]	= data[3];
			tick_BidPrice1[i][0]	= data[5];
			tick_AskVolume1[i][0]	= data[4];
			tick_BidVolume1[i][0]	= data[6];
			tick_Volume[i][0]		= data[8];
			tick_OpenInterest[i][0] = data[9];
		
			//cout << "Configuration:" << tick_data[i][2] << endl;	//打印某个数据
			ReceiveTick[i] = false;
				//}

				Sniffer();
				Trading();
				Sniffer();
				Trading();			//重复是为了和实际一致，1个tick收到后策略可能被多次运行。
				//Sleep(100);
				//cout << "Configuration:2_" << setprecision(9) <<GetLocalTimeMs1() << endl;	//打印某个数据
			}


		}

	}

	fin.clear();
	fin.close();

}
