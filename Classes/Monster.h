#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "cocos2d.h"
#include "Hero/Hero.h"
#include "Scene/HomeScene.h"
#include <ctime>
USING_NS_CC;
//ǰ������
class Hero;
class HomeScene;

enum monster_Animation   //����ĸ��ֶ���״̬
{
	//status:
	RIGHT = 1,//���ң�����ͬ��
	RIGHT_STAND = 11,
	RIGHT_MOVE = 12,
	RIGHT_ATTCK = 13,
	RIGHT_DYING=14,
	RIGHT_DEAD=15,

	LEFT = 2,
	LEFT_STAND = 21,
	LEFT_MOVE = 22,
	LEFT_ATTCK = 23,
	LEFT_DYING=24,
	LEFT_DEAD=25,

	//ActionTag
	MOVE = 100,
	SETANIMATION = 99,
	SHAKE = 98,
	BLINK = 97,
	DYING=96,

};

class Monster 
	:public Sprite
{
protected:
	//����
	int HP;
	//�ٶ�(1����ÿ���ƶ�����50(���޸ģ���
	float speed;
	//������
	int ATK;
	////λ�� ����sprite���Դ��У���ȡλ�õ���xx.sprite����getPosition��
	//Point position;
	//�沿����1��ʾ���ң�2��ʾ����
	int direction;
	//�Ƿ������ƶ�
	bool isMove;
	//�Ƿ��ڹ���
	bool isAttck;
	//һ���ƶ��ľ���
	float moveDistance;
	//����ʵ��(���鸺�𶯻���this����λ�Ƽ�������)
	Sprite* sprite;
	//����
	HomeScene* scene;
	//��ָ�����໥����������
	Hero* hero;
	//ģ�ⶶ��
	void shake();

	//��ֱ�������ȥ
	void directMove();
	//����ƶ�
	void randomMove();
	//��õ�hero�ľ���
	float getDistance();
	//�ָ�Ĭ��״̬
	void monsterResume();
	//��ɱ
    void killed();


public:
	void setSpeed();
	void setAttack();
	

	//��ʼ��
	virtual bool init();
	virtual void monsterInit(Point position, HomeScene* scene) = 0;//������ʱ��HomeScene���ָ�룩
	//��������(����Ϊ��������������֡��
	virtual Animate* createAnimate(int status, int numFrame) = 0;

	//�ж��ܷ��ƶ�(�Ƿ��(x,y)���ϰ���)
	bool isCanMove(int x, int y);

	//�Դ��Ķ�ʱ��
	void myUpdate(float dt);

	//���ö�����ֻ���𶯻��������漰������
	void setAnimation(int status);

	//׷�����
	void chase(float dt);

	//��monsterʵ����hero
	void getHeroInstance(Hero* hero);

	//��������hurtΪ�˺���ֵ��Ѫ��Ϊ�����Զ�����������
	void beAttcked(int hurt);

	//��˸
	Sprite* blinkSprite;//����˸ʱ�õ�
	void blink();
	void blinkPart2(float dt);

	//������
	void dead();
	//void deadPart2(float dt);
	//�Ƿ��ܱ���ײ(�����󲻿���ײ��
	bool isCanTouched;
};

#endif // !__MONSTER_H__