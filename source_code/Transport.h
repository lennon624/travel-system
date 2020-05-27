/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
 * Creation Date:   2020/5/26
 *******************************************/
#pragma once
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
using namespace std;
#define MAX_TIME	24
#define CITY_COUNT	4

typedef enum { bus, train, plane } vehicle;

/*
	Discription:	��ͨ��������
	Creation Date:	2020/5/27
*/
struct VehAttr {
	string m_name;
	int m_interval;		/*fix interval*/
	int m_distTimes;	/*�ٶ�,���distMap�е������ٱ�*/
};

/*�������н�ͨ���ߵ�����*/
static const vector<VehAttr> vehAttrs;

/*
	Discription:	������Ϣ
	Creation Date:	2020/5/26
*/

struct City {
	string m_name;	/*��������*/
	float m_risk;		/*���з���ָ��*/
	City(const string& cityName, float riskPoint)
		:m_name(cityName), m_risk(riskPoint) {}
};

/*
	Discription:	��ʾһ�����ࡢ����������
	Creation Date:	2020/5/26
*/
struct Transport
{
	/*TODO �Ѹ�����Ĺ��캯��������һ�°�*/
	Transport(const City& src, const City& dest, vehicle means, int startTime, int endTime)
		:m_srcCity(src), m_destCity(dest), m_means(means),
		m_startTime(startTime), m_endTime(endTime) {}

	//int m_id;				/*������ε�ID,��0��ʼ*/

	City m_srcCity;
	City m_destCity;
	vehicle m_means;		/*��ͨ��ʽ*/
	int m_startTime;		/*����ʱ��*/
	int m_endTime;			/*����ʱ��*/

};

/*
	Discription:	��ʾһ��������ʱ�̱��еĽ�㡣
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
	Discription:	��Ϣϵͳ,��������㷨,ʱ���
	Creation Date:	2020/5/26
*/
class TransSystem
{
public:
	TransSystem();
	~TransSystem();
	/*
		Discription:		����������ɴ�A-B����ĳһ���͵�ֱ�ﺽ��
		Params:
			int interval	�������
			int distTimes	�ý�ͨ���߻��ѵ�ʱ����distMap�б�ע�Ķ��ٱ�
			City src		Դ����
			City dest		Ŀ�ĳ���
	*/
	void GenRandTransport(vehicle means, int srcIndex, int destIndex);

private:
	vector<vector<vector<char>>> m_timeTable;	/*��ά����,��ʾ�����ص�֮��24Сʱ�����к���*/
	vector<vector<char>> m_transMap;			/*��ͨ��ʽ��,��ʵtimeTable������������չ,���ö�һ����Ϊ���˹�������һ��*/
	vector<vector<int>> m_distMap;				/*�����,��ʾ�����ɻ���ʱ��,������,����ʱ��ֱ�Ϊ�ɻ�*/
	vector<City> m_cityList;					/*�������Ŷ�Ӧ��*/

	static const unsigned char HAS_PLANE = 4;
	static const unsigned char HAS_TRAIN = 2;
	static const unsigned char HAS_BUS = 1;

};
