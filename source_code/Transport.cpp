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

const Vehicle::Attribute Vehicle::GetAttribute(Vehicle::Type type) {
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
	:m_time(0),/*时间为0*/
	m_day(0),
	m_cityList(										/*城市列表*/
		{ {"Beijing",0.9},{"Guangzhou",0.7},{"Wuhan",0.9},
		{"ShenZhen",0.5}/*,{"Tianjin",0.5},{"Shanghai",0.5},
		{"Xinjiang",0.2},{"Xiamen",0.2},{"Sichuan",0.2},{ "Guilin",0.2 }*/ }),
	m_transMap(CITY_COUNT, vector<Vehicle::Type>(CITY_COUNT)),	/*交通方式图*/
	m_distMap(CITY_COUNT, vector<int>(CITY_COUNT)),				/*距离图*/
	m_timeTable(CITY_COUNT, vector<vector<Vehicle::Type>>(CITY_COUNT, vector<Vehicle::Type>(MAX_TIME))),/*三维的时间表*/
	a_plan({}),
	a_bestRisk(-1),
	a_bestPlan({})
{

	/*确定交通方式图*/
	m_transMap[0][1] = Vehicle::plane | Vehicle::bus;	/*Beijing-Guangzhou*/
	m_transMap[1][0] = Vehicle::plane;
	m_transMap[0][2] = Vehicle::train | Vehicle::bus;	/*Beijing-Wuhan*/
	m_transMap[2][0] = Vehicle::bus | Vehicle::train;
	m_transMap[1][3] = Vehicle::bus | Vehicle::train | Vehicle::plane;	/*Guangzhou-Shenzhen*/
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
				GenRandTransports(Vehicle::plane, src, dest);
			}
			if (m_transMap[src][dest] & Vehicle::bus) {		/*有汽车*/
				GenRandTransports(Vehicle::bus, src, dest);
			}
			if (m_transMap[src][dest] & Vehicle::train) {	/*有火车*/	
				GenRandTransports(Vehicle::train, src, dest);
			}
		}
	}

	/*关键来了,如何在下单的时候找出最佳路径,并为用户构建一个plan呢??让我们拭目以待*/
}
TransSystem::~TransSystem() {
	/*释放航班表节点*/
}

/*
	Discription:		根据随机生成从A-B城市某一类型的直达航班
	Params:
		int interval	发车间隔
		int distTimes	该交通工具花费的时间是distMap中标注的多少倍
		City src		源城市
		City dest		目的城市
*/
void TransSystem::GenRandTransports(Vehicle::Type means, int srcIndex, int destIndex) {
	srand((int)time(0));												/*设定随机种子*/
	const Vehicle::Attribute attr = Vehicle::GetAttribute(means);		/*提取该交通工具的属性*/
	//int timeCost = m_distMap[srcIndex][destIndex] * attr.m_distTimes;	/*乘坐该工具需要的时间*/
	//int startTime = rand() % attr.m_interval;							/*随即设定第一个起始时间*/
	for (int startTime = rand() % attr.m_interval; startTime < MAX_TIME; startTime += attr.m_interval) {
		/*将该班次加入时间表中*/
		m_timeTable[srcIndex][destIndex][startTime] |= means;
	}
}

/*
	Discription:		根据给定的类型和发车时间生成一个车次
	Params:
		int startTime	发车时间
		Vehicle::Type means 交通方式
		City src		源城市
		City dest		目的城市
*/
const Transport TransSystem::GenTransport(
	int startTime, Vehicle::Type means, int srcIndex, int destIndex, int startDay) const
{
	/*获取属性*/
	Vehicle::Attribute attr = Vehicle::GetAttribute(means);
	/*生成到达时间, TODO: 这里需要添加是否隔天到达的信息*/
	int fullEndTime = (startTime + attr.m_distTimes * m_distMap[srcIndex][destIndex]);
	int endTime = fullEndTime % MAX_TIME;
	int endDay = fullEndTime / MAX_TIME + startDay;
	/*返回这个Transport*/
	return Transport(srcIndex, destIndex, means, startTime, endTime, startDay, endDay);
}

