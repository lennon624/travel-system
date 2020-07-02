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

	static const Type all = bus | train | plane;
	
	struct Attribute {
		string  name;
		int m_interval;	//���κ���Ĺ̶����
		int m_distTimes;//�ٶ�,��Ҫ��ʱ����distMap[a][b]�Ķ��ٱ�
	};
	
	//��ȡ����,����Attribute
	static const Vehicle::Attribute GetAttribute(Type);

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

	//Ĭ�Ϲ���
	City() :m_name("Beijing"), m_risk(1.0) {}
};

/*
	Discription:	��ʾһ�����ࡢ����������
	Creation Date:	2020/5/26
*/
struct Transport
{
	/*TODO �Ѹ�����Ĺ��캯��������һ�°�*/
	Transport(const City& src, const City& dest, Vehicle::Type means,
		int startTime, int endTime, int startDay, int endDay)
		:m_srcCity(src), m_destCity(dest), m_means(means),
		m_startTime(startTime), m_endTime(endTime), m_startDay(startDay), m_endDay(endDay) {}
	
	//Ĭ�Ϲ���
	Transport() :m_means(Vehicle::bus), m_startTime(0), m_endTime(0), m_startDay(0), m_endDay(0) {}

	//int m_id;				/*������ε�ID,��0��ʼ*/

	City m_srcCity;			
	City m_destCity;
	Vehicle::Type m_means;	/*��ͨ��ʽ*/
	int m_startTime;		/*����ʱ��*/
	int m_endTime;			/*����ʱ��*/
	int m_startDay;			/*��������*/
	int m_endDay;			/*��������*/

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

	
	int GetTime()const { return m_time; }
	int GetDay()const { return m_day; }
	const vector<City>& GetCityList()const { return m_cityList; }
	const vector<Transport> GetTransList(int srcIndex, int destIndex, Vehicle::Type means)const ;
	
	void SetTimeUp();

	static const int CountTime(int startTime, int endTime);
	static const int CountTime(int startTime, int endTime, int startDay, int endDay);

	/*�Ƚ�ʱ�����ڴ�С,����A>=B*/
	static const bool CompareDateTimeLE(int timeA, int timeB, int dayA, int dayB);
	/*�Ƚ�ʱ�����ڴ�С,����A>B*/
	static const bool CompareDateTimeL(int timeA, int timeB, int dayA, int dayB);

	/**************�㷨ģ��****************************************/
	const vector<Transport>FindPlanLimTime(
		int srcIndex, int destIndex,
		int startDay, int endDay,
		int startTime, int endTime);
	/**************�㷨ģ��****************************************/



private:
	vector<vector<vector<Vehicle::Type>>> m_timeTable;	/*��ά����,��ʾ�����ص�֮��24Сʱ�����к���*/
	vector<vector<Vehicle::Type>> m_transMap;	/*��ͨ��ʽ��,��ʾ����֮���Ƿ��к���*/
	vector<vector<int>> m_distMap;				/*�����,��ʾ�����ɻ���ʱ��,������,����ʱ��ֱ�Ϊ�ɻ�*/
	vector<City> m_cityList;					/*�������Ŷ�Ӧ��*/
	int m_time;									/*��ǰʱ��*/
	int m_day;									/*��ǰ����*/
	///*������ֽ�ͨ��ʽ�ı�־,�ö����Ƶ�һ��λ��ʾ*/
	//static const unsigned char HAS_PLANE = 4;
	//static const unsigned char HAS_TRAIN = 2;
	//static const unsigned char HAS_BUS = 1;


	/*ȫ�ֱ���,һ��24Сʱ�Լ���������*/
	static const int MAX_TIME = 24;
	static const int CITY_COUNT = 4;

	void GenRandTransports(Vehicle::Type means, int srcIndex, int destIndex);

	const Transport GenTransport(int startTime, Vehicle::Type means, int srcIndex, int destIndex, int startDay = 0) const;

	/*�㷨ģ��***a_��ͷ*********************************************/
	vector<Transport> a_plan;		/*ȫ�ֱ���,��ǰ������plan����*/
	vector<Transport> a_bestPlan;	/*���Ž�*/
	//int a_risk;					/*ȫ�ֱ���,��ǰplan�ۼƵķ���*/
	int a_bestRisk;					/*ȫ�ֱ���,��ǰ���ŵ�plan�ۼƵķ���,-1��ʾ��û�����Ž�*/
	
	void a_InitAlgorithm() { a_plan = {}, a_bestPlan = {}, a_bestRisk = -1; }/*��ʼ���㷨��ȫ�ֱ���*/

	void a_DFS_limTime(
		int srcIndex, int destIndex,
		int startDay, int endDay,
		int startTime, int endTime,
		int riskBefore);		/*�ݹ�������ʱ������ͷ���*/

	bool a_DFS_limTimeEachVehicle( /*a_DFS_limTime��һ������������������н�ͨ��ʽ*/
		int srcIndex, int destIndex,
		int startDay, int endDay,
		int startTime, int endTime,
		int riskBefore,
		int time, int cityi, int riskAfter, Vehicle::Type v);


	/*�㷨ģ��*****************a****************************/
};
