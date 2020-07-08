/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
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
	// TODO: �ڴ˴����� return ���
	switch (m_status)
	{
	case User::status::on:
		return u8"�ƶ���";
		break;
	case User::status::suspend:
		return u8"�ȴ���";
		break;
	case User::status::stay:
		return u8"ͣ����";
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
/*���ݵ�ǰ��ʱ�̸���user״̬,ע��!,ֻ������ȷ��ʱ�̸��£�һ�������ĳ��ʱ�̾ͻ���bug*/
void User::UpdateInfo(int time, int day) {

	switch (m_status)
	{
	case status::stay:
		/*�û�״̬Ϊstay��ʱ�����plan,�ȴ�updatePlan��״̬ת��Ϊsuspend*/
		break;

	case status::suspend:/*ʱ�䳬����һ��transport�Ŀ�ʼʱ���*/
		m_progress = 0; /*��������Ϊ0*/
		if (day > m_plan.at(m_planIndex).m_startDay 
			|| (day == m_plan.at(m_planIndex).m_startDay 
				&& time >= m_plan.at(m_planIndex).m_startTime)) {
			m_status = status::on;
		}
		break;

	case status::on:
		/*TODO: ��Ҫ����֧��һ���г̳���24Сʱ��ʱ�����,���е�==�ж϶�Ҫ�ĳɸ��Ͻ��ķ�Χ�ж�*/
		if (day > m_plan.at(m_planIndex).m_endDay
			|| (day == m_plan.at(m_planIndex).m_endDay
				&& time >= m_plan.at(m_planIndex).m_endTime)) {
			
			/*�������ڳ���,���ڳ���Ϊ�����ĳ���*/
			m_cityIndex = m_plan.at(m_planIndex).m_destIndex;
			/*����һ��transport*/
			m_planIndex++;

			/*�ж��Ƿ����һ��plan*/
			if (m_planIndex >= m_plan.size()) {
				m_planIndex = 0; /*�����Ϊ0*/
				m_status = status::stay;/*״̬��Ϊͣ��*/
				m_plan.clear();/*��ռƻ�*/
				m_progress = 0; /*��������Ϊ0*/
			} else { /*δ����,״̬�޸�Ϊ�ȴ���*/
				m_status = status::suspend;
				UpdateInfo(time, day);/*�ٸ���һ��,�����ǰʱ���з��������̳���*/
			}

		}
		break;
	default:
		break;
	}
}
