#ifndef __FIGHTSCENE3_H__
#define __FIGHTSCENE3_H__

#include "Scene/FightScene.h"

class FightScene3 :public FightScene
{
public:
	//��������
	static FightScene* createFightScene(Hero* _hero);
	//��ʼ������
	virtual bool initScene(Hero* _hero);

	//�л�С��
	virtual void changeMonster(int count, int num);
	//�������������浱ǰ�������й��
	std::list<Monster*> allMonster[3][2];//�����������������������Ҫ���ÿ����������
private:
	//��ʼ���ó����Ĺ�����Ϣ
	void initMonsterInformation();
	//��ʼ��������Ϣ
	void initAreaInformation();
	//��ʼ������λ����Ϣ
	void initBoxPosition();

	CREATE_FUNC(FightScene3);
};
#endif