#ifndef OUTRANGEEXCEPTION_H
#define OUTRANGEEXCEPTION_H

#include "Exception.h"

class OutRangeException : public Exception {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	OutRangeException(
			const QString & file, const QString & func, const int line,
			const void * thisPtr, const QString & msg = "") noexcept;
	
	~OutRangeException() noexcept;
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	
	OutRangeException(const QString & title) noexcept;
	
};

#endif // OUTRANGEEXCEPTION_H
