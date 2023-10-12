/////////////////////////////////////////////////////////////////////////
///名称：AutoTrader
///版本：v1.0.x
///日期：2014xxxx
///说明：基于上期CTP技术
/////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"

#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcTraderApi *pUserApi;

/**
 * 一些全局配置参数
 * 用户、密码、经纪商信息等
 *
 * external  的，在【AutoTrader.cpp】里面导入时    已有赋值。
 */
// 配置参数
extern char BROKER_ID[];        // 经纪公司代码
extern char INVESTOR_ID[];        // 投资者代码
extern char PASSWORD[];            // 用户密码

extern TThostFtdcInstrumentIDType INSTRUMENT_ID;    // 合约代码
extern int DIRECTION;            // 买卖方向
extern int MARKETState;        // 开平仓
extern double LIMIT_PRICE;    // 价格

// 请求编号
extern int iRequestID;
extern char LogFilePaths[80];                //交易日志
extern TThostFtdcDateExprType TradingDay;

extern double Trade_LastDate[20];
extern char InstrumentID_n[20][10];
extern int InstrumentID_lots[20];
extern double InstrumentID_minmove[20];
// 会话参数
TThostFtdcFrontIDType FRONT_ID;        //前置编号
TThostFtdcSessionIDType SESSION_ID;        //会话编号
TThostFtdcOrderRefType ORDER_REF;        //报单引用
TThostFtdcOrderActionRefType ORDERACTION_REF[20];    //撤单引用

/**
 * 当前置机连接成功后的初始化
 *
 */
/**
*
* 此处，有一个特点—————— 这个作者设计的  spi 兼具了  spi接收  和  api发送  的功能。
*/
void CTraderSpi::OnFrontConnected() {
    Sleep(3000);
    cerr << "--->>> " << "交易登录中..." << endl;

    ///用户登录请求
    CTraderSpi::ReqUserLogin();
}

/**
 * 发送登录请求
 *
 * 生成登录请求报文,发送登录请求
 *
 */
void CTraderSpi::ReqUserLogin() {
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);
    int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
    cerr << "--->>> 发送交易用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

/**
 * 收到登录响应
 *
    1.保存会话参数
    2.确认结算信息 ReqSettlementInfoConfirm()
    3.其他初始化操作
 *
 */
void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    int GetLocalTimeMs2();
    cerr << "--->>> " << __FUNCTION__ << endl;

    char times[10];

    strcpy(times, pRspUserLogin->SHFETime);

    string str1 = times;
    string str2 = times;
    string str3 = times;

    str1 = str1.substr(0, 2);        //
    str2 = str2.substr(3, 2);        //
    str3 = str3.substr(6, 2);        //

    int hours = atoi(str1.c_str());
    int minutes = atoi(str2.c_str());
    int seconds = atoi(str3.c_str());
    int Millisecs = 0;

    //BATtime和本地时间差，用于估算分钟开盘时间，刚登录时采用与上期所时间差进行初始化。
    //Timeoffset_Tick=(((hours*60*60+minutes*60+seconds)*1000+Millisecs)-GetLocalTimeMs2());

    if (IsErrorRspInfo(pRspInfo)) {
        cerr << "--->>> 交易登录错误: " << pRspInfo->ErrorID << pRspInfo->ErrorMsg << endl;
    }
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        // 保存会话参数
        FRONT_ID = pRspUserLogin->FrontID;
        SESSION_ID = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        iNextOrderRef++;
        sprintf(ORDER_REF, "%d", iNextOrderRef);
        //cerr << "--->>> 报单引用 = " << ORDER_REF << endl;
        ///获取当前交易日
        strcpy(TradingDay, pUserApi->GetTradingDay());
        cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay() << endl;
        ///投资者结算结果确认
        ReqSettlementInfoConfirm();
    }
}

/**
 *  确认结算信息
 *
 *  生成结算信息确认请求,发送
 */
