#include "Tetris.h"
#include "Block.h"
#include <time.h>
#include <stdlib.h>
#include<conio.h>
#include<fstream>
#include<iostream>

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"

//const int SPEED_NORMAL = 500; //ms
const int SPEED_NORMAL[MAX_LEVEL] = { 500,300,150,100,80 };
const int SPEED_QUICK = 50; //ms

Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blockSize;

	for (int i = 0; i < rows; i++)
	{
		vector<int> mapRow;
		for (int j = 0; j < cols; j++)
		{
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}

void Tetris::init()
{
	//背景音乐
	mciSendString("pkay res/bg3.mp3 repeat", 0, 0, 0);

	delay = SPEED_NORMAL[0];

	// 配置随机种子
	srand(time(NULL));

	// 创建游戏窗口
	initgraph(938, 896);

	// 加载背景图片
	loadimage(&imgBg, "res/bg2.png");
	
	loadimage(&imgWin, "res/win.png");
	loadimage(&imgOver, "res/over.png");

	// 初始化游戏区中的数据
	char data[20][10];
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j] = 0;
		}
	}
	score = 0;
	lineCount = 0;
	level = 1;
	
	// 初始化最高分
	ifstream file(RECORDER_FILE);
	if (!file.is_open())
	{
		std::cout << RECORDER_FILE << "faile to open!" << endl;
		highestscore = 0;
	}
	else 
	{
		file >> highestscore;
	}
	file.close(); // Close file

	gameOver = false;
}

void Tetris::play()
{
	init();

	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;


	int timer = 0;
	while (1) {
		// 接受用户的输入
		keyEven();

		timer += getDelay();
		if (timer > delay) 
		{
			timer = 0;
			drop();
			update = true;
		}

		if (update)
		{	
			update = false;
			// 渲染游戏画面
			updateWindow();

			// 更新游戏数据
			clearLine();
		}

		if (gameOver)
		{
			// 保存分数
			saveScore();

			// 更新游戏结束界面
			displayOver();

			system("pause");
			init();	//重新开始

		}

	}
}

void Tetris::keyEven()
{
	unsigned char ch; //0 - 255
	bool rotateFlag = false;
	int dx = 0;

	// 判断有无按键输入
	if (_kbhit()) {
		ch = _getch(); // 读取按键输入

		// 如果按下方向键，会自动返回两个字符
		// 如果按下 up 键，会先后依次返回：224 72
		// 如果按下 down 键，会先后依次返回：224 80
		// 如果按下 left 键，会先后依次返回：224 75
		// 如果按下 right 键，会先后依次返回：224 77
		if (ch == 224)
		{
			ch = _getch();
			switch (ch) {
			case 72:
				rotateFlag = true;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
		}
	}

	if (rotateFlag)
	{
		// to do
		rotate();
		update = true;
	}

	if (dx != 0)
	{
		// 实现左右移动
		moveLeftRight(dx);
		update = true;
	}
}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg); //绘制背景图片

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j] == 0) continue;

			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}

	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(689, 150);

	drawScore(); // 绘制分数;

	EndBatchDraw();
}

// 第一次调用，返回0
// 返回距离上一次调用该函数，间隔了多少时间（ms）
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currenTime = GetTickCount();

	if (lastTime ==0 )
	{
		lastTime = currenTime;
		return 0;
	}
	else
	{
		int ret = currenTime - lastTime;
		lastTime = currenTime;
		return ret;
	}
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blockInMap(map))
	{
		// 把方块固定
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;

		//检查游戏是否结束
		checkOver();
	}

	delay = SPEED_NORMAL[level - 1];
}

void Tetris::clearLine()
{
	int lines = 0;
	int k = rows - 1; //存储数据的行数
	for (int i = rows - 1; i >=0; i--)
	{
		// 检查第i行是否满行
		int count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j]; // 一遍扫描一遍存储
		}
		// 不是满行
		if (count < cols)
		{
			k--;
		}
		else //count == cols 满行	
		{
			lines++;
		}
	}

	if (lines > 0)
	{
		// to do.
		int addScore[4] = { 10,30,60,80 };
		score += addScore[lines - 1];

		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;

		//每100分一级别
		level = (score + 99) / 100;
		if (level > MAX_LEVEL)
		{
			gameOver = true;
		}

		lineCount += lines;
	}

}

void Tetris::moveLeftRight(int offset)
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);
	
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7) return;

	bakBlock = *curBlock;
	curBlock->rotate();

	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);

	setcolor(RGB(180, 180, 180)); // 字体颜色

	LOGFONT f;
	gettextstyle(&f); // 获取当前字体
	f.lfHeight = 60;	//字体宽
	f.lfWeight = 30;	//字体高
	f.lfQuality = ANTIALIASED_QUALITY; //字体抗锯齿
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);

	setbkmode(TRANSPARENT);	//字体背景透明效果;
	outtextxy(670, 727, scoreText);

	// 绘制消除行数
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);
	int xPos = 224 - f.lfWeight * strlen(scoreText);
	outtextxy(xPos, 817, scoreText);

	//绘制当前关卡数
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(224 - 30, 727, scoreText);

	//绘制最高分
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestscore);
	outtextxy(670, 817, scoreText);


}

void Tetris::checkOver()
{
	gameOver = (curBlock->blockInMap(map) == false);
}

void Tetris::saveScore()
{
	if (score > highestscore)
	{
		highestscore = score;
		
		ofstream file(RECORDER_FILE);
		file << highestscore;
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg.mp3", 0, 0, 0); //背景音乐停止

	//胜利or失败结束
	if (level <= MAX_LEVEL)
	{
		putimage(262, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else
	{
		putimage(262, 361, &imgWin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
