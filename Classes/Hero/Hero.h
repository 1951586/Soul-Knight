#ifndef __HERO_H__
#define __HERO_H__
#include "cocos2d.h"

USING_NS_CC;

class HRocker;
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
	//λ��
	Point position;//������˶��������½ǵ�����
	//�ƶ��ٶ�
	double velocity;
	//�ܷ����ƶ�
	bool canMove;
	//����ʵ��
	Sprite* sprite;
	//���ڵ�
	Scene* scene;
	virtual bool init();

	virtual void heroInit(Point position, Scene* scene) = 0;
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
	//�Զ���Ķ�ʱ��
	void myUpdate(float dt);
	//��ͼ��
	//CCTMXTiledMap* map;
	/* �����ײ�ĵ�ͼ�� */
	//CCTMXLayer* meta;
	//CCTMXObjectGroup* meta;
	
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
	//��������(��ʼ��ʱ��)
	float scale;
	//�Ƿ�չʾ
	bool isShow;
	
	//�Ƿ���泡����
	bool withScene;
	//������¼
	int count;	
	//�ڲ�����
	//��������
	//status = 1:����վ����2�������ܶ���-1������վ����-2�������ܶ�
	virtual Animate* createAnimate(int status) = 0;
	//�����ϳ�
	void setAction(int status);
	//�ж��ܷ��ƶ�(�ϰ���)
	//����
	virtual void ability() = 0;
	//��ʼ����Ϣ
	void initInformation();
	//�ı���ʾ��Ϣ
	void changeInformation();
	//�������Գ�ʼ��
	void commonDataInit();
	//�ƶ���ĳλ��
	void moveBy(double dx, double dy);
	//��Ԫ��
	friend class HRocker;
	//���ҡ��
	HRocker *rocker;
};

#endif //__HERO_H__