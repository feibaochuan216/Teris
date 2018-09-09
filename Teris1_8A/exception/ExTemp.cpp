#include "ExTemp.h"
#include <typeinfo>

/**
 * ================ 构造、析构 ================
 */

template<typename T>
ExTemp<T>::ExTemp (const QString & ttl, const void * thisPtr, const QString & msg/* = ""*/) noexcept
	: Exception(typeid(T).name()) {
	m_title += ttl;
	m_title += " : 0x";
	m_title += QString::number((quint64)thisPtr, 16);
	m_title += " :\n";
	m_msg += m_title;
	m_msg += msg;
	print();
}

template<typename T>
ExTemp<T>::~ExTemp() noexcept {}

/**
 * ================ 仅内部使用 ================
 */

/** 用于子类 */
template<typename T>
ExTemp<T>::ExTemp(const QString & title) noexcept
	: Exception(title) {}

/** 用于模板 */
template<typename T>
ExTemp<T>::ExTemp(const T & t) noexcept : m_nm(t) {}









