/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#pragma once
#include<string>
#include<vector>
#include"Transport.h"
using namespace std;
#define DEFAULT_CITY 0

/*
	Discription:旅客状态类型
	enum:
		on		正在乘坐交通工具
		suspend 正在某个城市短暂停留等待下一个航班
		stay	正在某个城市停留，没有出行计划
*/


class User
{
public:
	/*status在类内声明,表示这是用户的status*/
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

	string m_name;				/*名字*/
	status m_status;			/*当前状态*/
	City m_city;				/*当前所在城市*/
	int m_planIndex;			/*当前进行的行程在计划中的位置*/
	vector<Transport> m_plan;	/*当前选择的出行计划直接存transport,避免回收问题*/
	bool m_newPlanFlag;			/*当前是否申请了新的计划*/
};

