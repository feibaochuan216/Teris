#ifndef CONFIG_H
#define CONFIG_H
/** 配置参数：
 * 1, 游戏的全局常量集中存放在这里，如果想修改游戏的性能只需修改本文件即可。
 * 2, 尝试用C语言失败后，改尝试C++。先做最基本的功能。
 * @version 1.4C
 * @date 2018.06.05
 * @author 张良尧 */
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <QObject>

using namespace std;

/**
 * ================ 逻辑规则 ================
 */

const int SLOWEST = 2000; // 最慢下降速度，即第1关的初始速度，每下降一格需要的毫秒数，默认为2秒
const int FASTEST = 20; // 最快下隆速度，每20毫秒下隆一格
const double SPEED_RATE = 0.9; // speed time attenuation rate，每过一关的速度增长率（所花的毫秒数衰减）

/** 枚举在比较时报错 */
//enum RotateDir // Rotate Direction，旋转方向
//{
//	RD_CLCK, // clockwise，顺时针方向
//	RD_ANTI // anticlockwise，逆时针方向
//};
//const enum RotateDir ROTAT_DIR = RD_CLCK; // rotation direction，旋转方向，默认为顺时针

const int RD_CLCK = 0; // clockwise，顺时针方向
const int RD_ANTI = 1; // anticlockwise，逆时针方向

const int OBS_DENSITY = 70; // 障碍物密度，在障碍物的高度范围内用百分之多少的格子填满
const int OBS_INCREM = 3; // 随着游戏难度的增加，障碍物的初始形状高度的涨幅
const double OBS_RATE = 3; // 每过几关增涨1次障碍物

const qulonglong PASS_SCORE = 10; // 过第零关需要的分数，第1关的需要的分类是PASS_SCORE * PASS_RATE
const double PASS_RATE = 1.2; // 每过一关，过关需要的分数的增长率

const double AWARD_RATE = 2; // 每多消一行，得分乘以多少

#endif // CONFIG_H








