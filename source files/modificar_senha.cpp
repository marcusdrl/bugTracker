#include "modificar_senha.h"
#include "ui_modificar_senha.h"
#include <QIcon>

Modificar_Senha::Modificar_Senha(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modificar_Senha)
{
    ui->setupUi(this);

    this->setWindowTitle("Modificar Senha");
    this->setWindowIcon(QIcon(":/logo/C:/Users/Marcus Vinicius/Downloads/bug_logo.png"));

    ui->line_dadossenha1->setPlaceholderText("Digite sua nova senha");
    ui->line_dadossenha2->setPlaceholderText("Confirme sua nova senha");

    ui->line_dadossenha1->setMaxLength(8);
    ui->line_dadossenha2->setMaxLength(8);
}

Modificar_Senha::~Modificar_Senha()
{
    delete ui;
}

void Modificar_Senha::on_button_dados_enviarsenha_clicked()
{
    if(ui->line_dadossenha1->text() != "" && ui->line_dadossenha2->text() != "" && ui->line_dadossenha1->text() == ui->line_dadossenha2->text()){
        this->nova_senha = ui->line_dadossenha1->text();
        emit enviar_nova_senha();
    }
}

QString Modificar_Senha::getSenha(){
    return this->nova_senha;
}

void Modificar_Senha::clear(){
    ui->line_dadossenha1->clear();
    ui->line_dadossenha2->clear();
    this->close();
}
