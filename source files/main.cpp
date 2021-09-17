#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //adiciona translator
    QTranslator qtTranslator;
    if (qtTranslator.load(QLocale::system(),
                "qt", "_",
                QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
    {
        a.installTranslator(&qtTranslator);
    }

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
    {
        a.installTranslator(&qtBaseTranslator);
    }

    //cria variavel LOGIN
    Login w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();

    //TODO adicionar tradutor??
}
