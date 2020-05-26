/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
 * Creation Date:   2020/5/26
 *******************************************/
#include "User.h"

vector<string> User::status_name = { "on","suspend","stay" };


User::User() {

}

User::~User() {

}

void User::UpdatePlan(const vector<Transport*>& plan) {
	m_status = suspend;
	m_planIndex = 0;
	m_plan = plan;
}

void User::UpdateInfo(int time) {

	switch (m_status)
	{
	case stay:
		break;
	case suspend:
		if (time >= m_plan.at(m_planIndex)->startTime) {
			m_status = on;
		}
		break;
	case on:
		if (time >= m_plan.at(m_planIndex)->endTime) {
			/*����һ��transport*/
			m_planIndex++;
			/*�������ڳ���*/
			m_city = m_plan.at(m_planIndex)->destCity;
			/*�ж��Ƿ����һ��plan*/
			m_status = m_planIndex >= m_plan.size() ?
				stay : suspend;
		}
		break;
	default:
		break;
	}
}
