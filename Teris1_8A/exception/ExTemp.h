#ifndef EXTEMP_H
#define EXTEMP_H

#include "Exception.h"
#include <QString>

/** 异常模板：
 * 
 */
template<typename T>
class ExTemp : public Exception {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	/** 1, @param ttl：标题，即抛出异常代码在哪个源码文件、哪个函数、哪个行号。
	 * 2, @param thisPtr ：抛出异常的对象的地址。
	 * 3, @param msg ：异常描述信息。 */
	ExTemp(const QString & ttl, const void * thisPtr, const QString & msg = "") noexcept;
	
	virtual ~ExTemp() noexcept;
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	
	T m_nm; // 异常名称
	
	/**
	 * ~~~~~~~~~~~~ 内部构造 ~~~~~~~~~~~~
	 */
	
	/** 用于继承 */
	ExTemp(const QString & title) noexcept;
	
	/** 用于模板 */
	ExTemp(const T & t) noexcept;
};

/**
 * ================ 模板实例 ================
 */

class DbEx {}; // 数据库异常
class GeomEx {}; // 几何关系上的异常
class NullPtrEx {}; // 空指针异常
class NullParentEx {}; // 空上级物体指针异常
class OutRangeEx {}; // 下标或指针越界异常
class OutRangeGeomEx {}; // 几何关系上超出范围的异常
class LoopRelatEx {}; // 循环关系异常，如上级的上级是自己

#include "ExTemp.cpp"

#endif // EXTEMP_H
