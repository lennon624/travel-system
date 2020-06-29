/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#include "Transport.h"

 //
 // /*初始化定义所有交通工具的属性*/
 //const vector<VehAttr>vehAttrs(
 //	/*与enum的顺序对应*/
 //	{ {"Bus",2,4},{"Train",4,2 },{"Plane",6,1} }
 //);


Vehicle::Vehicle()
{
}

Vehicle::~Vehicle()
{
}

Vehicle::Attribute Vehicle::GetAttribute(Vehicle::Type type) {
	switch (type)
	{
	case Vehicle::bus:
		return { "BUS",2,4 };
		break;
	case Vehicle::train:
		return { "TRAIN",4,2 };
		break;
	case Vehicle::plane:
		return { "PLANE",6,1 };
		break;
	default:
		break;
	}
	return { "NONE", 0, 0 };
}

TransSystem::TransSystem()
	:m_cityList(										/*城市列表*/
		{ {"Beijing",0.9},{"Guangzhou",0.7},{"Wuhan",0.9},
		{"ShenZhen",0.5}/*,{"Tianjin",0.5},{"Shanghai",0.5},
		{"Xinjiang",0.2},{"Xiamen",0.2},{"Sichuan",0.2},{ "Guilin",0.2 }*/ }),
	m_transMap(CITY_COUNT, vector<Vehicle::Type>(CITY_COUNT)),	/*交通方式图*/
	m_distMap(CITY_COUNT, vector<int>(CITY_COUNT)),				/*距离图*/
	m_timeTable(CITY_COUNT, vector<vector<Vehicle::Type>>(CITY_COUNT, vector<Vehicle::Type>(MAX_TIME)))/*三维的时间表*/
{

	/*确定交通方式图*/
	m_transMap[0][1] = Vehicle::plane;					/*Beijing-Guangzhou*/
	m_transMap[1][0] = Vehicle::plane;
	m_transMap[0][2] = Vehicle::train | Vehicle::bus;	/*Beijing-Wuhan*/
	m_transMap[2][0] = Vehicle::bus | Vehicle::train;
	m_transMap[1][3] = Vehicle::bus | Vehicle::train;	/*Guangzhou-Shenzhen*/
	m_transMap[3][1] = Vehicle::train | Vehicle::bus;
	m_transMap[3][2] = Vehicle::train;					/*Wuhan-Shenzhen*/
	m_transMap[2][3] = Vehicle::train;
	m_transMap[0][3] = Vehicle::train;					/*Shenzhen-Beijing*/
	m_transMap[3][0] = Vehicle::train;
	/*直接读入交通方式图 用类似下图的方式也行*/

	/*确定交通距离图: */
	m_distMap = {
		{0,4,2,4},
		{4,0,2,1},
		{2,2,0,2},
		{4,1,2,0}
	};

	/*对于可以连通的线路,对于每个交通方式生成24小时内的车次*/
	srand((int)time(0));	/*设置随机数种子*/
	for (int src = 0; src < m_transMap.size(); ++src) {
		for (int dest = 0; dest < m_transMap[0].size(); ++dest) {
			if (m_transMap[src][dest] & Vehicle::plane) {	/*有飞机*/
				GenRandTransport(Vehicle::plane, src, dest);
			}
			if (m_transMap[src][dest] & Vehicle::bus) {		/*有汽车*/
				GenRandTransport(Vehicle::bus, src, dest);
			}
			if (m_transMap[src][dest] & Vehicle::train) {	/*有火车*/	
				GenRandTransport(Vehicle::train, src, dest);
			}
		}
	}

	/*关键来了,如何在下单的时候找出最佳路径,并为用户构建一个plan呢??让我们拭目以待*/
}
TransSystem::~TransSystem() {
	/*释放航班表节点*/
}


void TransSystem::GenRandTransport(Vehicle::Type means, int srcIndex, int destIndex) {
	srand((int)time(0));												/*设定随机种子*/
	const Vehicle::Attribute attr = Vehicle::GetAttribute(means);		/*提取该交通工具的属性*/
	int timeCost = m_distMap[srcIndex][destIndex] * attr.m_distTimes;	/*乘坐该工具需要的时间*/
	//int startTime = rand() % attr.m_interval;							/*随即设定第一个起始时间*/
	for (int startTime = rand() % attr.m_interval; startTime < MAX_TIME; startTime += timeCost) {
		/*将该班次加入时间表中*/
		m_timeTable[srcIndex][destIndex][startTime] |= means;
	}
}

