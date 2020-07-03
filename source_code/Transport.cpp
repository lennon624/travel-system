/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
 * Creation Date:   2020/5/26
 *******************************************/
#include "Transport.h"

 //
 // /*��ʼ���������н�ͨ���ߵ�����*/
 //const vector<VehAttr>vehAttrs(
 //	/*��enum��˳���Ӧ*/
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
	:m_time(0),/*ʱ��Ϊ0*/
	m_day(0),
	m_cityList(										/*�����б�*/
		{ {"Beijing",0.9},{"Guangzhou",0.7},{"Wuhan",0.9},
		{"ShenZhen",0.5}/*,{"Tianjin",0.5},{"Shanghai",0.5},
		{"Xinjiang",0.2},{"Xiamen",0.2},{"Sichuan",0.2},{ "Guilin",0.2 }*/ }),
	m_transMap(CITY_COUNT, vector<Vehicle::Type>(CITY_COUNT)),	/*��ͨ��ʽͼ*/
	m_distMap(CITY_COUNT, vector<int>(CITY_COUNT)),				/*����ͼ*/
	m_timeTable(CITY_COUNT, vector<vector<Vehicle::Type>>(CITY_COUNT, vector<Vehicle::Type>(MAX_TIME))),/*��ά��ʱ���*/
	a_plan({}),
	a_bestRisk(-1),
	a_bestPlan({})
{

	/*ȷ����ͨ��ʽͼ*/
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
	/*ֱ�Ӷ��뽻ͨ��ʽͼ ��������ͼ�ķ�ʽҲ��*/

	/*ȷ����ͨ����ͼ: */
	m_distMap = {
		{0,4,2,4},
		{4,0,2,1},
		{2,2,0,2},
		{4,1,2,0}
	};

	/*���ڿ�����ͨ����·,����ÿ����ͨ��ʽ����24Сʱ�ڵĳ���*/
	srand((int)time(0));	/*�������������*/
	for (int src = 0; src < m_transMap.size(); ++src) {
		for (int dest = 0; dest < m_transMap[0].size(); ++dest) {
			if (m_transMap[src][dest] & Vehicle::plane) {	/*�зɻ�*/
				GenRandTransports(Vehicle::plane, src, dest);
			}
			if (m_transMap[src][dest] & Vehicle::bus) {		/*������*/
				GenRandTransports(Vehicle::bus, src, dest);
			}
			if (m_transMap[src][dest] & Vehicle::train) {	/*�л�*/	
				GenRandTransports(Vehicle::train, src, dest);
			}
		}
	}

	/*�ؼ�����,������µ���ʱ���ҳ����·��,��Ϊ�û�����һ��plan��??��������Ŀ�Դ�*/
}
TransSystem::~TransSystem() {
	/*�ͷź����ڵ�*/
}

/*
	Discription:		����������ɴ�A-B����ĳһ���͵�ֱ�ﺽ��
	Params:
		int interval	�������
		int distTimes	�ý�ͨ���߻��ѵ�ʱ����distMap�б�ע�Ķ��ٱ�
		City src		Դ����
		City dest		Ŀ�ĳ���
*/
void TransSystem::GenRandTransports(Vehicle::Type means, int srcIndex, int destIndex) {
	srand((int)time(0));												/*�趨�������*/
	const Vehicle::Attribute attr = Vehicle::GetAttribute(means);		/*��ȡ�ý�ͨ���ߵ�����*/
	//int timeCost = m_distMap[srcIndex][destIndex] * attr.m_distTimes;	/*�����ù�����Ҫ��ʱ��*/
	//int startTime = rand() % attr.m_interval;							/*�漴�趨��һ����ʼʱ��*/
	for (int startTime = rand() % attr.m_interval; startTime < MAX_TIME; startTime += attr.m_interval) {
		/*���ð�μ���ʱ�����*/
		m_timeTable[srcIndex][destIndex][startTime] |= means;
	}
}

