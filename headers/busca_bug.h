#ifndef BUSCA_BUG_H
#define BUSCA_BUG_H

#include <QString>

class Busca_Bug
{
private:
    QString data_hora = "";
    QString titulo = "";
    QString bug_id = "";

public:
    Busca_Bug();
    void setData_hora(QString _data_hora) { this->data_hora = _data_hora; }
    QString getData_hora() {return this->data_hora;}
    void setTitulo(QString _titulo) { this->titulo = _titulo; }
    QString getTitulo() { return this->titulo; }
    void setBug_id(QString bug_id) { this->bug_id = bug_id; }
    QString getBug_id() { return this->bug_id; }

    void limpaCache() { this->data_hora = ""; this->titulo = ""; this->bug_id = "";}

};

#endif // BUSCA_BUG_H
