/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#include "User.h"



User::User(const string& name, int cityIndex)
	:m_name(name), m_cityIndex(cityIndex),
	m_status(status::stay), m_planIndex(0),
	m_newPlanFlag(false), m_progress(0)
{
	
}

User::~User() {

}

const string User::GetStatusName() const
{
	// TODO: 在此处插入 return 语句
	switch (m_status)
	{
	case User::status::on:
		return u8"移动中";
		break;
	case User::status::suspend:
		return u8"等待中";
		break;
	case User::status::stay:
		return u8"停留中";
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
/*根据当前的时刻更新user状态,注意!,只能在正确的时刻更新，一旦错过了某个时刻就会有bug*/
void User::UpdateInfo(int time, int day) {

	switch (m_status)
	{
	case status::stay:
		/*用户状态为stay的时候清空plan,等待updatePlan将状态转换为suspend*/
		break;

	case status::suspend:/*时间超过下一个transport的开始时间后*/
		m_progress = 0; /*进度设置为0*/
		if (day > m_plan.at(m_planIndex).m_startDay 
			|| (day == m_plan.at(m_planIndex).m_startDay 
				&& time >= m_plan.at(m_planIndex).m_startTime)) {
			m_status = status::on;
		}
		break;

	case status::on:
		/*TODO: 需要完善支持一次行程超过24小时的时间机制,所有的==判断都要改成更严谨的范围判断*/
		if (day > m_plan.at(m_planIndex).m_endDay
			|| (day == m_plan.at(m_planIndex).m_endDay
				&& time >= m_plan.at(m_planIndex).m_endTime)) {
			
			/*更新所在城市,所在城市为触发的城市*/
			m_cityIndex = m_plan.at(m_planIndex).m_destIndex;
			/*结束一段transport*/
			m_planIndex++;

			/*判断是否结束一段plan*/
			if (m_planIndex >= m_plan.size()) {
				m_planIndex = 0; /*序号置为0*/
				m_status = status::stay;/*状态置为停留*/
				m_plan.clear();/*清空计划*/
				m_progress = 0; /*进度设置为0*/
			} else { /*未结束,状态修改为等待中*/
				m_status = status::suspend;
				UpdateInfo(time, day);/*再更新一次,如果当前时刻有发车则立刻出发*/
			}

		}
		break;
	default:
		break;
	}
}
