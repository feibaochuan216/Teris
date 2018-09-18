#include "Usr.h"

/**
 * ================ 析构 ================
 */

UsrData::~UsrData() {
	if(! m_load.isEmpty()) {
		QMapIterator<GamePanel *, Shape *> it(m_load);
		while(it.hasNext()) {
			it.next();
			delete it.key();
			delete it.value();
		}
	}
}

/**
 * ================ 游戏存档解析 ================
 */

QMap<GamePanel *, Shape *> anlys(const QString & load) {
	
	
	
	
	
	
	
	
	
	
	// 未完成
	
	
}

QString invsAnlys(const QMap<GamePanel *, Shape *> & map) {
	
	
	// 未完成
	
	
}

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




