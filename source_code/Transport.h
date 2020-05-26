/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
 * Creation Date:   2020/5/26
 *******************************************/
#pragma once
#include<vector>
#include<string>
using namespace std;

typedef enum { train, bus, plane } vehicle;

/*
	Discription:	��ʾһ�����ࡢ����������
	Creation Date:	2020/5/26
*/
struct Transport
{
	vehicle means;		/*��ͨ��ʽ*/
	int startTime;		/*����ʱ��*/
	int endTime;		/*����ʱ��*/
	City srcCity;
	City destCity;
	int id;				/*������ε�ID,��0��ʼ*/
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
	Discription:	������Ϣ
	Creation Date:	2020/5/26
*/
struct City {
	string name;	/*��������*/
	float risk;		/*���з���ָ��*/
	City(const string& cityName, float riskPoint)
		:name(cityName), risk(riskPoint) {}
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
	
private:
	vector<TransNode> m_table;	/*��������,a[i]Ϊ��iʱ�̳��������г���*/
	
};


