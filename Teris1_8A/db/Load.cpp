#include "Load.h"
#include <QDebug>

/**
 * ================ 析构 ================
 */

LoadData::~LoadData() {
	if(! m_map.isEmpty()) {
		QMapIterator<GamePanel *, Shape *> it(m_map);
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

QString LoadData::invsAnlys() const {
	
	
	// 未完成
	
	
}

/**
 * ================ 内部成员 ================
 */

/**
 * ~~~~~~~~~~~~ 内部构造 ~~~~~~~~~~~~
 */

LoadData::LoadData() {}

LoadData::LoadData(const QString & load, qulonglong totalScore) {
	if(! load.contains(ARR_ELMT_SEP)) invalidLoadStrEx(ET, load);
	if(load.contains(KEY_VAL_SEP)) {
		int cur = load.indexOf(KEY_VAL_SEP) + 1;
		int end = load.indexOf(ARR_ELMT_SEP, 1);
		
		qDebug() << "loading load:" << load.mid(cur, end);
		
		while(cur < load.size()) {
			int level = load.mid(cur, end).toInt();
			cur = end + 1;
			end = load.indexOf(KEY_VAL_SEP, cur);
			
			qDebug() << "loading load:" << load.mid(cur, end);
			
			qulonglong score = load.mid(cur, end).toULongLong();
			GamePanel * gp = new GamePanel(level, score, totalScore, nullptr);
			cur = end + 1;
			Shape * faller = Shape::anlys(load, & cur, gp);
			gp->setFaller(* faller);
			Shape * next = Shape::anlys(load, & cur, gp);
			cur = load.indexOf(ARR_ELMT_SEP, cur) + 1;
			end = load.indexOf(ARR_ELMT_SEP, cur);
			
			qDebug() << "loading load:" << load.mid(cur, end);
			
			int obsSz = load.mid(cur, end).toInt();
			for(int i = 0; i < obsSz; ++i) {
				gp->obs() += Shape::anlys(load, & cur, gp);
			}
			m_map.insert(gp, next);
		}
	} // load不包含KEY_VAL_SEP，则认为存档为空
	
	
	
	
	
	// 未完成
	
	
	
}


