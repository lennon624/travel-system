/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#pragma once
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
using namespace std;
//#define MAX_TIME	24
//#define CITY_COUNT	4

//
//enum class vehicle { bus = 0, train = 1, plane = 2 };
//
///*
//	Discription:	交通工具属性
//	Creation Date:	2020/5/27
//*/
//struct VehAttr {
//	string m_name;
//	int m_interval;		/*fix interval*/
//	int m_distTimes;	/*速度,相比distMap中的慢多少倍*/
//};
//
///*声明所有交通工具的属性*/
//extern const vector<VehAttr> vehAttrs;

class Vehicle
{
public:
	Vehicle();
	~Vehicle();

	typedef unsigned char Type;

	/*定义各种交通方式的标志,用二进制的一个位表示*/
	static const Type bus = 0x01;
	static const Type train = 0x02;
	static const Type plane = 0x04;

	static const Type all = bus | train | plane;
	
	struct Attribute {
		string  name;
		int m_interval;	//两次航班的固定间隔
		int m_distTimes;//速度,需要的时间是distMap[a][b]的多少倍
	};
	
	//获取属性,返回Attribute
	static const Vehicle::Attribute GetAttribute(Type);

private:

};

/*
	Discription:	城市信息
	Creation Date:	2020/5/26
*/



struct City {
	string m_name;		/*城市名称*/
	float m_risk;		/*城市风险指数*/
	City(const string& cityName, float riskPoint)
		:m_name(cityName), m_risk(riskPoint) {}

	//默认构造
	City() :m_name("Beijing"), m_risk(1.0) {}
};

/*
	Discription:	表示一个航班、汽车、车次
	Creation Date:	2020/5/26
*/
struct Transport
{
	/*TODO 把各个类的构造函数给完善一下吧*/
	Transport(const City& src, const City& dest, Vehicle::Type means,
		int startTime, int endTime, int startDay, int endDay)
		:m_srcCity(src), m_destCity(dest), m_means(means),
		m_startTime(startTime), m_endTime(endTime), m_startDay(startDay), m_endDay(endDay) {}
	
	//默认构造
	Transport() :m_means(Vehicle::bus), m_startTime(0), m_endTime(0), m_startDay(0), m_endDay(0) {}

	//int m_id;				/*这个车次的ID,从0开始*/

	City m_srcCity;			
	City m_destCity;
	Vehicle::Type m_means;	/*交通方式*/
	int m_startTime;		/*出发时间*/
	int m_endTime;			/*到达时间*/
	int m_startDay;			/*出发日期*/
	int m_endDay;			/*到达日期*/

};

/*
	Discription:	表示一个车次在时刻表中的结点。
	Creation Date:	2020/5/26
*/
//struct TransNode
//{
//	TransNode(const Transport& transport)
//		:next(nullptr), trans(transport) {}
//	Transport trans;
//	TransNode* next;
//};

/*
	Discription:	信息系统,包含查表算法,时间等
	Creation Date:	2020/5/26
*/
class TransSystem
{
public:
	TransSystem();
	~TransSystem();

	
	int GetTime()const { return m_time; }
	int GetDay()const { return m_day; }
	const vector<City>& GetCityList()const { return m_cityList; }
	const vector<Transport> GetTransList(int srcIndex, int destIndex, Vehicle::Type means)const ;
	
	void SetTimeUp();

	static const int CountTime(int startTime, int endTime);
	static const int CountTime(int startTime, int endTime, int startDay, int endDay);

	/*比较时间日期大小,返回A>=B*/
	static const bool CompareDateTimeLE(int timeA, int timeB, int dayA, int dayB);
	/*比较时间日期大小,返回A>B*/
	static const bool CompareDateTimeL(int timeA, int timeB, int dayA, int dayB);

	/**************算法模块****************************************/
	const vector<Transport>FindPlanLimTime(
		int srcIndex, int destIndex,
		int startDay, int endDay,
		int startTime, int endTime);
	/**************算法模块****************************************/



private:
	vector<vector<vector<Vehicle::Type>>> m_timeTable;	/*三维数组,表示两个地点之间24小时的所有航班*/
	vector<vector<Vehicle::Type>> m_transMap;	/*交通方式表,表示两点之间是否有航班*/
	vector<vector<int>> m_distMap;				/*距离表,表示乘坐飞机的时间,乘坐火车,汽车时间分别为飞机*/
	vector<City> m_cityList;					/*城市与编号对应表*/
	int m_time;									/*当前时间*/
	int m_day;									/*当前日期*/
	///*定义各种交通方式的标志,用二进制的一个位表示*/
	//static const unsigned char HAS_PLANE = 4;
	//static const unsigned char HAS_TRAIN = 2;
	//static const unsigned char HAS_BUS = 1;


	/*全局变量,一天24小时以及城市数量*/
	static const int MAX_TIME = 24;
	static const int CITY_COUNT = 4;

	void GenRandTransports(Vehicle::Type means, int srcIndex, int destIndex);

	const Transport GenTransport(int startTime, Vehicle::Type means, int srcIndex, int destIndex, int startDay = 0) const;

	/*算法模块***a_开头*********************************************/
	vector<Transport> a_plan;		/*全局变量,当前遍历的plan数组*/
	vector<Transport> a_bestPlan;	/*最优解*/
	//int a_risk;					/*全局变量,当前plan累计的风险*/
	int a_bestRisk;					/*全局变量,当前最优的plan累计的风险,-1表示还没有最优解*/
	
	void a_InitAlgorithm() { a_plan = {}, a_bestPlan = {}, a_bestRisk = -1; }/*初始化算法的全局变量*/

	void a_DFS_limTime(
		int srcIndex, int destIndex,
		int startDay, int endDay,
		int startTime, int endTime,
		int riskBefore);		/*递归搜有限时间内最低风险*/

	bool a_DFS_limTimeEachVehicle( /*a_DFS_limTime的一个组件，用来区分所有交通方式*/
		int srcIndex, int destIndex,
		int startDay, int endDay,
		int startTime, int endTime,
		int riskBefore,
		int time, int cityi, int riskAfter, Vehicle::Type v);


	/*算法模块*****************a****************************/
};
