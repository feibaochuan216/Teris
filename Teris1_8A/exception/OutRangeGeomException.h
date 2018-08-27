#ifndef OUTRANGEGEOMETRYEXCEPTION_H
#define OUTRANGEGEOMETRYEXCEPTION_H

/** 超范围异常：
 * 1, 在图形界面中，像素坐标 < 0 或 > 窗口的宽高，则抛出本异常。
 * @version 1.7B
 * @date 2018.07.29
 * @author 张良尧 */

#include "GeomException.h"

class OutRangeGeomException : GeomException {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	OutRangeGeomException(
			const QString & file, const QString & func, const int line,
			const void * thisPtr, const QString & msg = "") noexcept;
	
	~OutRangeGeomException() noexcept;
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	
	OutRangeGeomException(const QString & title) noexcept;
	
};

#endif // OUTRANGEGEOMETRYEXCEPTION_H
