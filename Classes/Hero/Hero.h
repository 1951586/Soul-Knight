#ifndef __HERO1_H__
#define __HERO1_H__
#include "cocos2d.h"

USING_NS_CC;

class HRocker;
class Hero :public Layer//?
{
public:
	//����ֵ
	int lifeNumber;
	//����ֵ
	int powerNumber;
	//����ֵ
	int shieldNumber;
	//�����
	int goldCoin;
	//λ��
	Point position;
	//�ƶ��ٶ�
	double velocity;
	//����ʵ��
	Sprite* sprite;
	//���ڵ�
	Scene* scene;
	virtual bool init();

	virtual void heroInit(Point position, Scene* scene) = 0;
	//�ƶ���ĳλ��
	void moveTo(double dx, double dy);

	//��������
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status) = 0;
	//�����ϳ�
	void setAction(int status);
	//�ж��ܷ��ƶ�(�ϰ���)
	bool isCanMove(int dx,int dy);
	//����
	virtual void ability() = 0;
	//�Դ��Ķ�ʱ��
	void myUpdate(float dt);
	//ҡ�˿���
	//��������ң��
	void startKeyRocker();
	//��������ҡ��
	void startTouchRocker();
	//ֹͣ����ҡ��
	void stopKeyRocker();
	//ֹͣ����ҡ��
	void stopTouchRocker();
	
protected:
	//�ƶ���΢С��
	float dx, dy;
	//���ϴ��˶��ļ���
	float dx_, dy_;
	//1�����ң�-1������
	int faceDirection;
	//1�����ң�-1������0��վ��
	int isRun;
	//�ж��Ƿ����ͷż���
	bool isAbility;
	//����ʱ����
	int abilityInterval;
	//������¼
	int count;	
	//��Ԫ��
	friend class HRocker;
	//���ҡ��
	HRocker *rocker;
};

#endif //__HERO_H__