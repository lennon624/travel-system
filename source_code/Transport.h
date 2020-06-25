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
//#define MAX_TIME	24
//#define CITY_COUNT	4

//
//enum class vehicle { bus = 0, train = 1, plane = 2 };
//
///*
//	Discription:	��ͨ��������
//	Creation Date:	2020/5/27
//*/
//struct VehAttr {
//	string m_name;
//	int m_interval;		/*fix interval*/
//	int m_distTimes;	/*�ٶ�,���distMap�е������ٱ�*/
//};
//
///*�������н�ͨ���ߵ�����*/
//extern const vector<VehAttr> vehAttrs;

class Vehicle
{
public:
	Vehicle();
	~Vehicle();

	typedef unsigned char Type;

	/*������ֽ�ͨ��ʽ�ı�־,�ö����Ƶ�һ��λ��ʾ*/
	static const Type bus = 0x01;
	static const Type train = 0x02;
	static const Type plane = 0x04;
	
	struct Attribute {
		string  name;
		int m_interval;	//���κ���Ĺ̶����
		int m_distTimes;//�ٶ�,��Ҫ��ʱ����distMap[a][b]�Ķ��ٱ�
	};
	
	//��ȡ����,����Attribute
	static Attribute GetAttribute(Type);

private:

};

/*
	Discription:	������Ϣ
	Creation Date:	2020/5/26
*/

struct City {
	string m_name;		/*��������*/
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
	Transport(const City& src, const City& dest, Vehicle::Type means, int startTime, int endTime)
		:m_srcCity(src), m_destCity(dest), m_means(means),
		m_startTime(startTime), m_endTime(endTime) {}

	//int m_id;				/*������ε�ID,��0��ʼ*/

	City m_srcCity;
	City m_destCity;
	Vehicle::Type m_means;		/*��ͨ��ʽ*/
	int m_startTime;		/*����ʱ��*/
	int m_endTime;			/*����ʱ��*/

};

/*
	Discription:	��ʾһ��������ʱ�̱��еĽ�㡣
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
	void GenRandTransport(Vehicle::Type means, int srcIndex, int destIndex);

private:
	vector<vector<vector<Vehicle::Type>>> m_timeTable;	/*��ά����,��ʾ�����ص�֮��24Сʱ�����к���*/
	vector<vector<Vehicle::Type>> m_transMap;	/*��ͨ��ʽ��,��ʾ����֮���Ƿ��к���*/
	vector<vector<int>> m_distMap;				/*�����,��ʾ�����ɻ���ʱ��,������,����ʱ��ֱ�Ϊ�ɻ�*/
	vector<City> m_cityList;					/*�������Ŷ�Ӧ��*/

	///*������ֽ�ͨ��ʽ�ı�־,�ö����Ƶ�һ��λ��ʾ*/
	//static const unsigned char HAS_PLANE = 4;
	//static const unsigned char HAS_TRAIN = 2;
	//static const unsigned char HAS_BUS = 1;

	/*ȫ�ֱ���,һ��24Сʱ�Լ���������*/
	static const int MAX_TIME = 24;
	static const int CITY_COUNT = 4;
};
