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
	:m_cityList(										/*�����б�*/
		{ {"Beijing",0.9},{"Guangzhou",0.7},{"Wuhan",0.9},
		{"ShenZhen",0.5}/*,{"Tianjin",0.5},{"Shanghai",0.5},
		{"Xinjiang",0.2},{"Xiamen",0.2},{"Sichuan",0.2},{ "Guilin",0.2 }*/ }),
	m_transMap(CITY_COUNT, vector<Vehicle::Type>(CITY_COUNT)),	/*��ͨ��ʽͼ*/
	m_distMap(CITY_COUNT, vector<int>(CITY_COUNT)),				/*����ͼ*/
	m_timeTable(CITY_COUNT, vector<vector<Vehicle::Type>>(CITY_COUNT, vector<Vehicle::Type>(MAX_TIME)))/*��ά��ʱ���*/
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
	int startTime, Vehicle::Type means, int srcIndex, int destIndex) const
{
	/*��ȡ����*/
	Vehicle::Attribute attr = Vehicle::GetAttribute(means);
	/*���ɵ���ʱ��, TODO: ������Ҫ����Ƿ���쵽�����Ϣ*/
	int endTime = (startTime + attr.m_distTimes * m_distMap[srcIndex][destIndex]) % MAX_TIME;
	/*�������Transport*/
	return Transport(m_cityList[srcIndex], m_cityList[destIndex], means, startTime, endTime);
}

/*
	Discription:		������������֮���24Сʱ�е�����transport
	Parems:	
		int srcIndex			��ʼ�������
		int destIndex			Ŀ�ĳ������
		Vehicle::Type vehicles	ѡ��Ľ�ͨ��ʽ
*/
const vector<Transport> TransSystem::GetTransList(int srcIndex, int destIndex, Vehicle::Type means)
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

