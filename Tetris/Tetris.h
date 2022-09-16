#pragma once
#include <vector>
#include<graphics.h>
#include"Block.h"
using namespace std;

class Tetris
{
public:
	//构造函数
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init(); //初始化
	void play(); //开始游戏

private:
	int delay;
	bool update; // 是否更新

	// int map[20][10]
	// 0:空白，无方块
	// 5:是第五种俄罗斯方块
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	IMAGE imgOver;
	IMAGE imgWin;

	Block* curBlock; //当前方块
	Block* nextBlock; //预告方块
	Block bakBlock; //当前方块降落过程中，用来备份上一个合法位置的

	int score; // 当前的分数;
	int highestscore; // 最高分
	int level; //当前关卡
	int lineCount; //当前已消行数
	bool gameOver; //游戏是否已经结束


	void keyEven();
	void updateWindow();

	// 返回距离上一次调用该函数，间隔了多少时间（ms）
	// 第一次调用返回0
	int getDelay(); 
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate(); //旋转
	void drawScore();	//绘制分数
	void checkOver();	//检查游戏是否结束
	void saveScore(); //保存最高分
	void displayOver(); //绘制游戏结束画面
	

};

