#pragma once
#ifndef __FIGHTSCENE4_H__
#define __FIGHTSCENE4_H__

#include "Scene/FightScene.h"

class FightScene4 :public FightScene
{
public:
	//��������
	static FightScene4* createFightScene(Hero* _hero);
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

	CREATE_FUNC(FightScene4);
};
#endif
