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
	/*����ֵ���֣������������λ�������ʱ�䣬��λʱ�����ֵ*/
	switch (type)
	{
	case Vehicle::bus:
		//return { "BUS",2,4,2}; 
		return { u8"����",2,4,2}; 
		break;
	case Vehicle::train:
		//return { "TRAIN",4,2,5 };
		return { u8"��",4,2,5 };
		break;
	case Vehicle::plane:
		//return { "PLANE",6,1,9 };
		return { u8"�ɻ�",6,1,9 };
		break;
	default:
		break;
	}
	return { u8"��", 0, 0 };
}



TransSystem::TransSystem()
	:m_time(0),/*ʱ��Ϊ0*/
	m_day(0),
	m_cityList(													/*�����б�,����+����ֵ*/
		{ {u8"�ɶ�",0.9}, {u8"����",0.7}, {u8"̫ԭ",0.5},
		  {u8"����",0.9}, {u8"����",0.5}, {u8"��ɳ",0.7},
		  {u8"�人",0.5}, {u8"�Ϸ�",0.9}, {u8"����",0.7},
		  {u8"����",0.9}, {u8"����",0.5}, {u8"����",0.7}
		}),
	m_transMap(CITY_COUNT, vector<Vehicle::Type>(CITY_COUNT)),	/*��ͨ��ʽͼ*/
	m_distMap(CITY_COUNT, vector<int>(CITY_COUNT)),				/*����ͼ*/
	m_timeTable(CITY_COUNT, vector<vector<Vehicle::Type>>(CITY_COUNT, vector<Vehicle::Type>(MAX_TIME))),/*��ά��ʱ���*/
	a_plan({}),
	a_bestRisk(-1),
	a_bestPlan({}),
	//a_routeIndices({}),
	a_visitedFlags(vector<bool>(CITY_COUNT, false))
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
	params:
		��ʼ����,�յ����
		��ʼ����,����������(����ʱ��ʱ�����������Ч)
		�ۼƷ���
		������־λ:�Ƿ������ظ�����,�Ƿ���ʱ,�Ƿ����·�Ϸ���
