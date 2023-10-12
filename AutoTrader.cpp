/////////////////////////////////////////////////////////////////////////
///名称：AutoTrader
///版本：v1.0.2
///日期：20140701
///说明：基于上期CTP技术  6.2.0_20131205_tradeapi_windows
///版本：ctp 6.2.0_20131205   
///作者：小平常心
///  QQ：1396144618
///共享给大家，你可以打造完全属于自己的自动交易所系统了
/////////////////////////////////////////////////////////////////////////
// AutoTrader.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <windows.h>
#include <time.h>
#include <algorithm>

#include <stdlib.h>
using namespace std;

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"
#include "Common.h"
#include "DataSniffer.h"
#include "MyTrader.h"

#pragma warning(disable : 4996)

// UserApi对象
CThostFtdcTraderApi *pUserApi;
// MdApi对象
CThostFtdcMdApi *pMdApi;

/**
 * 定义了常量和全局变量:包括运行模式、CTP的前置地址、用户名密码等。
 */

int		RunMode = 0;		//运行模式选择，0=本地测试，1=实盘运行 ，提示Common.h：设置InstrumentID_En=0，实盘在线仿真模式，InstrumentID_En=1，实盘在线交易模式，

// 配置参数
char  FRONT_ADDR_1A[] = "tcp://180.168.212.51:41205";		// 前置地址1交易:实盘
char  FRONT_ADDR_1B[] = "tcp://180.168.212.51:41213";		// 前置地址1行情:实盘
char  FRONT_ADDR_2A[] = "tcp://180.168.212.52:41205";		// 前置地址2交易:实盘
char  FRONT_ADDR_2B[] = "tcp://180.168.212.53:41213";		// 前置地址2行情:实盘
char  FRONT_ADDR_3A[] = "tcp://180.168.212.53:41205";		// 前置地址3交易:实盘 盘后
char  FRONT_ADDR_3B[] = "tcp://180.168.212.53:41213";		// 前置地址3行情:实盘 盘后

char  FRONT_ADDR_6A[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";	// 前置地址6行情:仿真 17:00开始
char  FRONT_ADDR_6B[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// 前置地址6行情:仿真 17:00开始

TThostFtdcBrokerIDType	BROKER_ID = "2030";								// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "88888888";						// 投资者代码
TThostFtdcPasswordType  PASSWORD = "123456";							// 用户密码
//TThostFtdcBrokerIDType	BROKER_ID = "2030";							// 经纪公司代码:仿真
//TThostFtdcInvestorIDType INVESTOR_ID = "00069";						// 投资者代码:仿真"00069","00092"
//TThostFtdcPasswordType  PASSWORD = "888888";							// 用户密码:仿真

//char *ppInstrumentID[] = {"rb1410"};									// 行情订阅列表
//int iInstrumentID = 1;												// 行情订阅数量
//							0		1			2		  3			4		 5			6		   7		8			9		10		11		12		13		14		15		  16		17	   18		 19
char *ppInstrumentID[] = {  "i1409", "jm1409", "j1409", "rb1410","rb1501", "TA409", "l1409","ru1409", "ru1501", "jd1409", "RM409", "m1409","y1501", "p1501","ag1506","ag1412","cu1408","cu1409","IF1409","IF1407"};//行情订阅列表
//char *ppInstrumentID[] = {  "ag1412"};//行情订阅列表
int iInstrumentID = 1;													// 行情订阅数量

// User行情数据

// 声明会话参数

// 声明外部函数


void main(void)
{
	void Erasefiles();
	bool ReadMdConfiguration();	
	bool ReadTradeConfiguration();
    // _record0用于记录日志。
	void _record0(char *txt);
	void Sniffer();
	void Trading();
	void test();
    // SendOrder()用于发送订单。
	void SendOrder(TThostFtdcInstrumentIDType FuturesId,int BuySell,int OpenClose,int i);


	JustRun = true;													//正在启动标志
    /**
     * Erasefiles()清除文件。
     */
	Erasefiles();
	Sleep(2000);

	cerr << "--->>> " << "Welcom MyAutoTrader System!" << endl;
	cerr << "--->>> " << "Version 1.0.3!" << endl;

    /**
     * ReadMdConfiguration()和ReadTradeConfiguration()用于读取配置。
     */
	ReadMdConfiguration();
	Sleep(2000);

    /**
     * 调用函数初始化UserApi和MdApi对象,连接行情和交易前置。
     */

	// 初始化UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./thosttraderapi.dll");			// 创建UserApi//"./thosttraderapi.dll"
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			// 注册私有流
	pUserApi->RegisterFront(FRONT_ADDR_1A);							// connect
	//pUserApi->RegisterFront(FRONT_ADDR_2A);						// connect
	
	if (RunMode)
	{
		pUserApi->Init();
		cerr << "--->>> " << "Initialing UserApi" << endl;
	}
	

	// 初始化MdApi
	pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll");					// 创建MdApi//"./thostmduserapi.dll"
	CThostFtdcMdSpi* pMdSpi = new CMdSpi();
	pMdApi->RegisterSpi(pMdSpi);									// 注册事件类
	pMdApi->RegisterFront(FRONT_ADDR_1B);							// connect
	//pMdApi->RegisterFront(FRONT_ADDR_2B);							// connect

	if (RunMode)    // 实盘
	{
		pMdApi->Init();
		cerr << "--->>> " << "Initialing MdApi" << endl;
	}
	else
	{
		cerr << "--->>> " << "Test Mode!" << endl;
	}
	
	//pMdApi->Join();
	//pMdApi->Release();
	
	Sleep(7000);	
	ReadTradeConfiguration();
	Sleep(1000);
	cerr << "--->>> " << "初始化完成!" << endl;

    /**
     * 循环调用Sniffer()和Trading()函数。
     */
	while(RunMode)	//实盘
	{
        /**
         * Sniffer()可能是行情采集和处理函数。
         */
		Sniffer();

        /**
         * Trading()可能是生成和发送交易指令的函数。
         */
		Trading();
		Sleep(050);
	}

    /**
     * 有一个本地测试模式,在该模式下调用test()函数进行测试。
     */
	while(!RunMode)	//本地测试
	{
		if (JustRun==true)
		{
			test();	//本地回测
			JustRun=false;
		}
	}



}

