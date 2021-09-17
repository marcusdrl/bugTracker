#ifndef MODIFICAR_SENHA_H
#define MODIFICAR_SENHA_H

#include <QWidget>

namespace Ui {
class Modificar_Senha;
}

class Modificar_Senha : public QWidget
{
    Q_OBJECT

public:
    explicit Modificar_Senha(QWidget *parent = nullptr);
    ~Modificar_Senha();

    QString getSenha();
    void clear();

private slots:
    void on_button_dados_enviarsenha_clicked();

signals:
    void enviar_nova_senha();

private:
    Ui::Modificar_Senha *ui;
    QString nova_senha;
};

#endif // MODIFICAR_SENHA_H
