#ifndef FORMULARIO_DE_CADASTRO_H
#define FORMULARIO_DE_CADASTRO_H

#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <SmtpMime>

namespace Ui {
class Formulario_de_Cadastro;
}

class Formulario_de_Cadastro : public QWidget
{
    Q_OBJECT

public:
    explicit Formulario_de_Cadastro(QWidget *parent = nullptr);
    ~Formulario_de_Cadastro();

private:
    Ui::Formulario_de_Cadastro *ui;
    QString pepper = ""; //inserir string pepper

    bool checa_campos_preenchidos();

signals:
    void home_buttonClicked();
private slots:
    bool enviar_email();
    void on_button_voltar_clicked();
    void on_button_enviacadastro_clicked();

};

#endif // FORMULARIO_DE_CADASTRO_H
