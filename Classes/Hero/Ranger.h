#ifndef __RANGER_H__
#define __RANGER_H__
#include "Hero/Hero.h"

class Ranger :public Hero
{
public:
	CREATE_FUNC(Ranger);

	static Ranger* createHeroSprite(Point position, FightScene* scene);

	virtual void heroInit(Point position, FightScene* scene);
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status);
	//����
	virtual void ability();
	//���ܹ���Ч��
	virtual void removeAbilityFunction();
private:
	//��������
	int rotateDistance;
	//������ת����
	Animate* createRotateAnimate();
	//�����˶�����
	MoveBy* abilityMoveBy();
	//��������
	void afterAbility();

};

#endif
