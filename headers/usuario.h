#ifndef USUARIO_H
#define USUARIO_H

#include <QString>
#include <QWidget>

class Usuario
{
private:
    QString _nome;
    QString _cargo;
    QString _id;
    QString _empresa;
    QString _email;
    QString _status_pedido;

public:
    Usuario();

    void setNome(QString nome);
    QString getNome();
    void setCargo(QString cargo);
    QString getCargo();
    void setId(QString id);
    QString getId();
    void setEmpresa(QString empresa);
    QString getEmpresa();
    void setEmail(QString email);
    QString getEmail();
    void setStatus(QString status);
    QString getStatus();

    void limpaCache();

};

#endif // USUARIO_H
