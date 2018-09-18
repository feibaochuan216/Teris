#include "Usr.h"

/**
 * ================ 析构 ================
 */

UsrData::~UsrData() {}

/**
 * ================ 内部成员 ================
 */

/**
 * ~~~~~~~~~~~~ 内部构造 ~~~~~~~~~~~~
 */

UsrData::UsrData(const QString & usrNm, UsrType type, qulonglong totalScore, const QString & load)
	: m_usrNm(usrNm), m_type(type), m_totalScore(totalScore) {
	if(! load.isEmpty()) {
		
		
		// 未完成：解析数据库的游戏存档字符串
		
		
	}
}




