#pragma once
#include <vector>
#include<graphics.h>
#include"Block.h"
using namespace std;

class Tetris
{
public:
	//���캯��
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init(); //��ʼ��
	void play(); //��ʼ��Ϸ

private:
	int delay;
	bool update; // �Ƿ����

	// int map[20][10]
	// 0:�հף��޷���
	// 5:�ǵ����ֶ���˹����
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	IMAGE imgOver;
	IMAGE imgWin;

	Block* curBlock; //��ǰ����
	Block* nextBlock; //Ԥ�淽��
	Block bakBlock; //��ǰ���齵������У�����������һ���Ϸ�λ�õ�

	int score; // ��ǰ�ķ���;
	int highestscore; // ��߷�
	int level; //��ǰ�ؿ�
	int lineCount; //��ǰ��������
	bool gameOver; //��Ϸ�Ƿ��Ѿ�����


	void keyEven();
	void updateWindow();

	// ���ؾ�����һ�ε��øú���������˶���ʱ�䣨ms��
	// ��һ�ε��÷���0
	int getDelay(); 
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate(); //��ת
	void drawScore();	//���Ʒ���
	void checkOver();	//�����Ϸ�Ƿ����
	void saveScore(); //������߷�
	void displayOver(); //������Ϸ��������
	

};

