# COVID-19����ϵͳ
## �����
### User

* û�мƻ�����stop״̬,ϵͳ��������ÿ���Ϣ��
* �мƻ����Ǹ���m_plan,������Ϊon����stay״̬,ϵͳÿ��һ��ʱ�����һ���ÿ�״̬��λ�á�

### Transport
* beijing guangzhou �ɻ�
* beijing wuhan     �� ����
* �ǵ��ڶ�������transportSystem,��Ȼ��ը�ѵ�
* externȫ�ֱ��� const vector< VehAttr >vehAttrs,���������н�ͨ���ߵ����ԡ�
* Transport��һ�ν�ͨ,��һ���㵽һ�����һ�ν�ͨ��Ϣ��
* TransSystem������ϵͳ,�����˳��о����,���н�ͨʱ���,�涨�˳���������ʱ�䡣

### User
* ��ʼ����ʱ��,�趨״̬Ϊstay����ͣ�����޼ƻ���
* ���԰������֣�״̬�����У���ǰ�г��ڼƻ��е�λ�ã����мƻ���
* UpdateInfo(time) ���ݵ�ǰʱ�������Ϣ�������˿�״̬,��ǰ�г̣���ǰ���еȵȡ�
* Updateplan(...) ,�û�ѡ���ƻ�ʱ���øú�����

### Vehicle
* Vehicle::Type����ָ����ͨ��ʽ
* Vehicle::GetAttribute( VeHicle :: Type)���Ի�ȡ�ý�ͨ��ʽ������
* Type������������

### TransSystem
* �������ֱ��ʾ����֮��ľ������ͨ��ʽ����ͨʱ���
* 