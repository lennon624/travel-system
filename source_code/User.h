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
	typedef enum { on, suspend, stay }status;
	static vector<string>status_name;

	User();
	~User();
	
	const string& getName() const { return m_name; }
	void setName(const string& name) { m_name = name; }
	void setCity(const City& city) { m_city = city; }
	
	/*�����µ��г�*/
	void UpdatePlan(const vector<Transport*>& plan);

	/*������Ϣ,ͨ��m_plan����״̬��λ��*/
	void UpdateInfo(int time);

private:

	string m_name;
	status m_status;
	City m_city;
	int m_planIndex;			/*��ǰ���е��г��ڼƻ��е�λ��*/
	vector<Transport*> m_plan;	/*��ǰѡ��ĳ��мƻ�,����ָ��,�������ͷ�,�����ͷŵ���ҪС��*/

};

