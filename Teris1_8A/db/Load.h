#ifndef LOAD_H
#define LOAD_H

#include "../service/GamePanel.h"
#include "../exception/ExTemp.h"
#include <QString>
#include <QMap>
#include <memory>
using namespace std;

/** 游戏存档映射：
 * 1, 本类的对象是当前用户在内存中的游戏存档，不是保存到数据库的游戏存档字符串。
 * 2, 本类提供了方法实现本类对象与游戏存档字符串之间的转换。
 * @version 1.8A
 * @date 2018.09.18
 * @author 张良尧 */
class LoadData;
typedef shared_ptr<LoadData> Load; // 游戏存档的智能指针

class LoadData {
public:
	
	/**
	 * ================ 创建 ================
	 */
	
	/** 根据游戏存档字符串创建一个游戏存档映射的智能指针：
	 * 1, @param load ：数据库里的游戏存档字符串。
	 * 2, @param totalScore ：当前用户的总分数。 */
	inline static Load newLoad(const QString & load, qulonglong totalScore) {
		Load res(new LoadData(load, totalScore));
		return res;
	}
	
	/**
	 * ================ 析构 ================
	 */
	
	~LoadData();
	
	/**
	 * ================ 判断 ================
	 */
	
	inline bool isEmpty() const { return m_map.isEmpty(); }
	
	/**
	 * ================ 查 ================
	 */
	
	/** 一共有条存档数据 */
	inline int sz() const { return m_map.size(); }
	
	/**
	 * ================ 游戏存档解析 ================
	 * 
	 * 解析出的存档用映射集合保存。映射的key为GamePanel对象的指针，保存第几关、所得分数、下落形状、障碍物等；映射的value为下一形状。
	 */
	
	/** 将存档映射反解析为存档字符串 */
	QString invsAnlys() const;
	
	/** 更新存档：
	 * 1, @param gp ：当前关的游戏面板数据。
	 * 2, @param next ：存档时的下一形状。
	 * 3, 如果gp所在关已在存档映射中存在，则替换；如果不存在，则添加。 */
//	void updLoad(const GamePanel & gp, const Shape & next);
	
	
	// 未完成
	
	
	
	/**
	 * ================ 异常 ================
	 *
	 * 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
	 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。 */
	
	/** 游戏存档字符串异常：
	 * 游戏存档字符串非法，如没有以','开头或结尾。 */
	inline static void invalidLoadStrEx(const QString & ttl, const QString & load) {
		throw Exception(ttl, nullptr, "load string is invalid:\n" + load);
	}
	
	/** 游戏存档解析异常：
	 * 1, @param load ：游戏存档字符串。
	 * 2, @param begin、end ：存档字符串中有异常的部分的起、止位置。必须 ≥ 0，end必须 ≤ load.size()。 */
	inline static void anlysEx(const QString & ttl, const QString & load, int begin, int end) {
		throw Exception(ttl, nullptr,
						 "load analyses exception, the load string is: " + load.mid(begin, end));
	}
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	QMap<GamePanel *, Shape *> m_map;
	
	/**
	 * ~~~~~~~~~~~~ 内部构造 ~~~~~~~~~~~~
	 */
	
	LoadData();
	
	LoadData(const QString & load, qulonglong totalScore);
};

#endif // LOAD_H