*/
void TransSystem::a_DFS(
	int srcIndex, int destIndex, 
	int startDay, int endDay, 
	int startTime, int endTime,
	int riskBefore, 
	bool enableRepeat, bool enableLimTime,bool enableTransRisk)
{
	/*
		ÿһ��������ʱ�򣬶���һ��transport����plan����,Ȼ��Ӹ�transport�ĵ���ʱ��͵������ڿ�ʼ�ݹ�
		�ݹ���ý���,����transport��plan��pop������
		* ���ܽ����a��a������,���Һܳ�ʱ��û�н�,���ڿͻ��˲�������㷨���ú��������� 
		* �ڲ�����·�Ϸ��յ�ʱ�򣬵��ҵ�һ���ھ����յ��ʱ�򣬲��ٱ��������ھ�(�Ӻ�������true)����Ϊ��ʱ��ͣ��ʱ����̣�������С��
		* ����·�Ϸ��յ�ʱ��,�Ӻ�����Զ����false�����ˣ��Ӻ�����Ҫ����riskAfter+=���г̵�risk��
	*/
	if (destIndex == srcIndex)return;/*�Ƿ����,�Ҳ�����*/

	//int riskAfter = riskBefore;/*��������һ���г̵�risk,ridkAfter = riskBefore+time*riskRate/
	/*�����ӿ�ʼʱ�̿�ʼ��24Сʱ,tΪ���startTime�Ŀ�ʼʱ��*/
	for (int time = 0; time < MAX_TIME; time++)
	{

		/*��֦: ����ֵ����*/
		int riskAfter = riskBefore + time * m_cityList[srcIndex].m_risk;/*����ѡ��˿̳������ۻ�����*/
		if (a_bestRisk >= 0 && riskAfter >= a_bestRisk)return;			/*����˿̳��������Ѿ�����,�ǾͲ���������ȥ��*/

		for (int cityi = 0; cityi < CITY_COUNT; cityi++) {	/*����ÿ�����У�Ѱ�Ҵ˿̵��ھӽڵ�*/
			
			/*��֦, �û�Ҫ�������ظ�����һ������*/
			if (!enableRepeat && a_visitedFlags.at(cityi)) {
				continue;
			}
			
			if (cityi != srcIndex) {	/*�ų���һ���ڵ��ǵ�ǰ���е����*/

				/*���ú���,���ȼ�˳��plane > train > bus*/
				/*����˿��д�src��cityi�ķɻ�,��÷ɻ�����ƻ��в��ݹ�*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::plane) {
					if (a_DFS_EachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, 
						enableRepeat, enableLimTime,enableTransRisk,
						time, cityi, riskAfter, Vehicle::plane)) {
						return;
					}
				}
				/*����˿��д�src��cityi�Ļ�,��û𳵼���ƻ��в��ݹ�*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::train) {
					if (a_DFS_EachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, 
						enableRepeat, enableLimTime,enableTransRisk,
						time, cityi, riskAfter, Vehicle::train)) {
						return;
					}
				}
				/*����˿��д�src��cityi������,�����������ƻ��в��ݹ�*/
				if (m_timeTable[srcIndex][cityi].at((startTime + time)%MAX_TIME)& Vehicle::bus) {
					if (a_DFS_EachVehicle(
						srcIndex, destIndex,
						startDay, endDay,
						startTime, endTime,
						riskBefore, 
						enableRepeat, enableLimTime,enableTransRisk,
						time, cityi, riskAfter, Vehicle::bus)) {
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
bool TransSystem::a_DFS_EachVehicle(
	int srcIndex, int destIndex,
	int startDay, int endDay,
	int startTime, int endTime,
	int riskBefore,	/*�������û��*/
	bool enableRepeat, bool enableLimTime, bool enableTransRisk,
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
	//a_routeIndices.push_back(t.m_destIndex);	/*���¸����м���route��*/
	a_visitedFlags.at(t.m_destIndex) = true;	/*�����ѱ���*/


	/*******************��֦*********************************************/

	/*��֦,����û�Ҫ��**��ʱ**,���ҵ�ǰ����ʱ�䳬��(>)��ddl,���ٿ������transport*/
	/*��֦,����û�Ҫ��**����·�Ϸ���**,��ô����riskAfter�������ж�һ�η����Ƿ񳬳���ѷ���*/
	if (enableTransRisk) {
		riskAfter += CountTime(t.m_startTime, t.m_endTime, t.m_startDay, t.m_endDay)
			* Vehicle::GetAttribute(v).m_risk;/*·�Ϸ���=·��ʱ��*��ͨ���߷���ֵ**/
	}

	if (
		(!enableLimTime || !CompareDateTimeL(t.m_endTime, endTime, t.m_endDay, endDay)) /*ûҪ����ʱ��û����ddl*/
		&& 
		((a_bestRisk < 0) || (a_bestRisk >= 0 && riskAfter <= a_bestRisk))				/*�ۼƷ���û������ѷ���*/
		) {
		/*��֦��������Ŀ�ĵص����*/
		if (cityi == destIndex) {	/*����Ŀ�ĵ�*/
			a_bestPlan = a_plan;	/*��������г�plan*/
			a_bestRisk = riskAfter;	/*������ѷ���ֵ*/
			a_plan.pop_back();		/*�����÷���*/
			a_visitedFlags.at(t.m_destIndex) = false;
			/*�������±���(���ٿ����Ӻ�ʱ��),ֱ�ӷ���,*/
			/*��Ϊ������ʱ�䳤�̵�ʱ��,��������ȥ����Ҳ��һ����*/
			/*����·�ϵķ���ʱ,��return false,Ҳ�������±���,����������Ӧ�ÿ��������ھ�*/
			return !enableTransRisk;/*�������յ�true���return,���ڿ��������ھ�*/
		}

		/*���������,�������transportû�г�ʱ,��Ӹ�transport�����ĵص�ͳ��п�ʼ�������ұ���*/
		a_DFS(
			cityi, destIndex,
			t.m_endDay, endDay,
			t.m_endTime, endTime,
			riskAfter, enableRepeat, enableLimTime, enableTransRisk);
	}


	/*******************************************************/

	a_plan.pop_back();/*����*/
	a_visitedFlags.at(t.m_destIndex) = false;
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
const vector<Transport> TransSystem::FindPlan(
	int srcIndex, int destIndex, 
	int startDay, int endDay, 
	int startTime, int endTime,
	bool enableRepeat, bool enableLimTime, bool enableTransRisk)
{
	a_InitAlgorithm(srcIndex);/*��ʼ��ȫ�ֱ���*/
	a_DFS(srcIndex, destIndex, startDay, endDay, startTime, endTime, 0, enableRepeat, enableLimTime, enableTransRisk);/*��ʼ����ֵ��Ϊ0*/
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
