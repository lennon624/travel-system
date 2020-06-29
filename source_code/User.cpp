/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
 * Creation Date:   2020/5/26
 *******************************************/
#include "User.h"

vector<string> User::status_name = { "on","suspend","stay" };


User::User(const string& name, const City& city)
	:m_name(name), m_city(city), m_status(status::stay), m_planIndex(0) {
	
}

User::~User() {

}

void User::UpdatePlan(const vector<Transport>& plan) {
	m_status = status::suspend;	/*���û�״̬���µ�������*/
	m_planIndex = 0;
	m_plan = plan;
}

void User::UpdateInfo(int time) {

	switch (m_status)
	{
	case status::stay:
		break;
	case status::suspend:	
		if (time >= m_plan.at(m_planIndex).m_startTime) {
			m_status = status::on;
		}
		break;
	case status::on:
		if (time >= m_plan.at(m_planIndex).m_endTime) {
			/*����һ��transport*/
			m_planIndex++;
			/*�������ڳ���*/
			m_city = m_plan.at(m_planIndex).m_destCity;
			/*�ж��Ƿ����һ��plan*/
			m_status = m_planIndex >= m_plan.size() ?
				status::stay : status::suspend;
		}
		break;
	default:
		break;
	}
}
