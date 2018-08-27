#ifndef GEOMETRYEXCEPTION_H
#define GEOMETRYEXCEPTION_H

/** 几何异常：
 * 1, 几何逻辑错误类的异常。
 * @version 1.7B
 * @date 2018.07.29
 * @author 张良尧 */

#include "Exception.h"

class GeomException : public Exception {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	GeomException(
			const QString & file, const QString & func, const int line,
			const void * thisPtr, const QString & msg = "") noexcept;
	
	~GeomException() noexcept;
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	
	GeomException(const QString & title) noexcept;
};

#endif // GEOMETRYEXCEPTION_H
