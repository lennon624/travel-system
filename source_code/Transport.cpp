/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
 * Creation Date:   2020/5/26
 *******************************************/
#include "Transport.h"


 /*��ʼ���������н�ͨ���ߵ�����*/
static const vector<VehAttr>vehAttrs(
	/*��enum��˳���Ӧ*/
	{ {"Bus",2,4},{"Train",4,2 },{"Plane",6,1} }
);

TransSystem::TransSystem()
	:m_timeTable(MAX_TIME, nullptr),					/*ʱ�̱�*/
	m_cityList(											/*�����б�*/
		{ {"Beijing",0.9},{"Guangzhou",0.9},{"Wuhan",0.9},
		{"ShenZhen",0.5}/*,{"Tianjin",0.5},{"Shanghai",0.5},
		{"Xinjiang",0.2},{"Xiamen",0.2},{"Sichuan",0.2},{ "Guilin",0.2 }*/ }),
	m_transMap(CITY_COUNT, vector<char>(CITY_COUNT)),	/*��ͨ��ʽͼ*/
	m_distMap(CITY_COUNT, vector<int>(CITY_COUNT))		/*����ͼ*/
{
	/*ȷ����ͨ��ʽͼ*/
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

	/*ȷ�����о���ͼ: */
	m_distMap = {
		{0,4,2,4},
		{4,0,2,1},
		{2,2,0,2},
		{4,1,2,0}
	};

	/*����distMap�е�ÿһ��,������ɹ̶�����ĺ��೵�β��뵽ʱ�����*/

	srand((int)time(0));	/*�������������*/
	for (int src = 0; src < m_transMap.size(); ++src) {
		for (int dest = 0; dest < m_transMap[0].size(); ++dest) {
			if (m_transMap[src][dest] & HAS_PLANE) {	/*�зɻ�*/
				GenRandTransport(plane, src, dest);
			}
			if (m_transMap[src][dest] & HAS_TRAIN) {	/*�л�*/
				GenRandTransport(train, src, dest);
			}
			if (m_transMap[src][dest] & HAS_BUS) {		/*������*/
				GenRandTransport(bus, src, dest);
			}

		}
	}

	//��ʼ��ʱ��
	//��ʼ���ÿ�
	//������ɲ�������

}
TransSystem::~TransSystem() {
	/*�ͷź����ڵ�*/
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
	srand((int)time(0));												/*�趨�������*/
	const VehAttr& attr = vehAttrs[0];									/*��ȡ�ý�ͨ���ߵ�����*/
	int timeCost = m_distMap[srcIndex][destIndex] * attr.m_distTimes;	/*�����ù�����Ҫ��ʱ��*/
	int startTime = rand() % attr.m_interval;							/*�漴�趨��һ����ʼʱ��*/
	while (startTime < MAX_TIME) {										/*����û���������(������ֹͣ)��ʱ��*/

	}
}