void CTraderSpi::ReqSettlementInfoConfirm() {
    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    cerr << "--->>> 确认结算... " << endl;
    //请求查询结算信息确认
    int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
    cerr << "--->>> 投资者结算结果确认: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

/**
 * 收到结算信息确认响应
 *
    1.查询合约信息 ReqQryInstrument()
    2.查询资金信息 ReqQryTradingAccount()
 */
void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
                                            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        ///请求查询合约
        //for (int i = 0; i < 20; i++)
        //{
        //strcpy(INSTRUMENT_ID,InstrumentID_n[1]);
        ReqQryInstrument();
        //Sleep(1000);
        //}
    }
}

/**
 * 查询合约信息
 *              生成合约查询请求,发送
 */
void CTraderSpi::ReqQryInstrument() {
    CThostFtdcQryInstrumentField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
    //cerr << "--->>> 请求查询合约: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

/**
 * 收到合约信息响应
 *              查询资金账户信息 ReqQryTradingAccount();
 */
void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                               bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        ///请求查询合约

        //strcpy(INSTRUMENT_ID,pInstrument->InstrumentID);
        //for (int i = 0; i < 20; i++)
        //{
        //if (INSTRUMENT_ID==InstrumentID_n[i])
        //{
        //Trade_LastDate[i]=0;//pInstrument->ExpireDate;
        //cerr << "--->>> 合约最后交易日: " <<pInstrument->InstrumentID<<"_"<< pInstrument->ExpireDate << endl;
        //InstrumentID_minmove[i]=pInstrument->PriceTick;
        //}
        //}

        ReqQryTradingAccount();
    }
}

/**
 * 查询资金账户
 *              生成资金账户查询请求,发送
 */
void CTraderSpi::ReqQryTradingAccount() {
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
    cerr << "--->>> 请求查询资金账户: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

/**
 * 收到资金账户响应
 *              查询持仓 ReqQryInvestorPosition();
 */
void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        cerr << "--->>> 交易日: " << pTradingAccount->TradingDay << endl;
        cerr << "--->>> 可用资金: " << pTradingAccount->Available << endl;
        //cerr << "--->>> 可取资金: " << pTradingAccount->WithdrawQuota  << endl;
        //静态权益=上日结算-出金金额+入金金额
        double preBalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;
        //cerr << "--->>> 静态权益: " << preBalance  << endl;
        //动态权益=静态权益+ 平仓盈亏+ 持仓盈亏- 手续费
        double current = preBalance + pTradingAccount->CloseProfit + pTradingAccount->PositionProfit -
                         pTradingAccount->Commission;
        //cerr << "--->>> 动态权益: " << current  << endl;

        //检查交易日志文件是否存在，是否需要新建文本文件
        if (LogFilePaths[0] == '\0') {
            strcpy(LogFilePaths, "./AutoTrade/");
            strcat_s(LogFilePaths, pTradingAccount->TradingDay);
            strcat_s(LogFilePaths, ".txt");

            //检查文件是否存在，是否需要新建文本文件
            ifstream inf;
            ofstream ouf;
            inf.open(LogFilePaths, ios::out);
        }

        ///请求查询投资者持仓
        //for (int i = 0; i < 20; i++)
        //{
        strcpy(INSTRUMENT_ID, InstrumentID_n[3]);
        ReqQryInvestorPosition();
        //}
    }
}

/**
 * 查询持仓
 *              生成持仓查询请求,发送
 */
void CTraderSpi::ReqQryInvestorPosition() {
    CThostFtdcQryInvestorPositionField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
    cerr << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;
}



///请求查询投资者持仓响应

/**
 * 收到持仓响应
 *              合约费率查询 ReqQryInstrumentMarginRate();
 */
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition,
                                          CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    cerr << "--->>> " << "投资者持仓数量:" << "0" << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
        ///合约保证金以及手续费查询请求
        //for (int i = 0; i < 20; i++)
        //{
        strcpy(INSTRUMENT_ID, InstrumentID_n[3]);
        ReqQryInstrumentMarginRate();
        ReqQryInstrumentCommissionRate();
        Sleep(1000);
        //}

    }

    //END

}

