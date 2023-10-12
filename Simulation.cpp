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

extern	double  Day_open[20][60];	//����K�߿�
extern	double  Day_high[20][60];	//����K�߸�
extern	double  Day_low[20][60];		//����K�ߵ�
extern	double  Day_close[20][60];	//����K����

extern double	tick_data[20][10];				//TICK��������
extern	double	tick_AskPrice1[20][60];			//��һ��
extern	double	tick_BidPrice1[20][60];			//��һ��
extern	double	tick_AskVolume1[20][60];		//��һ��
extern	double	tick_BidVolume1[20][60];		//��һ��
extern	double	tick_Volume[20][60];			//�ɽ���
extern	double	tick_OpenInterest[20][60];		//�ֲ���

extern	string	InstrumentID_name;	//
extern	char    LogFilePaths[80];	//������־

extern	double  Day_open[20][60];	//��K�߿�
extern	double  Day_high[20][60];	//��K�߸�
extern	double  Day_low[20][60];	//��K�ߵ�
extern	double  Day_close[20][60];	//��K����

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
	

    //�����ļ���·��  
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
			cout<<"׼�����Ե�"<<n<<"���ļ���"<<endl;
		
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
	//cout << Readfilepaths << endl;	//��ӡ��������

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

	_record0("��ʼ���ԣ�����");
	int t0=0;	

	while(fin.getline(line, sizeof(line)))
	{
		std::stringstream word(line);
		//cout << line << endl;	//��ӡ��������
		//Sleep(100);

		t0=t0+1;
		
		for (int i = 0; i < 20; i++)
		{	
			
			if (InstrumentID_n[i]==str0 && testfile)
			{
				
				InstrumentID_name=InstrumentID_n[i];
				tick_data[i][0]= 1;//���ñ�־λ
				ReceiveTick[i] = true;

				for (int j = 0; j < 10; j++)
				{
					word >> data[j];
					//cout << "Configuration:" << data[j] << endl;	//��ӡĳ������ 
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
		
			//cout << "Configuration:" << tick_data[i][2] << endl;	//��ӡĳ������
			ReceiveTick[i] = false;
				//}

				Sniffer();
				Trading();
				Sniffer();
				Trading();			//�ظ���Ϊ�˺�ʵ��һ�£�1��tick�յ�����Կ��ܱ�������С�
				//Sleep(100);
				//cout << "Configuration:2_" << setprecision(9) <<GetLocalTimeMs1() << endl;	//��ӡĳ������
			}


		}

	}

	fin.clear();
	fin.close();

}
