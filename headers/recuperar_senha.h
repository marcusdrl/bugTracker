#ifndef RECUPERAR_SENHA_H
#define RECUPERAR_SENHA_H

#include <QWidget>
#include <QSqlQuery>
#include <SmtpMime>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QMessageBox>

namespace Ui {
class Recuperar_Senha;
}

class Recuperar_Senha : public QWidget
{
    Q_OBJECT

public:
    explicit Recuperar_Senha(QWidget *parent = nullptr);
    ~Recuperar_Senha();

private slots:
    void on_button_voltar_clicked();

    void on_button_recuperar_senha_clicked();

signals:
    void home_buttonClicked();

private:
    Ui::Recuperar_Senha *ui;
    QString senha_temp;
    QString pepper = ""; //inserir valor de pepper

    bool enviar_email();
    QString GetRandomString() const;
};

#endif // RECUPERAR_SENHA_H