///请求查询合约保证金率
/**
 * 合约费率/保证金率  查询
 *              生成合约费率查询请求,发送
 */
void CTraderSpi::ReqQryInstrumentMarginRate() {
    //cerr << "--->>> 查询合约保证金率"<< endl;
    CThostFtdcQryInstrumentMarginRateField req;
    memset(&req, 0, sizeof(req));

    ///经纪公司代码
    strcpy(req.BrokerID, BROKER_ID);
    ///投资者代码
    strcpy(req.InvestorID, INVESTOR_ID);
    ///合约代码
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    //////投机套保标志
    req.HedgeFlag = THOST_FTDC_HF_Speculation;    //投机

    int iResult = pUserApi->ReqQryInstrumentMarginRate(&req, ++iRequestID);
    //cerr << "--->>> 查询合约保证金率: " << ((iResult == 0) ? " 成功" : " 失败") << endl;
}


///请求查询合约保证金率响应
/**
 * 收到合约费率/保证金率  响应
 *              记录合约费率
 */
void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate,
                                              CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    double bzj_b = 100 * (pInstrumentMarginRate->LongMarginRatioByMoney);
    cerr << "--->>> " << "合约保证金率:" << pInstrumentMarginRate->InstrumentID << "_" << bzj_b << "%" << endl;
}

///请求查询合约手续费率
/**
 * 生成查询合约手续费率的请求,并发送给前置机。
 *
    包含以下主要参数:

                    经纪商代码
                    投资者代码
                    合约代码
                    投保标志(投机/套保)
                    用于获取合约的手续费率。

 */
void CTraderSpi::ReqQryInstrumentCommissionRate() {
    //cerr << "--->>> 查询合约手续费率"<< endl;
    CThostFtdcQryInstrumentCommissionRateField req;
    memset(&req, 0, sizeof(req));

    ///经纪公司代码
    strcpy(req.BrokerID, BROKER_ID);
    ///投资者代码
    strcpy(req.InvestorID, INVESTOR_ID);
    ///合约代码
    strcpy(req.InstrumentID, INSTRUMENT_ID);

    int iResult = pUserApi->ReqQryInstrumentCommissionRate(&req, ++iRequestID);
    //cerr << "--->>> 查询合约手续费率: " << ((iResult == 0) ? " 成功" : " 失败") << endl;
}


///请求查询合约手续费率响应
/**
 * 合约手续费率查询回报
 *
 *
 * 可以采用的流程：

            主要处理流程:

                            通过pRspInfo判断查询是否成功
                            如果成功,遍历所有返回的合约手续费率记录
                            缓存或更新合约的开仓、平今手续费率到本地记录
                            打印查询到的手续费率信息
                            如果不是最后一条记录,继续等待其余的返回
                            如果是最后一条,处理查询结束,可触发后续业务

 */
void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
                                                  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    double sxf_w = 100 * 100 * (pInstrumentCommissionRate->OpenRatioByMoney);
    cerr << "--->>> " << "合约手续费率:" << pInstrumentCommissionRate->InstrumentID << "_" << sxf_w << "%%_"
         << pInstrumentCommissionRate->CloseTodayRatioByVolume << "RMB" << endl;
}


//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


//报单-限价,暂时未使用
/**
 * 用于生成报单内容,发送报单录入请求,包含报单价格、数量、开平仓方向等信息。
 */
