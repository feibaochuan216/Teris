#ifndef NULLPTREXCEPTION_H
#define NULLPTREXCEPTION_H

/** 空指针异常：
 * @version 1.7B
 * @date 2018.07.29
 * @author 张良尧 */

#include "Exception.h"

class NullPtrException : public Exception {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	NullPtrException(
			const QString & file, const QString & func, const int line,
			const void * thisPtr, const QString & msg = "") noexcept;
	
	~NullPtrException() noexcept;
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	
	NullPtrException(const QString & title) noexcept;
	
};

#endif // NULLPTREXCEPTION_H
