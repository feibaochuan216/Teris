#include "show/GameDlg.h"
#include <QApplication>

int main(int argc, char * argv[]){
	QApplication a(argc, argv);
	GameDlg w;
	w.show();
	return a.exec();
}


