/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#pragma once
#include<vector>
#include<string>
using namespace std;

typedef enum { train, bus, plane } vehicle;

/*
	Discription:	表示一个航班、汽车、车次
	Creation Date:	2020/5/26
*/
struct Transport
{
	vehicle means;		/*交通方式*/
	int startTime;		/*出发时间*/
	int endTime;		/*到达时间*/
	City srcCity;
	City destCity;
	int id;				/*这个车次的ID,从0开始*/
};

/*
	Discription:	表示一个车次在时刻表中的结点。
	Creation Date:	2020/5/26
*/
struct TransNode
{
	TransNode(const Transport& transport)
		:next(nullptr), trans(transport) {}
	Transport trans;
	TransNode* next;
};

/*
	Discription:	城市信息
	Creation Date:	2020/5/26
*/
struct City {
	string name;	/*城市名称*/
	float risk;		/*城市风险指数*/
	City(const string& cityName, float riskPoint)
		:name(cityName), risk(riskPoint) {}
};




/*
	Discription:	信息系统,包含查表算法,时间等
	Creation Date:	2020/5/26
*/
class TransSystem
{
public:
	TransSystem();
	~TransSystem();
	
private:
	vector<TransNode> m_table;	/*链表数组,a[i]为第i时刻出发的所有车次*/
	
};


