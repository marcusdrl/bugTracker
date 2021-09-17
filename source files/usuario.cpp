#include "usuario.h"

Usuario::Usuario()
{

}

void Usuario::setNome(QString nome){
    this->_nome = nome;
}

QString Usuario::getNome(){
    return this->_nome;
}

void Usuario::setCargo(QString cargo){
    this->_cargo = cargo;
}

QString Usuario::getCargo(){
    return this->_cargo;
}

void Usuario::setId(QString id){
    this->_id = id;
}

QString Usuario::getId(){
    return this->_id;
}

void Usuario::setEmpresa(QString empresa){
    this->_empresa = empresa;
}

QString Usuario::getEmpresa(){
    return this->_empresa;
}

void Usuario::setEmail(QString email){
    this->_email = email;
}

QString Usuario::getEmail(){
    return this->_email;
}

void Usuario::setStatus(QString status){
    this->_status_pedido = status;
}

QString Usuario::getStatus(){
    return this->_status_pedido;
}

void Usuario::limpaCache(){
    this->_cargo="";
    this->_nome="";
    this->_id="";
    this->_empresa="";
    this->_email="";
}


