/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
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
	m_status = status::suspend;	/*将用户状态更新到出行中*/
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
			/*结束一段transport*/
			m_planIndex++;
			/*更新所在城市*/
			m_city = m_plan.at(m_planIndex).m_destCity;
			/*判断是否结束一段plan*/
			m_status = m_planIndex >= m_plan.size() ?
				status::stay : status::suspend;
		}
		break;
	default:
		break;
	}
}
