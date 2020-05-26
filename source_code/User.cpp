/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
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
			/*结束一段transport*/
			m_planIndex++;
			/*更新所在城市*/
			m_city = m_plan.at(m_planIndex)->destCity;
			/*判断是否结束一段plan*/
			m_status = m_planIndex >= m_plan.size() ?
				stay : suspend;
		}
		break;
	default:
		break;
	}
}
