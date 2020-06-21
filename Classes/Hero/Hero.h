#ifndef __HERO_H__
#define __HERO_H__
#include "cocos2d.h"
#include <vector>
#include <list>
#include <string>

USING_NS_CC;

class HRocker;
class Monster;
class Bullet;//ǰ������
class Weapon;
class FightScene;
struct specialSprite;
class Hero :public Layer
{
public:
	//����ֵ
	int lifeNumberNow;
	int lifeNumberMax;
	//����ֵ
	int powerNumberNow;
	int powerNumberMax;
	//����ֵ
	int shieldNumberNow;
	int shieldNumberMax;
	//�����
	int goldCoin;
	//Ӣ������
	std::string name;
	//λ�ã�������˶�ͼ�����½ǵ����꣩
	Point position;
	//�ƶ��ٶ�
	float velocity;
	//�ж��Ƿ���ײ
	bool isCollision;
	//�ܷ����ƶ�
	bool canMove;
	//����ʵ��
	Sprite* sprite;
	//���ڵ�
	FightScene* scene;
	//�ж��Ƿ�����������
	bool isOtherFunction;
	//�������ܽṹ
	specialSprite* spec;
	//���˼���
	std::vector<int> _num;
	//����Ƶ��
	float k;
	//�������ܺ���
	void otherFunction(specialSprite* _spec);
	virtual bool init();
	//���麯��
	virtual void heroInit(Point position, FightScene* scene) = 0;
	//�ƶ��ϰ��ж�
	bool isCanMove(int dx,int dy);
	//ҡ�˿���
	//��������ң��
	void startKeyRocker();
	//��������ҡ��
	void startTouchRocker();
	//ֹͣ����ҡ��
	void stopKeyRocker();
	//ֹͣ����ҡ��
	void stopTouchRocker();
	//��ʾ������Ϣ
	void showInformation();
	//����������Ϣ
	void hideInformation();
	//��������泡����
	void moveWithScene();
	//ֹͣ�����볡���Ļ���
	void moveWithoutScene();
	//��ȡӢ�۾��ο���д��
	Rect boundingBox();
	//�Զ���Ķ�ʱ��
	void myUpdate(float dt);
	//��ͼ��
	//CCTMXTiledMap* map;
	/* �����ײ�ĵ�ͼ�� */
	//CCTMXLayer* meta;
	//CCTMXObjectGroup* meta;


	//�ӵ�����
	std::vector<Bullet*> bullet;
	//�ϰ�������
	std::vector<Node*> barrier;
	//��������
	std::vector<Weapon*> weapon;
	//��ǰ�����е�����
	Weapon* weaponNow;
	//���ӵ�е�������
	int weaponNumberMax;
	//Ŀǰӵ�е�������
	int weaponNumberNow;
	//��ӻ��л�����
	void changeWeapon(Weapon* _weapon);
	//��һ������Ӣ��
	void nextSceneHero(Hero* hero);
	//��һ������
	void nextScene(FightScene* scene);
	//��Ϯ������
	void beAttacked(int damage);
	
protected:
	//�ƶ���΢С��
	float dx, dy;
	//���ϴ��˶��ļ���
	float dx_, dy_;
	//1�����ң�-1������
	int faceDirection;
	//1�����ң�-1������0��վ��
	int isRun;
	//�ƶ��ķ���(������)
	double moveDirection;


	//���˵ķ�������еĻ��������ƣ�
	double enemyDirection;


	//�ж��Ƿ����ͷż���
	bool isAbility;
	//����ʱ��
	float abilityTime;
	//����ʱ����
	float abilityBetweenTime;
	//�ָ�һ�����ܵ�ʱ��
	float recoverTime;
	//��ʱ��ÿ���ε���ʱ����
	float betweenTime;
	//��������(��ʼ��ʱ��)
	float scale;
	//�Ƿ�չʾ
	bool isShow;
	//��¼����(���ܼ��)
	int abilityCount;
	//��¼����(����ʱ��)
	int abilityCount_;
	//��¼�������ָ���
	int recoverCount;
	//��¼����������ʹ�ã�
	int attackCount;
	//���ܼ��ʱ����ô���
	int abilityInterval;
	//���ܵ��ô���
	int loopTime;
	//ÿ����Ϯ�����õĴ���
	int attackTime;
	//�Ƿ���泡����
	bool withScene;
	//�Ƿ��ܹ���
	bool canAttack;
	


	//�ڲ�����
	//��������
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status) = 0;
	//�����ϳ�
	void setAction(int status);
	//����ͼ��Ч��
	virtual void ability() = 0;
	//ȡ�����ܹ���Ч��
	virtual void removeAbilityFunction() = 0;
	//��ʼ����Ϣ
	void initInformation();
	//�ı���ʾ��Ϣ
	void changeInformation();
	//�������Գ�ʼ��
	void commonDataInit();
	//�ƶ���ĳλ��
	void moveBy(double dx, double dy);
	//���ָܻ�����
	void recover();
	//����
	void dead();
	
	//��������
	void attack();
	//�ı仡���Ʒ���
	void changeDirection();
	//�������
	void addWeapon(Weapon* _weapon);
	//�л���ӵ�е�����
	void changeWeaponNow();
	//�������
	std::string randWeapon(int num);
	//��Ԫ��
	friend class HRocker;
	friend class Weapon;
	friend class FightScene;
	//���ҡ��
	HRocker *rocker;
	
};

#endif //__HERO_H__