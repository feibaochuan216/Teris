#include "Usr.h"

/**
 * ================ 构造 ================
 */

Usr::Usr() {}

Usr::Usr(const QString & usrNm, const QString & pswd,
		 UsrType type/* = Normal*/) 
	: m_usrNm(usrNm), m_pswd(pswd), m_type(type) {}




