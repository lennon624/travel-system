/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
 * Creation Date:   2020/5/26
 *******************************************/
#pragma once
#include<string>
#include<vector>
#include"Transport.h"
using namespace std;
#define DEFAULT_CITY 0

/*
	Discription:�ÿ�״̬����
	enum:
		on		���ڳ�����ͨ����
		suspend ����ĳ�����ж���ͣ���ȴ���һ������
		stay	����ĳ������ͣ����û�г��мƻ�
*/


class User
{
public:
	/*status����������,��ʾ�����û���status*/
	enum class status { on = 0, suspend = 1, stay = 2 };

	User(const string& name, const City& city);
	~User();

	const string& GetName() const { return m_name; }
	const string GetStatusName ()const;
	const User::status GetStatus()const { return m_status; }
	const City& GetCity() const { return m_city; }
	const Transport& GetTransport()const { return m_plan[m_planIndex]; }
	const bool HaveNewPlan()const { return m_newPlanFlag; }


	void SetName(const string& name) { m_name = name; }
	void SetCity(const City& city) { m_city = city; }
	void SetNewPlanFlag(bool flag) { m_newPlanFlag = flag; }

	void UpdatePlan(const vector<Transport>& plan);

	void UpdateInfo(int time, int day);
	

private:

	string m_name;				/*����*/
	status m_status;			/*��ǰ״̬*/
	City m_city;				/*��ǰ���ڳ���*/
	int m_planIndex;			/*��ǰ���е��г��ڼƻ��е�λ��*/
	vector<Transport> m_plan;	/*��ǰѡ��ĳ��мƻ�ֱ�Ӵ�transport,�����������*/
	bool m_newPlanFlag;			/*��ǰ�Ƿ��������µļƻ�*/
};

