#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "cocos2d.h"
#include <ctime>
USING_NS_CC;
//ǰ������
class Hero;
class FightScene;
class MonsWeapon;

enum monster_Animation   //����ĸ��ֶ���״̬
{
	//status:
	RIGHT = 1,//���ң�����ͬ��
	RIGHT_STAND = 11,
	RIGHT_MOVE = 12,
	RIGHT_ATTACK = 13,
	RIGHT_ATTACK_PART2=16,
	RIGHT_DYING=14,
	RIGHT_DEAD=15,

	LEFT = 2,
	LEFT_STAND = 21,
	LEFT_MOVE = 22,
	LEFT_ATTACK = 23,
	LEFT_ATTACK_PART2 = 26,
	LEFT_DYING=24,
	LEFT_DEAD=25,

	//ActionTag
	MOVE = 100,
	SETANIMATION = 99,
	SHAKE = 98,
	BLINK = 97,
	DYING=96,
	ATTACK=95,
	ATTACK_PART2=951,

};

class Monster 
	:public Sprite
{
protected:
	//����
	int HP;
	//�ٶ�
	float speed;
	//������
	int ATK;
	////λ�� ����sprite���Դ��У���ȡλ�õ���xx.sprite����getPosition��
	//Point position;
	//�沿����1��ʾ���ң�2��ʾ����
	int direction;
	//�ƶ������ã�x��y����������ʾ��
	Vec2 moveDirection;
	//�Ƿ������ƶ�
	bool isMove;
	//�Ƿ��ڹ���
	bool isAttck;
	//һ���ƶ��ľ���
	float moveDistance;
	//����ʱǰ���ľ���
	float attackDis;
	float attackSpeed;
	//����ʵ��(���鸺�𶯻���this����λ�Ƽ�������)
	Sprite* sprite;
	//����
	FightScene* scene;
	//��ָ�����໥����������
	Hero* hero;
	//ģ�ⶶ��
	void shake();
	//��ֱ�������ȥ
	void directMove();
	//����ƶ�
	void randomMove();
	//��õ�hero�ľ���
	float getHeroDistance();
	//��ɱ
	virtual void killed();
	//�������
	float attackTime;
	//Э����������
	Sprite* actionAsit;


public:

	//״̬
	//�Ƿ���ţ�1���ţ�0����
	bool isAlive;
	//�Ƿ�ǳ�
	bool isOnStage;
 	//�Ƿ��ܱ���ײ(�����󲻿���ײ��
	bool isCanTouched;
	//��ʼ��
	virtual bool init();
	virtual void monsterInit(Point position, FightScene* scene) = 0;//������ʱ��HomeScene���ָ�룩
	//��������(����Ϊ��������������֡��
	virtual Animate* createAnimate(int status, int numFrame) = 0;
	//�ж��ܷ��ƶ�
	bool isCanMove();
	//���ö�����ֻ���𶯻��������漰������
	virtual void setAnimation(int status)=0;
	//׷�����
	virtual void chase(float dt);
	//��monsterʵ����hero
	void getHeroInstance(Hero* hero);
	//��������hurtΪ�˺���ֵ��Ѫ��Ϊ�����Զ�����������
	void beAttacked(int hurt);
	//��˸
	Sprite* blinkSprite;//����˸ʱ�õ�
	void blink();
	void blinkPart2(float dt);
	//������
	void dead();
	//void deadPart2(float dt);
	//�ǳ�
	virtual void comeOnStage();
	//���monster���м��λ��
	Point position();
	//�ָ�Ĭ��״̬������
	void monsterResume();
	//���˺��ж���״̬
	void hurting(float dt);
	//����
	virtual void attack(float dt) = 0;
	////����buff
	//void getBuff(int type);
	//���ؾ���
	Sprite* getSprite();
	//���ط���
	int getDirection();
	Vec2 getMoveDirection();
	//��дboundingBox()
	Rect boundingBox();
	//���蹥�����
	void resetAttackTime(float dt);
	//�ƶ���ͣ��
	void moveOrStop(float dt);
	MonsWeapon* weapon;
	
};

#endif // !__MONSTER_H__