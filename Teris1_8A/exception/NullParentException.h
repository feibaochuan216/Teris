#ifndef NULLPARENTEXCEPTION_H
#define NULLPARENTEXCEPTION_H

/** 无父对象异常：
 * 业务逻辑类型的对象大部分需要有父对象地址的成员变量，如果该有父对象的没有，则会抛此异常。
 * @version 1.7B
 * @date 2018.07.29
 * @author 张良尧 */

#include "NullPtrException.h"

class NullParentException : public NullPtrException {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	NullParentException(
			const QString & file, const QString & func, const int line,
			const void * thisPtr, const QString & msg = "") noexcept;
	
	~NullParentException() noexcept;
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	
	NullParentException(const QString & title) noexcept;
};

#endif // NULLPARENTEXCEPTION_H
