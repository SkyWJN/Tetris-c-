# Tetris-c++-
纯c++的俄罗斯方块小游戏  
利用easyx图形库设计俄罗斯方块小游戏  
小键盘方向键控制，← →：控制方块左右移动；↑：旋转方块；↓：快速下降方块。 
***
消除行会得分，使用ifstream头文件读取/存储在txt文件内的分数。添加了背景音乐和消除音效。  
使用vector创建二维数组map[][]记录方块可以存在的区域、方块下落固定后的位置和类型。***//0表示没有方块，1...7表示方块的7中类型。***  
使用struct定义俄罗斯方块的四个小方块smallBlcok[4]来记录当前正在下落的俄罗斯方块的位置。  
利用map和smallBlock判断方块下落后所在的位置是否合法，实现方块下落后堆在其他方块上的效果。实现判断游戏结束的方法。   
通过改变smallBlock的数据来实现方块移动和旋转的效果。
***
![1](https://user-images.githubusercontent.com/66019283/190573999-d7052380-f86a-444c-bd0d-65b7b05daaa4.png)
![2](https://user-images.githubusercontent.com/66019283/190574014-31eeaba4-f790-4e92-99a2-f7a810333f6c.png)
![over](https://user-images.githubusercontent.com/66019283/190574018-d9fe3bc1-5526-420c-bdb8-e908659f300f.png)
