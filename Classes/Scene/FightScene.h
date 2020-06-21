#ifndef __FIGHT_SCENE_H__
#define __FIGHT_SCENE_H__

#include "cocos2d.h"
#include <vector>//�о������������Ǻõ�
#include <list>


USING_NS_CC;
class Hero;
class Monster;
class Bullet;
class weapon;

//ս���������ݽṹ
struct Area
{
	//�������
	Rect rect;
	//�Ƿ�����
	bool haveCome;
	//���ܣ�-1����û��
	int around[4];//���ϡ��ҡ���
	//�����Ƿ���С��
	bool isFightArea;
	//�ڼ�����С�ֵ�����(��allMonster�й�)
	int monsterCount;
	//�����ѳ���С�ֲ���
	int monsterNum;
	//����С�ֲ��������ֵ
	int monsterNumMax;
	//С��ͼ����
	Point posLittleMap;
};
//���䡢ҩˮ�����ݽṹ
struct specialSprite
{
	Sprite* sprite;
	int type;//0:���䣻1:����ҩˮ��2:����ҩˮ��3:������4:���;5������
	std::string name;
};
//��������
class FightScene :	public cocos2d::Scene
{
public:
	//���ݣ�
	//Ӣ��
	Hero *hero;  
	//�˶�ͼ��
	Layer* dynamicLayer;
	//��ֹͼ��
	Layer* staticLayer;
	//��ͼ�߶�
	int height;
	//�ϰ�����Ϣ
	Point gid;
	//С��ͼ
	Sprite* littleMap;
	//��������(���е�һ��)
	std::list<Monster*> monster; 
	//Ӣ���ӵ�����
	std::list<Bullet*> heroBullet;
	//С���ӵ�����
	std::list<Bullet*> monsterBullet;
	//ս����������
	std::vector<Area> area;
	//���⾫������
	std::list<specialSprite*> special;
	//ս�������ı���λ������
	std::vector<Point> posBox;
	//�˶�ͼ����tmx�ĳ�ʼ���
	Point pos;
	//��һ��
	int num;


	//������
	//��ʼ��Scene��������
	bool init();
	//���ͼ��
	void addLayer();
	//���Ӣ�ۣ�����ʱ�ã�
	void addHero(Hero* _hero);
	//�Ӵ����飨�ӵ���Ӣ�ۣ�
	void heroAndBullet();
	//�Ӵ����飨�ӵ���С�֣�
	void monsterAndBullet();
	//�Ӵ����飨Ӣ����С�֣�
	void heroAndMonster();
	//�Ӵ����飨�ӵ����ϰ��
	void bulletAndMeta();
	//�Ӵ�����(������С��)
	void weaponAndMonster();
	//�Ӵ�����(�������ӵ�)
	void weaponAndBullet();
	//�Ӵ�����(Ӣ������������)
	void heroAndSpecial();
	//����������˶������ĵ��Ƿ����ϰ���
	bool isCollision(int _x,int _y);
	//�л�С��
	virtual void changeMonster(int count, int num) = 0;
	//���������Ƴ���ȥ��С��
	void removeMonster();
	//���ӱ���
	void addBox(Vec2 vec);
	//�������
	void addLoop(Vec2 vec);
	//���鲢���³��������Ķ�ʱ��
	void update(float dt);
	//�����������ݵĶ�ʱ��
	void updateArea(float dt);
protected:
	//��������
	float scale;
	//�������һ������
	void addMonster(std::list<Monster*> _monster);
	
	friend class Hero;
	friend class Monster;
};
#endif
