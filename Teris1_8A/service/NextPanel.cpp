#include "NextPanel.h"

/**
 * ================ 构造、析构 ================
 */

NextPanel::NextPanel(Service * parent/* = nullptr*/)
	: Service(parent), m_shp(new Shape(nullptr)),
	  m_client(nullptr) { // 必须为NULL，防止循环构造
	m_shp->setParent(this);
}

NextPanel::~NextPanel() {
	if(nullptr != m_shp) delete m_shp;
}

/**
 * ================ 查 ================
 */

int NextPanel::xPix() const {
	return m_client->xPixNp();
}

int NextPanel::yPix() const {
	return m_client->yPixNp();
}

int NextPanel::widthPix() const {
	return (SHP_SZ + 2) * LTC_SZ;
}

int NextPanel::heightPix() const {
	return (SHP_SZ + 2) * LTC_SZ;
}

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

void NextPanel::print(const int level/* = 0*/) const {
	indent(level);
	cout << "NextPanel: " << this << " = {parent: " << m_parent
		 << ", client: " << m_client << " = \'"
		 << m_client->objectName().toStdString() << "\'," << endl;
	m_shp->print(level + 1);
	indent(level);
	cout << '}' << endl;
}

#endif // DEBUG












