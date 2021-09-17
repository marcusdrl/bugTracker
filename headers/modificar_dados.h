#ifndef MODIFICAR_DADOS_H
#define MODIFICAR_DADOS_H

#include <QWidget>

namespace Ui {
class Modificar_Dados;
}

class Modificar_Dados : public QWidget
{
    Q_OBJECT

public:
    explicit Modificar_Dados(QWidget *parent = nullptr);
    ~Modificar_Dados();

    QString getEmail();
    QString getCargo();
    QString getCargo_mod();
    QString getEmail_mod();
    void setCargo(QString cargo_atual);
    void setEmail(QString email_atual);
    void checa_cargo(QString cargo_atual);
    void clear();

private slots:
    void on_button_dados_enviarmod_clicked();

signals:
    void envia_modificacoes();

private:
    Ui::Modificar_Dados *ui;
    QString email_mod;
    QString email;
    QString cargo_mod;
    QString cargo;
};

#endif // MODIFICAR_DADOS_H
