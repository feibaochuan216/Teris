#ifndef MASK_H
#define MASK_H

/** 布尔值集合：
 * 1, 用无符号字符型数据作为布尔值的集合，每一位（注意不是字节）都是一个布尔值，1代表真，零代表假。 
 * @version 1.6D
 * @date 2018.07.27
 * @author 张良尧 */

#include <iostream>
using namespace std;

/**
 * ================ 别名 ================
 */

typedef unsigned char uchar; 

class Mask {
public:
	/**
	 * ================ 构造、析构 ================
	 */
	
	/** 创建一个8个布尔值的集合 */
	Mask();

	/** 创建一个指定个数的布尔值集合 */
	Mask(const int sz);
	
	/** 拷贝构造：
	 * 深拷贝，内部数据是完全不同的两个对象。 */
	Mask(const Mask & that);
	
	~Mask();
	
	/**
	 * ================ 赋值拷贝 ================
	 * 深拷贝，规则同拷贝构造。
	 */
	
	Mask & operator =(const Mask & that);
	
	/**
	 * ================ 判断 ================
	 */
	
	/** 相同数量的布尔值是否全一样 */
	friend bool operator ==(const Mask & m1, const Mask & m2);
	
	inline bool operator !=(const Mask & that) const {
		return !(* this == that);
	}
	
	/**
	 * ================ 查 ================
	 */
	
	/** 返回指定位置的布尔值：
	 * @param index：必须在0～sz - 1之间，没有越界检查。 */
	inline bool operator [](const int index) const {
		return mkMask(indexOfBit(index)) & m_data[indexOfByte(index)];
	}
	
	/**
	 * ================ 增 ================
	 */
	
	/** 增加指定个数的布尔值：
	 * @param ds：deta size，如果为零则什么都不做，如果为负则减少。 */
	Mask & addBit(const int ds);
	
	/**
	 * ================ 删 ================
	 */
	
	/** 移除指定个数的布尔值：
	 * @param ds：deta size，如果为零则什么都不做，如果为负则减少。 */
	Mask & rmBit(const int ds);
	
	/**
	 * ================ 改 ================
	 */
	
	/** 修改指定位置的布尔值：
	 * @param index：必须在0～sz - 1之间，没有越界检查。 */
	Mask & setVal(const int index, const bool v);
	
	/** 全部填充为指定值 */
	Mask & fill(const bool v);
	
	/**
	 * ================ 输入输出重载 ================
	 */
	
	/** 生成由'0'和'1'组成的字符串：
	 * 每4个'0'或'1'插入一个空格。 */
	friend ostream & operator <<(ostream & os, const Mask & This);
	
	/** 只接收由'0'和'1'组成的字符串：
	 * 其它字符被认为是分隔符。 */
	friend istream & operator >>(istream & is, Mask & This);
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline int size() const { return m_sz; }
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	uchar * m_data; // 无符号字符数组，每一位（注意不是每个字节）都是一个布尔值
	int m_sz; // 布尔值的数量
	const static int BASE_SZ = 8; // 一个字节里有几位
	
	/**
	 * ~~~~~~~~~~~~ 判断 ~~~~~~~~~~~~
	 */
	
	/** 是否需要重新分配内存：
	 * 如果给本集合设置一个新的尺寸，是否需要修改数组长度。
	 * @param size：新的尺寸，必须 > 0。 */
	inline bool needRecap(const int size) {
		return (m_sz - 1) / BASE_SZ != (size - 1) / BASE_SZ;
	}
	
	/**
	 * ~~~~~~~~~~~~ 查 ~~~~~~~~~~~~
	 */
	
	/** 将布尔值的下标转换成数据数组的下标：
	 * @param index：必须 ≥ 0 且 < sz 。 */
	inline static int indexOfByte(const int index) {
		return index / BASE_SZ;
	}
	
	/** 布尔值下标指向的是uchar的第几位的下标：
	 * @param index：必须 ≥ 0 且 < sz 。 */
	inline static int indexOfBit(const int index) {
		return index % BASE_SZ;
	}
	
	/** 根据布尔值的数量计算数据数组长度：
	 * @param size：必须 > 0 。 */
	inline static int mkCap(const int size) {
		return (size - 1) / BASE_SZ + 1;
	}
	
	/** 获取指定位的掩码：
	 * 1, 与返回值按位或可以让index位为1。
	 * 2, @param index：必须 ≥ 0 且 < 8 。 */
	inline static uchar mkMask(const int index) {
		uchar mask = 128;
		return mask >>= index;
	}
};

#endif // MASK_H











