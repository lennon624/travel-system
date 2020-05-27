# COVID-19旅行系统
## 类设计
### User

* 没有计划就是stop状态,系统不会更新旅客信息。
* 有计划就是更新m_plan,并调整为on或者stay状态,系统每隔一定时间更新一次旅客状态和位置。

### Transport
* beijing guangzhou 飞机
* beijing wuhan     火车 汽车
* 记得在堆区分配transportSystem,不然会炸裂的