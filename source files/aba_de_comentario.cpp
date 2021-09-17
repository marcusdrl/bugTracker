#include "aba_de_comentario.h"
#include "ui_aba_de_comentario.h"

Aba_De_Comentario::Aba_De_Comentario(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Aba_De_Comentario)
{
    ui->setupUi(this);
    this->setWindowTitle("Novo Comentário");
    this->setWindowIcon(QIcon(":/logo/C:/Users/Marcus Vinicius/Downloads/bug_logo.png"));

    ui->text_comentario->setPlaceholderText("Digite seu comentário, utilizando até 450 caracteres");
}

Aba_De_Comentario::~Aba_De_Comentario()
{
    delete ui;
}

void Aba_De_Comentario::on_button_cancelacoment_clicked()
{
    this->close();
}


void Aba_De_Comentario::on_button_enviacoment_clicked()
{
    texto = ui->text_comentario->toPlainText();
    if(texto.length() < 450){
        emit enviar_comentario();
        ui->text_comentario->clear();
        this->close();
    }else{
        QMessageBox::warning(this, "Erro de Texto", "Tamanho do texto excedeu o número máximo de caracteres permitido.");
    }
}

void Aba_De_Comentario::on_text_comentario_textChanged()
{
    tracker = ui->text_comentario->toPlainText();
    contagem = tracker.length();
    ui->label_chars->setText(QString::number(450 - contagem) + " caracteres restantes");

    if(contagem > 450){
        ui->label_chars->setStyleSheet("color: red;");
        ui->label_chars->setText("Você excedeu o número máximo de caracteres para o comentário");
    }
    else
        ui->label_chars->setStyleSheet("color: black;");
}

