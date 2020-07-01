# COVID-19旅行系统
## 类设计
### User

* 没有计划就是stop状态,系统不会更新旅客信息。
* 有计划就是更新m_plan,并调整为on或者stay状态,系统每隔一定时间更新一次旅客状态和位置。

### Transport
* beijing guangzhou 飞机
* beijing wuhan     火车 汽车
* 记得在堆区分配transportSystem,不然会炸裂的
* extern全局变量 const vector< VehAttr >vehAttrs,定义了所有交通工具的属性。
* Transport是一次交通,从一个点到一个点的一次交通信息。
* TransSystem是整个系统,包括了城市距离表,城市交通时间表,规定了城市数量和时间。

### User
* 初始化的时候,设定状态为stay，即停留且无计划。
* 属性包括名字，状态，城市，当前行程在计划中的位置，出行计划表。
* UpdateInfo(time) 根据当前时间更新信息，包括乘客状态,当前行程，当前城市等等。
* Updateplan(...) ,用户选定计划时调用该函数。

### Vehicle
* Vehicle::Type可以指定交通方式
* Vehicle::GetAttribute( VeHicle :: Type)可以获取该交通方式的特性
* Type可以作与运算

### TransSystem
* 三个表，分别表示两点之间的距离表，交通方式表，交通时间表。
* 设计得比较不好的地方是，City是一个struct，没有自己的静态方法，没有像vehicle那样舒服。懒得改了


## UI用户逻辑
### 3个page
0. TimeTable,每次选择不同的城市就会显示他们之间的所有车次。
1. Travel, 旅行页面，选择两个城市并生成一个plan给用户。
2. MAP, 地图页面，显示地图以及用户的位置状态。
3. 点击状态栏会弹出一个listWidget,显示当前的行程。


## 规范
* qt控件传参一律用指针，与库统一

## TODO
* Transport需要更新,需要添加一个信息：日期是否+1。
* 每个transport都有起始日期和到达日期的属性,但是在TRANSPORTS页面不显示,默认每天都一样,存储的时候按照day0存储,仅显示+1