void CTraderSpi::ReqOrderInsert() {
    //const char bsstat;
    //char bsopenstat;

    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    ///经纪公司代码
    strcpy(req.BrokerID, BROKER_ID);
    ///投资者代码
    strcpy(req.InvestorID, INVESTOR_ID);
    ///合约代码
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    ///报单引用
    strcpy(req.OrderRef, ORDER_REF);
    ///用户代码
//	TThostFtdcUserIDType	UserID;
    ///报单价格条件: 限价
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ///买卖方向:
    req.Direction = DIRECTION;
    ///组合开平标志: 开仓
    req.CombOffsetFlag[0] = MARKETState;
    ///组合投机套保标志
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    ///价格
    req.LimitPrice = LIMIT_PRICE;
    ///数量: 1
    req.VolumeTotalOriginal = 1;
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

    //int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
    //cerr << "--->>> 报单录入请求: " << ((iResult == 0) ? " 成功" : " 失败") << endl;
}



// 报单后，如不能通过THOST校验，THOST拒绝报单，返回OnRspOrderInsert（含错误编码）。如果校验通过，THOST接收报单，转发给交易所
// 交易所收到报单后，通过校验。用户会收到OnRtnOrder、OnRtnTrade。
// 如果交易所认为报单错误，用户就会收到OnErrRtnOrder
/**
 * 当收到报单录入请求的响应时调用,用于处理报单被接受或报单错误的回报信息。

    包含以下处理:

            打印报单回报内容
            通过IsErrorRspInfo判断是否报单失败
            如果接受报单,记录相关数据
            如果报单失败,进行错误处理

 *
 */
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                             bool bIsLast) {
    double GetLocalTimeSec1();

    cerr << "--->>> " << __FUNCTION__ << endl;
    double system_times = GetLocalTimeSec1();//格式时间0.145100
    //cerr << "--->>> 报单录入响应: "<<pInputOrder->InstrumentID<< endl;
    cerr << "--->>> 报单响应信息: " << pInputOrder->InstrumentID << "_" << ((pInputOrder->Direction == '0') ? "买" : "卖")
         << "_" << ((pInputOrder->CombOffsetFlag[0] == '0') ? "开仓" : "平仓") << "_" << pInputOrder->LimitPrice << endl;
    IsErrorRspInfo(pRspInfo);
}

//报单-修改，撤单
/**
 * 用于生成报单撤单或者修改请求,包含原始报单信息和操作标志等。

    主要参数:

                    pOrder:原始报单信息,提供报单参数
                    ActionFlag:操作标志,撤单或修改
                    其他报单操作需要的信息

    调用此函数可以发送报单撤单或修改请求。

 */
void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder) {
    static bool ORDER_ACTION_SENT = false;        //是否发送了报单
    if (ORDER_ACTION_SENT)
        return;

    CThostFtdcInputOrderActionField req;
    memset(&req, 0, sizeof(req));
    ///经纪公司代码
    strcpy(req.BrokerID, pOrder->BrokerID);
    ///投资者代码
    strcpy(req.InvestorID, pOrder->InvestorID);
    ///报单操作引用
//	TThostFtdcOrderActionRefType	OrderActionRef;
    ///报单引用
    strcpy(req.OrderRef, pOrder->OrderRef);
    ///请求编号
//	TThostFtdcRequestIDType	RequestID;
    ///前置编号
    req.FrontID = FRONT_ID;
    ///会话编号
    req.SessionID = SESSION_ID;
    ///交易所代码
//	TThostFtdcExchangeIDType	ExchangeID;
    ///报单编号
//	TThostFtdcOrderSysIDType	OrderSysID;
    ///操作标志
    req.ActionFlag = THOST_FTDC_AF_Delete;
    ///价格
//	TThostFtdcPriceType	LimitPrice;
    ///数量变化
//	TThostFtdcVolumeType	VolumeChange;
    ///用户代码
//	TThostFtdcUserIDType	UserID;
    ///合约代码
    strcpy(req.InstrumentID, pOrder->InstrumentID);
    //cerr << "--->>> 报单操作请求: "<< endl;
    int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
    cerr << "--->>> 报单撤单请求: " << pOrder->InstrumentID << "_" << ((pOrder->Direction == '0') ? "买" : "卖") << "_"
         << ((pOrder->CombOffsetFlag[0] == '0') ? "开仓" : "平仓") << ((iResult == 0) ? " 成功" : " 失败") << endl;
    ORDER_ACTION_SENT = true;
}

