#ifndef __RANGER_H__
#define __RANGER_H__
#include "Hero.h"
class Ranger :public Hero
{
public:
	CREATE_FUNC(Ranger);

	static Ranger* createHeroSprite(Point position, Scene* scene);

	virtual void heroInit(Point position, Scene* scene);
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status);
	//����
	virtual void ability();
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
