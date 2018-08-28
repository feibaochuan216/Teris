#include "config.h"
/** 配置参数：
 * 1, 游戏的全局常量集中存放在这里，如果想修改游戏的性能只需修改本文件即可。
 * 2, 尝试用C语言失败后，改尝试C++。先做最基本的功能。
 * @version 1.4C
 * @date 2018.06.05
 * @author 张良尧 */
using namespace std;

/*
 * ================ 逻辑规则 ================
 */

bool FIXED_SZ = true; // fixed size，形状是否是固定大小，即它所包含的格子数是固定的；如果为否，则格子数量在1～SHP_SZ之间（包含SHP_SZ）

/*
 * ================ 界面显示 ================
 */

int LTC_SZ = 10; // 格子的大小，占多少像素

int ROTAT_DIR = RD_CLCK; // rotation direction，旋转方向，默认为顺时针

int GP_WIDTH = 24; // Game Panel width，游戏面板宽度；必须 ≥ 2 且 ≥ SHP_SZ
int GP_HEIGHT = 32; // Game Panel height，游戏面板高度；必须 ≥ 2 且 > SHP_SZ

int SHP_SZ = 4; // Shape size，下落形状最多占几个格子；必须 ≥ 2 且 ≤ GP_WIDTH、< GP_HEIGHT

int OBS_MAX = 16; // 障碍物的最大高度，是GP_HEIGHT的一半

/*
 * ================ 其他 ===================
 */

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // 线程锁，由主线程销毁






