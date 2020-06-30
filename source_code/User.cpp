/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#include "User.h"



User::User(const string& name, const City& city)
	:m_name(name), m_city(city), m_status(status::stay), m_planIndex(0) {
	
}

User::~User() {

}

const string User::GetStatusName() const
{
	// TODO: 在此处插入 return 语句
	switch (m_status)
	{
	case User::status::on:
		return "on";
		break;
	case User::status::suspend:
		return "suspend";
		break;
	case User::status::stay:
		return "stay";
		break;
	default:
		break;
	}
	return "bug";
}

/*更新行程,更新完记得*/
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
		if (time == m_plan.at(m_planIndex).m_startTime) {
			m_status = status::on;
		}
		break;
	case status::on:
		/*TODO: 需要完善支持一次行程超过24小时的时间机制,所有的==判断都要改成更严谨的范围判断*/
		if (time == m_plan.at(m_planIndex).m_endTime) {
			
			/*更新所在城市,所在城市为触发的城市*/
			m_city = m_plan.at(m_planIndex).m_destCity;
			/*结束一段transport*/
			m_planIndex++;
			/*判断是否结束一段plan*/
			m_status = m_planIndex >= m_plan.size() ?
				status::stay : status::suspend;

			/*再更新一次,如果当前时刻有发车则立刻出发*/
			UpdateInfo(time);
		}
		break;
	default:
		break;
	}
}
