/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#include "Transport.h"


 /*初始化定义所有交通工具的属性*/
static const vector<VehAttr>vehAttrs(
	/*与enum的顺序对应*/
	{ {"Bus",2,4},{"Train",4,2 },{"Plane",6,1} }
);

TransSystem::TransSystem()
	:m_timeTable(MAX_TIME, nullptr),					/*时刻表*/
	m_cityList(											/*城市列表*/
		{ {"Beijing",0.9},{"Guangzhou",0.9},{"Wuhan",0.9},
		{"ShenZhen",0.5}/*,{"Tianjin",0.5},{"Shanghai",0.5},
		{"Xinjiang",0.2},{"Xiamen",0.2},{"Sichuan",0.2},{ "Guilin",0.2 }*/ }),
	m_transMap(CITY_COUNT, vector<char>(CITY_COUNT)),	/*交通方式图*/
	m_distMap(CITY_COUNT, vector<int>(CITY_COUNT))		/*距离图*/
{
	/*确定交通方式图*/
	m_transMap[0][1] = HAS_PLANE;			/*Beijing-Guangzhou*/
	m_transMap[1][0] = HAS_PLANE;
	m_transMap[0][2] = HAS_TRAIN | HAS_BUS;	/*Beijing-Wuhan*/
	m_transMap[2][0] = HAS_BUS | HAS_TRAIN;
	m_transMap[1][3] = HAS_BUS | HAS_TRAIN;	/*Guangzhou-Shenzhen*/
	m_transMap[3][1] = HAS_TRAIN | HAS_BUS;
	m_transMap[3][2] = HAS_TRAIN;			/*Wuhan-Shenzhen*/
	m_transMap[2][3] = HAS_TRAIN;
	m_transMap[0][3] = HAS_BUS;				/*Shenzhen-Beijing*/
	m_transMap[3][0] = HAS_BUS;

	/*确定飞行距离图: */
	m_distMap = {
		{0,4,2,4},
		{4,0,2,1},
		{2,2,0,2},
		{4,1,2,0}
	};

	/*对于distMap中的每一项,随机生成固定间隔的航班车次插入到时间表中*/

	srand((int)time(0));	/*设置随机数种子*/
	for (int src = 0; src < m_transMap.size(); ++src) {
		for (int dest = 0; dest < m_transMap[0].size(); ++dest) {
			if (m_transMap[src][dest] & HAS_PLANE) {	/*有飞机*/
				GenRandTransport(plane, src, dest);
			}
			if (m_transMap[src][dest] & HAS_TRAIN) {	/*有火车*/
				GenRandTransport(train, src, dest);
			}
			if (m_transMap[src][dest] & HAS_BUS) {		/*有汽车*/
				GenRandTransport(bus, src, dest);
			}

		}
	}

	//初始化时钟
	//初始化旅客
	//随机生成测试样例

}
TransSystem::~TransSystem() {
	/*释放航班表节点*/
	for (TransNode* node : m_timeTable) {
		TransNode* curr = node, * next;
		while (curr) {
			TransNode* next = curr->next;
			delete curr;
			curr = next;
		}
	}
}


void TransSystem::GenRandTransport(vehicle means, int srcIndex, int destIndex) {
	srand((int)time(0));												/*设定随机种子*/
	const VehAttr& attr = vehAttrs[0];									/*提取该交通工具的属性*/
	int timeCost = m_distMap[srcIndex][destIndex] * attr.m_distTimes;	/*乘坐该工具需要的时间*/
	int startTime = rand() % attr.m_interval;							/*随即设定第一个起始时间*/
	while (startTime < MAX_TIME) {										/*当还没到程序结束(即航班停止)的时候*/

	}
}