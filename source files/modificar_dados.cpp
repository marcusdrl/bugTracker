#include "modificar_dados.h"
#include "ui_modificar_dados.h"
#include <QIcon>
#include <QMessageBox>

Modificar_Dados::Modificar_Dados(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modificar_Dados)
{
    ui->setupUi(this);

    this->setWindowTitle("Modificar Dados");
    this->setWindowIcon(QIcon(":/logo/C:/Users/Marcus Vinicius/Downloads/bug_logo.png"));

    ui->line_dadosemail1->setPlaceholderText("Digite o novo e-mail");
    ui->line_dadosemail2->setPlaceholderText("Confirme o novo e-mail");
    ui->combo_dadoscargos->setPlaceholderText("Escolha um cargo");

    ui->label_6->setWordWrap(true);
    ui->label_5->setWordWrap(true);
}

Modificar_Dados::~Modificar_Dados()
{
    delete ui;
}

void Modificar_Dados::on_button_dados_enviarmod_clicked()
{
    QString email1 = ui->line_dadosemail1->text();
    QRegularExpression regmail("^[A-Z0-9a-z._-]{1,}@(\\w+)(\\.(\\w+))(\\.(\\w+))?(\\.(\\w+))?$");
    QRegularExpressionMatch match1 = regmail.match(email1);

    QString email2 = ui->line_dadosemail2->text();
    QRegularExpressionMatch match2 = regmail.match(email2);
    if(ui->line_dadosemail1->text() != "" && ui->line_dadosemail2->text() != "" && ui->line_dadosemail1->text() == ui->line_dadosemail2->text() && match1.hasMatch() && match2.hasMatch()){
        this->email_mod = ui->line_dadosemail1->text();
        this->cargo_mod = ui->combo_dadoscargos->currentText();
        emit envia_modificacoes();
    }else if(ui->line_dadosemail1->text() == "" && ui->line_dadosemail2->text() == ""){
        this->email_mod = ui->line_dadosemail1->text();
        this->cargo_mod = ui->combo_dadoscargos->currentText();
        emit envia_modificacoes();
    }else
        QMessageBox::information(this, "Campos preenchido incorretamente", "Favor conferir o preenchimento dos campos de e-mail.");
}

QString Modificar_Dados::getEmail(){
    return this->email;
}

QString Modificar_Dados::getCargo(){
    return this->cargo;
}

QString Modificar_Dados::getEmail_mod(){
    return this->email_mod;
}

QString Modificar_Dados::getCargo_mod(){
    return this->cargo_mod;
}

void Modificar_Dados::checa_cargo(QString cargo_atual){
    if(cargo_atual == "Desenvolvedor Jr.")
        ui->combo_dadoscargos->setCurrentIndex(0);
    else if(cargo_atual == "Desenvolvedor Sr.")
        ui->combo_dadoscargos->setCurrentIndex(1);
    else if(cargo_atual == "Admin")
        ui->combo_dadoscargos->setCurrentIndex(2);
    else if(cargo_atual == "Manager")
        ui->combo_dadoscargos->setCurrentIndex(3);
}

void Modificar_Dados::setCargo(QString cargo_atual){
    this->cargo = cargo_atual;

    if(cargo == "Manager")
        ui->combo_dadoscargos->addItem("Manager");
}

void Modificar_Dados::setEmail(QString email_atual){
    this->email = email_atual;
}

void Modificar_Dados::clear(){
    ui->line_dadosemail1->clear();
    ui->line_dadosemail2->clear();
    ui->combo_dadoscargos->removeItem(3);
    this->close();
}