/*
	Discription::		DFS递归函数寻找有限时间内最低风险
*/
void TransSystem::a_DFS_limTime(
	int srcIndex, int destIndex, 
	int startDay, int endDay, 
	int startTime, int endTime,
	int riskBefore)
{
	/*
		每一轮搜索的时候，都将一个transport塞到plan后面,然后从该transport的到达时间和到达日期开始递归
		递归调用结束,将该transport从plan中pop出来。
		* 不能解决从a到a的问题,会找很长时间没有解,请在客户端层面或者算法调用函数层面解决 TODO
	*/
	if (destIndex == srcIndex)return;/*非法情况,找不到解*/

	//int riskAfter = riskBefore;/*计算完下一次行程的risk,ridkAfter = riskBefore+time*riskRate/
	/*遍历从开始时刻开始的24小时,t为相对startTime的开始时间*/
	for (int time = 0; time < MAX_TIME; time++)
	{

		/*剪枝: 风险值过大*/
		int riskAfter = riskBefore + time * m_cityList[srcIndex].m_risk;/*计算选择此刻出发的累积风险*/
		if (a_bestRisk >= 0 && riskAfter >= a_bestRisk)return;								/*如果此刻出发风险已经超了,那就不用再算下去了*/

		for (int cityi = 0; cityi < CITY_COUNT; cityi++) {	/*遍历每个城市，寻找此刻的邻居节点*/

			if (cityi != srcIndex) {						/*排除下一个节点是当前城市的情况*/

				/*调用函数,优先级顺序plane > train > bus*/
				/*如果此刻有从src到cityi的飞机,则该飞机加入计划中并递归*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::plane) {
					if (a_DFS_limTimeEachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, time, cityi, riskAfter, Vehicle::plane)) {
						return;
					}
				}
				/*如果此刻有从src到cityi的火车,则该火车加入计划中并递归*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::train) {
					if (a_DFS_limTimeEachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, time, cityi, riskAfter, Vehicle::train)) {
						return;
					}
				}
				/*如果此刻有从src到cityi的汽车,则该汽车加入计划中并递归*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::bus) {
					if (a_DFS_limTimeEachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, time, cityi, riskAfter, Vehicle::bus)) {
						return;
					}
				}
			}
		}
	}
}

/*
	Discription: a_DFS_limTime的一个小组件
	return:	
		bool		为true的时候说明主函数需要执行return操作
*/
bool TransSystem::a_DFS_limTimeEachVehicle(
	int srcIndex, int destIndex,
	int startDay, int endDay, 
	int startTime, int endTime,
	int riskBefore, 
	int time,int cityi,int riskAfter, Vehicle::Type v)/*最后四个参数是主函数中的局部变量*/
{
	//TODO:	if have transport in this time
	/*计算transport信息并生成transport*/
	int transStartDay = startDay;				/*下一个transport起始日期是今天*/
	int transStartTime = startTime + time;		/*下一个transport的起始时间是startTime+time*/
	transStartDay += transStartTime / MAX_TIME;	/*日期往后偏移*/
	transStartTime = transStartTime % MAX_TIME;	/*时间取模*/
	Transport t = GenTransport(transStartTime, v, srcIndex, cityi, transStartDay);/*这里交通方式是个变量,要改*/
	a_plan.push_back(t);						/*将生成的transport加入plan中*/

	/*******************剪枝*********************************************/

	/*剪枝,当前方案时间超出(>)了ddl,不再考虑这个transport*/
	if (!CompareDateTimeL(t.m_endTime, endTime, t.m_endDay, endDay)) {

		/*剪枝,处理"如果选择该transport后即可到达目的地"的情况*/
		if (cityi == destIndex) {		/*到达目的城市*/
			a_bestPlan = a_plan;		/*更新最佳行程plan*/
			a_bestRisk = riskAfter;		/*更新最佳风险值*/
			a_plan.pop_back();			/*弹出该方案*/
			return true;				/*不再往下遍历(不再考虑延后时间),直接返回,*/
										/*因为不考虑时间长短的时候,继续走下去风险也是一样的*/
										/*考虑路上的风险时,则需要更复杂的方法,不能直接return,因为可能1小时后的飞机风险更低*/
		}

		/*正常情况下,加入这个transport没有超时,则从该transport结束的地点和城市开始继续查找遍历*/
		a_DFS_limTime(
			cityi, destIndex,
			t.m_endDay, endDay,
			t.m_endTime, endTime,
			riskAfter);
	}


	/*******************************************************/

	a_plan.pop_back();/*弹出*/
	return false;

}


/*
	Discription:		返回两个城市之间的24小时中的所有transport
	Parems:	
		int srcIndex			起始城市序号
		int destIndex			目的城市序号
		Vehicle::Type vehicles	选择的交通方式
*/
const vector<Transport> TransSystem::GetTransList(int srcIndex, int destIndex, Vehicle::Type means)const
{	
	/*获取两个城市之间的时刻表*/
	const vector<Vehicle::Type>& tTable = m_timeTable[srcIndex][destIndex];

	/*通过时刻表生成所有的车次信息ransport*/
	vector<Transport> tList;

	/*先把属性提取出来，减少GetAttribute调用的次数*/

	for (int startT = 0; startT < MAX_TIME; ++startT) {
		if ((Vehicle::bus & means) && (Vehicle::bus & tTable[startT])) {
			/*添加这个车次*/
			tList.push_back(GenTransport(startT, Vehicle::bus, srcIndex, destIndex));
		}
		if ((Vehicle::plane & means) && (Vehicle::plane & tTable[startT])) {
			/*添加这个车次*/
			tList.push_back(GenTransport(startT, Vehicle::plane, srcIndex, destIndex));
		}
		if ((Vehicle::train & means) && (Vehicle::train & tTable[startT])) {
			/*添加这个车次*/
			tList.push_back(GenTransport(startT, Vehicle::train, srcIndex, destIndex));
		}
	}
	return tList;
}

/* 
	description:	根据给定的城市和起止时间日期来计算最佳路径
	return:			
		const vector<Transport>TransSystem	最佳plan数组
*/
const vector<Transport> TransSystem::FindPlanLimTime(
	int srcIndex, int destIndex, 
	int startDay, int endDay, 
	int startTime, int endTime)
{
	a_InitAlgorithm();/*初始化全局变量*/
	if (CompareDateTimeL(endTime, startTime, endDay, startDay)) {/*只有在合理的情况下才计算*/
		a_DFS_limTime(srcIndex, destIndex, startDay, endDay, startTime, endTime, 0);/*初始风险值设为0*/
	}
	return a_bestPlan;
}

void TransSystem::SetTimeUp()
{
	m_time = (m_time + 1) % MAX_TIME;
	if (!m_time) {
		++m_day;
	}
}
/*
工具函数，用来计算两个时刻之间的总时间
TODO: 现在的算法限制在两个时刻的总时间<23小时
*/
const int TransSystem::CountTime(int startTime, int endTime)
{
	return (endTime >= startTime) ? 
		(endTime - startTime) : (endTime + MAX_TIME - startTime);
}

/*
工具函数，用来计算两个时刻之间的总时间,将日期也计入考虑之中
*/
const int TransSystem::CountTime(int startTime, int endTime, int startDay, int endDay)
{
	return (endDay - startDay) * 24 + endTime - startTime;
}
const bool TransSystem::CompareDateTimeLE(int timeA, int timeB, int dayA, int dayB)
{
	return (dayA > dayB || (dayA == dayB && timeA >= timeB));
}
const bool TransSystem::CompareDateTimeL(int timeA, int timeB, int dayA, int dayB)
{
	return (dayA > dayB || (dayA == dayB && timeA > timeB));
}
