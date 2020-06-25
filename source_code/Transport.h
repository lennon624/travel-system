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
	
	struct Attribute {
		string  name;
		int m_interval;	//两次航班的固定间隔
		int m_distTimes;//速度,需要的时间是distMap[a][b]的多少倍
	};
	
	//获取属性,返回Attribute
	static Attribute GetAttribute(Type);

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
};

/*
	Discription:	表示一个航班、汽车、车次
	Creation Date:	2020/5/26
*/
struct Transport
{
	/*TODO 把各个类的构造函数给完善一下吧*/
	Transport(const City& src, const City& dest, Vehicle::Type means, int startTime, int endTime)
		:m_srcCity(src), m_destCity(dest), m_means(means),
		m_startTime(startTime), m_endTime(endTime) {}

	//int m_id;				/*这个车次的ID,从0开始*/

	City m_srcCity;
	City m_destCity;
	Vehicle::Type m_means;		/*交通方式*/
	int m_startTime;		/*出发时间*/
	int m_endTime;			/*到达时间*/

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

	/*
		Discription:		根据随机生成从A-B城市某一类型的直达航班
		Params:
			int interval	发车间隔
			int distTimes	该交通工具花费的时间是distMap中标注的多少倍
			City src		源城市
			City dest		目的城市
	*/
	void GenRandTransport(Vehicle::Type means, int srcIndex, int destIndex);

private:
	vector<vector<vector<Vehicle::Type>>> m_timeTable;	/*三维数组,表示两个地点之间24小时的所有航班*/
	vector<vector<Vehicle::Type>> m_transMap;	/*交通方式表,表示两点之间是否有航班*/
	vector<vector<int>> m_distMap;				/*距离表,表示乘坐飞机的时间,乘坐火车,汽车时间分别为飞机*/
	vector<City> m_cityList;					/*城市与编号对应表*/

	///*定义各种交通方式的标志,用二进制的一个位表示*/
	//static const unsigned char HAS_PLANE = 4;
	//static const unsigned char HAS_TRAIN = 2;
	//static const unsigned char HAS_BUS = 1;

	/*全局变量,一天24小时以及城市数量*/
	static const int MAX_TIME = 24;
	static const int CITY_COUNT = 4;
};
