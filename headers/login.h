#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>
#include <QTimer>
#include "gerenciador.h"
#include <QString>
#include "formulario_de_cadastro.h"
#include <QCloseEvent>
#include "recuperar_senha.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_button_entrar_clicked();
    void on_button_cadastrar_clicked();
    void home_button();
    void closeEvent(QCloseEvent *event);
    void on_button_forgotpassword_clicked();

private:
    Ui::Login *ui;
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
    Gerenciador gerenciador;
    Formulario_de_Cadastro formulario;
    Recuperar_Senha recuperador;
    QString pepper = ""; //inserir valor de pepper

};
#endif // LOGIN_H