/*
	Discription:		���ݸ��������ͺͷ���ʱ������һ������
	Params:
		int startTime	����ʱ��
		Vehicle::Type means ��ͨ��ʽ
		City src		Դ����
		City dest		Ŀ�ĳ���
*/
const Transport TransSystem::GenTransport(
	int startTime, Vehicle::Type means, int srcIndex, int destIndex, int startDay) const
{
	/*��ȡ����*/
	Vehicle::Attribute attr = Vehicle::GetAttribute(means);
	/*���ɵ���ʱ��, TODO: ������Ҫ����Ƿ���쵽�����Ϣ*/
	int fullEndTime = (startTime + attr.m_distTimes * m_distMap[srcIndex][destIndex]);
	int endTime = fullEndTime % MAX_TIME;
	int endDay = fullEndTime / MAX_TIME + startDay;
	/*�������Transport*/
	return Transport(srcIndex, destIndex, means, startTime, endTime, startDay, endDay);
}

/*
	Discription::		DFS�ݹ麯��Ѱ������ʱ������ͷ���
*/
void TransSystem::a_DFS_limTime(
	int srcIndex, int destIndex, 
	int startDay, int endDay, 
	int startTime, int endTime,
	int riskBefore)
{
	/*
		ÿһ��������ʱ�򣬶���һ��transport����plan����,Ȼ��Ӹ�transport�ĵ���ʱ��͵������ڿ�ʼ�ݹ�
		�ݹ���ý���,����transport��plan��pop������
		* ���ܽ����a��a������,���Һܳ�ʱ��û�н�,���ڿͻ��˲�������㷨���ú��������� TODO
	*/
	if (destIndex == srcIndex)return;/*�Ƿ����,�Ҳ�����*/

	//int riskAfter = riskBefore;/*��������һ���г̵�risk,ridkAfter = riskBefore+time*riskRate/
	/*�����ӿ�ʼʱ�̿�ʼ��24Сʱ,tΪ���startTime�Ŀ�ʼʱ��*/
	for (int time = 0; time < MAX_TIME; time++)
	{

		/*��֦: ����ֵ����*/
		int riskAfter = riskBefore + time * m_cityList[srcIndex].m_risk;/*����ѡ��˿̳������ۻ�����*/
		if (a_bestRisk >= 0 && riskAfter >= a_bestRisk)return;								/*����˿̳��������Ѿ�����,�ǾͲ���������ȥ��*/

		for (int cityi = 0; cityi < CITY_COUNT; cityi++) {	/*����ÿ�����У�Ѱ�Ҵ˿̵��ھӽڵ�*/

			if (cityi != srcIndex) {						/*�ų���һ���ڵ��ǵ�ǰ���е����*/

				/*���ú���,���ȼ�˳��plane > train > bus*/
				/*����˿��д�src��cityi�ķɻ�,��÷ɻ�����ƻ��в��ݹ�*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::plane) {
					if (a_DFS_limTimeEachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, time, cityi, riskAfter, Vehicle::plane)) {
						return;
					}
				}
				/*����˿��д�src��cityi�Ļ�,��û𳵼���ƻ��в��ݹ�*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::train) {
					if (a_DFS_limTimeEachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, time, cityi, riskAfter, Vehicle::train)) {
						return;
					}
				}
				/*����˿��д�src��cityi������,�����������ƻ��в��ݹ�*/
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
	Discription: a_DFS_limTime��һ��С���
	return:	
		bool		Ϊtrue��ʱ��˵����������Ҫִ��return����
*/
bool TransSystem::a_DFS_limTimeEachVehicle(
	int srcIndex, int destIndex,
	int startDay, int endDay, 
	int startTime, int endTime,
	int riskBefore, 
	int time,int cityi,int riskAfter, Vehicle::Type v)/*����ĸ��������������еľֲ�����*/
{
	//TODO:	if have transport in this time
	/*����transport��Ϣ������transport*/
	int transStartDay = startDay;				/*��һ��transport��ʼ�����ǽ���*/
	int transStartTime = startTime + time;		/*��һ��transport����ʼʱ����startTime+time*/
	transStartDay += transStartTime / MAX_TIME;	/*��������ƫ��*/
	transStartTime = transStartTime % MAX_TIME;	/*ʱ��ȡģ*/
	Transport t = GenTransport(transStartTime, v, srcIndex, cityi, transStartDay);/*���ｻͨ��ʽ�Ǹ�����,Ҫ��*/
	a_plan.push_back(t);						/*�����ɵ�transport����plan��*/

	/*******************��֦*********************************************/

	/*��֦,��ǰ����ʱ�䳬��(>)��ddl,���ٿ������transport*/
	if (!CompareDateTimeL(t.m_endTime, endTime, t.m_endDay, endDay)) {

		/*��֦,����"���ѡ���transport�󼴿ɵ���Ŀ�ĵ�"�����*/
		if (cityi == destIndex) {		/*����Ŀ�ĳ���*/
			a_bestPlan = a_plan;		/*��������г�plan*/
			a_bestRisk = riskAfter;		/*������ѷ���ֵ*/
			a_plan.pop_back();			/*�����÷���*/
			return true;				/*�������±���(���ٿ����Ӻ�ʱ��),ֱ�ӷ���,*/
										/*��Ϊ������ʱ�䳤�̵�ʱ��,��������ȥ����Ҳ��һ����*/
										/*����·�ϵķ���ʱ,����Ҫ�����ӵķ���,����ֱ��return,��Ϊ����1Сʱ��ķɻ����ո���*/
		}

		/*���������,�������transportû�г�ʱ,��Ӹ�transport�����ĵص�ͳ��п�ʼ�������ұ���*/
		a_DFS_limTime(
			cityi, destIndex,
			t.m_endDay, endDay,
			t.m_endTime, endTime,
			riskAfter);
	}


	/*******************************************************/

	a_plan.pop_back();/*����*/
	return false;

}


/*
	Discription:		������������֮���24Сʱ�е�����transport
	Parems:	
		int srcIndex			��ʼ�������
		int destIndex			Ŀ�ĳ������
		Vehicle::Type vehicles	ѡ��Ľ�ͨ��ʽ
*/
const vector<Transport> TransSystem::GetTransList(int srcIndex, int destIndex, Vehicle::Type means)const
{	
	/*��ȡ��������֮���ʱ�̱�*/
	const vector<Vehicle::Type>& tTable = m_timeTable[srcIndex][destIndex];

	/*ͨ��ʱ�̱��������еĳ�����Ϣransport*/
	vector<Transport> tList;

	/*�Ȱ�������ȡ����������GetAttribute���õĴ���*/

	for (int startT = 0; startT < MAX_TIME; ++startT) {
		if ((Vehicle::bus & means) && (Vehicle::bus & tTable[startT])) {
			/*����������*/
			tList.push_back(GenTransport(startT, Vehicle::bus, srcIndex, destIndex));
		}
		if ((Vehicle::plane & means) && (Vehicle::plane & tTable[startT])) {
			/*����������*/
			tList.push_back(GenTransport(startT, Vehicle::plane, srcIndex, destIndex));
		}
		if ((Vehicle::train & means) && (Vehicle::train & tTable[startT])) {
			/*����������*/
			tList.push_back(GenTransport(startT, Vehicle::train, srcIndex, destIndex));
		}
	}
	return tList;
}

/* 
	description:	���ݸ����ĳ��к���ֹʱ���������������·��
	return:			
		const vector<Transport>TransSystem	���plan����
*/
const vector<Transport> TransSystem::FindPlanLimTime(
	int srcIndex, int destIndex, 
	int startDay, int endDay, 
	int startTime, int endTime)
{
	a_InitAlgorithm();/*��ʼ��ȫ�ֱ���*/
	if (CompareDateTimeL(endTime, startTime, endDay, startDay)) {/*ֻ���ں��������²ż���*/
		a_DFS_limTime(srcIndex, destIndex, startDay, endDay, startTime, endTime, 0);/*��ʼ����ֵ��Ϊ0*/
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
���ߺ�����������������ʱ��֮�����ʱ��
TODO: ���ڵ��㷨����������ʱ�̵���ʱ��<23Сʱ
*/
const int TransSystem::CountTime(int startTime, int endTime)
{
	return (endTime >= startTime) ? 
		(endTime - startTime) : (endTime + MAX_TIME - startTime);
}

/*
���ߺ�����������������ʱ��֮�����ʱ��,������Ҳ���뿼��֮��
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
