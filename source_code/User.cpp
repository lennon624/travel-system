/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
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
	// TODO: �ڴ˴����� return ���
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

/*�����г�,������ǵ�*/
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
		if (time == m_plan.at(m_planIndex).m_startTime) {
			m_status = status::on;
		}
		break;
	case status::on:
		/*TODO: ��Ҫ����֧��һ���г̳���24Сʱ��ʱ�����,���е�==�ж϶�Ҫ�ĳɸ��Ͻ��ķ�Χ�ж�*/
		if (time == m_plan.at(m_planIndex).m_endTime) {
			
			/*�������ڳ���,���ڳ���Ϊ�����ĳ���*/
			m_city = m_plan.at(m_planIndex).m_destCity;
			/*����һ��transport*/
			m_planIndex++;
			/*�ж��Ƿ����һ��plan*/
			m_status = m_planIndex >= m_plan.size() ?
				status::stay : status::suspend;

			/*�ٸ���һ��,�����ǰʱ���з��������̳���*/
			UpdateInfo(time);
		}
		break;
	default:
		break;
	}
}