/**
 * 当收到报单撤单或者修改请求的响应时调用,用于处理请求的回报。

    主要包含:

            打印撤单回报内容
            通过IsErrorRspInfo判断是否撤单成功
            如果成功,记录相关信息
            如果失败,进行错误处理

 */
void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo,
                                  int nRequestID, bool bIsLast) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    IsErrorRspInfo(pRspInfo);

}

///报单通知
/**
 * 用于接收报单回报,可以根据回报进行撤单或者其他处理
 *

OnRtnOrder()是报单回报的回调函数,用于接收并处理报单状态的推送更新。

主要参数:

        pOrder: 报单信息,包含报单状态、报单编号、合约代码、价格、数量等信息。

主要处理流程:

        判断报单是否是自己发出的(通过会话编号、报单引用等判断)
        如果是自己的报单,进一步判断报单状态 2.1 如果报单正在交易,则调用ReqOrderAction()发送撤单请求 2.2 如果报单已撤销,打印提示报单撤销成功
        根据报单状态更新相关记录,如委托成功、部分成交、完全成交等
        根据报单信息进行其他业务处理
        打印报单信息用于确认接收到报单回报

所以OnRtnOrder主要作用是接收报单状态的实时推送,然后根据报单信息进行撤单、记录更新、业务处理等操作。

 */
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {

    char str[10];
    int i = 0;
    sprintf(str, "%d", pOrder->OrderSubmitStatus);
    int OrderState = atoi(str) - 48;    //报单状态0=已经提交，3=已经接受
    //cerr << "--->>> " << __FUNCTION__ <<"_"<< OrderState << endl;

    if (IsMyOrder(pOrder)) {
        if (IsTradingOrder(pOrder)) {
            //ReqOrderAction(pOrder);	//撤单
            cout << "--->>> 正在执行！" << endl;
            i = 1;
        } else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
            cout << "--->>> 撤单成功！" << endl;
    }
}

///成交通知
/**
 * 用于接收成交回报,可以记录成交信息

 */
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
    cerr << "--->>> " << __FUNCTION__ << endl;
    cerr << "--->>> 成交通知:" << pTrade->TradingDay << "_" << pTrade->TradeTime << "_" << pTrade->InstrumentID << "_"
         << ((pTrade->Direction == '0') ? "买" : "卖") << "_" << ((pTrade->OffsetFlag == '0') ? "开仓" : "平仓") << "_"
         << pTrade->Volume << "_" << pTrade->Price << endl;
}

/**
 * 断开连接回报
 */
void CTraderSpi::OnFrontDisconnected(int nReason) {
    cerr << "--->>> " << "OnFrontDisconnected!" << endl;
    //cerr << "--->>> Reason = " << nReason << endl;
    //cerr << "--->>> " << "交易断开! Reason ="+nReason << endl;
}

/**
 * 心跳超时回报
 */
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse) {
    cerr << "--->>> " << "OnHeartBeatWarning" << endl;
    cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

/**
 * 用于接收错误回报,可以获取错误信息
 *
 */
void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    cerr << "--->>> " << "OnRspError" << endl;
    IsErrorRspInfo(pRspInfo);
}

/**
 * 判断一个回报是否包含错误信息
 */
bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo) {
    // 如果ErrorID != 0, 说明收到了错误的响应
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
        cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

/**
 * 判断一个报单回报是否是自己发出的
 */
bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder) {
    //cerr << "--->>> " << "IsMyOrder" << endl;
    return ((pOrder->FrontID == FRONT_ID) &&
            (pOrder->SessionID == SESSION_ID) &&
            (strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

/**
 * 判断一个报单是否正在交易中
 */
bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder) {
    //cerr << "--->>> " << "IsTradingOrder" << endl;
    return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
            (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
            (pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

