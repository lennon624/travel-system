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
	typedef enum { on, suspend, stay }status;
	static vector<string>status_name;

	User();
	~User();
	
	const string& getName() const { return m_name; }
	void setName(const string& name) { m_name = name; }
	void setCity(const City& city) { m_city = city; }
	
	/*更新新的行程*/
	void UpdatePlan(const vector<Transport*>& plan);

	/*更新信息,通过m_plan更新状态和位置*/
	void UpdateInfo(int time);

private:

	string m_name;
	status m_status;
	City m_city;
	int m_planIndex;			/*当前进行的行程在计划中的位置*/
	vector<Transport*> m_plan;	/*当前选择的出行计划,仅存指针,不负责释放,负责释放的人要小心*/

};

