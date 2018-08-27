#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
using namespace std;

const int BYTE_SZ = 8; // 一个字节有几位

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

inline void indent(int level) {
	for(int i = 0; i < level; ++i) cout << "    ";
}

#endif // DEBUG

#endif // TOOLS_H








