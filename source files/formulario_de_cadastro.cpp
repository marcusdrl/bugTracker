#include "formulario_de_cadastro.h"
#include "ui_formulario_de_cadastro.h"

Formulario_de_Cadastro::Formulario_de_Cadastro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formulario_de_Cadastro)
{
    ui->setupUi(this);

    //determina tamanho máximo dos campos de senha e usuario
    ui->line_senha1->setMaxLength(8);
    ui->line_senha2->setMaxLength(8);
    ui->line_username->setMaxLength(16);

    //placeholders para os line edits
    ui->line_senha1->setPlaceholderText("Digite sua senha (min 4, max 8 caracteres)");
    ui->line_senha2->setPlaceholderText("Confirme sua senha");
    ui->line_username->setPlaceholderText("Digite o nome de usuário (min 6, max 16 caracteres)");
    ui->line_email1->setPlaceholderText("Digite seu e-mail");
    ui->line_email2->setPlaceholderText("Confirme seu e-mail");

}

Formulario_de_Cadastro::~Formulario_de_Cadastro()
{
    delete ui;
}

void Formulario_de_Cadastro::on_button_voltar_clicked()
{
    emit home_buttonClicked();
}

void Formulario_de_Cadastro::on_button_enviacadastro_clicked()
{
    QString email1 = ui->line_email1->text();
    QRegularExpression regmail("^[A-Z0-9a-z._-]{1,}@(\\w+)(\\.(\\w+))(\\.(\\w+))?(\\.(\\w+))?$");
    QRegularExpressionMatch match1 = regmail.match(email1);

    QString email2 = ui->line_email2->text();
    QRegularExpressionMatch match2 = regmail.match(email2);


    if(this->checa_campos_preenchidos()){
        if(ui->line_senha1->text() == ui->line_senha2->text() && match1.hasMatch() && match2.hasMatch() && ui->line_email1->text() == ui->line_email2->text()){
            //query para adicionar novo usuario ao DB
            QSqlQuery adiciona_ao_db;
            adiciona_ao_db.prepare("insert into registro_programadores (nome_usuario, nome_programador,salt, senha, email, permissao)"
                                "values(?, ?, ?, ?, ?, ?)");
            adiciona_ao_db.addBindValue(ui->line_username->text());
            adiciona_ao_db.addBindValue(ui->line_nomecompleto->text());

            //criptografando senha (salt + hash + pepper)
            quint32 salt = QRandomGenerator::system()->generate();
            QString salt_senha_pepper = QString::number(salt) + ui->line_senha1->text() + pepper;
            QByteArray hash = QCryptographicHash::hash(salt_senha_pepper.toLocal8Bit(), QCryptographicHash::Md5).toHex();
            adiciona_ao_db.addBindValue(salt);  //salt
            adiciona_ao_db.addBindValue(hash);

            adiciona_ao_db.addBindValue(ui->line_email1->text());
            adiciona_ao_db.addBindValue(ui->comboFuncao->currentText());

            //query para checar se o username ja esta registrado
            QSqlQuery checa_email;
            checa_email.prepare("select * from registro_programadores where email='"+ui->line_email1->text()+"'");

            QSqlQuery checa_username;
            checa_username.prepare("select * from registro_programadores where nome_usuario='"+ui->line_username->text()+"'");
            if(checa_username.exec()){
                int count_username = 0;
                int count_email = 0;

                checa_email.exec();

                //itera sobre todos os usuarios para checar username
                while(checa_username.next())
                    count_username++;

                //itera sobre todos os usuarios para checar o email
                while(checa_email.next())
                    count_email++;

                if(count_username >= 1)
                    QMessageBox::warning(this, "Usuario Existente", "Não foi possível realizar o cadastro porque o nome de usuário já está sendo utilizado");
                else if(count_email >= 1){
                    QMessageBox::warning(this, "Usuario Existente", "Não foi possível realizar o cadastro porque o e-mail já está sendo utilizado");
                }else if(count_username == 0 && count_email == 0){
                    if(adiciona_ao_db.exec() && this->enviar_email())
                        QMessageBox::information(this,"Cadastro Realizado", "Seu cadastro foi concluído com sucesso.\nAcesse seu e-mail para realizar a ativação do cadastro.");


                    //limpa os campos preenchidos no formulário
                    ui->line_email1->clear();
                    ui->line_email2->clear();
                    ui->line_nomecompleto->clear();
                    ui->line_senha1->clear();
                    ui->line_senha2->clear();
                    ui->line_username->clear();

                    //volta para a pagina de login
                    emit home_buttonClicked();
                }
            }
        }else if(ui->line_senha1->text() != ui->line_senha2->text())
            QMessageBox::information(this, "Campos preenchidos incorretamente", "As senhas inseridas são diferentes");
        else if(!match1.hasMatch())
            QMessageBox::information(this, "E-mail", "O e-mail inserido na primeira linha é inválido.\nFavor utilizar algo parecido com - Ex: wolhaiksong@gmail.com");
        else if(!match2.hasMatch())
            QMessageBox::information(this, "E-mail", "O e-mail inserido na linha de confirmação de e-mail é inválido.\nFavor utilizar algo parecido com - Ex: wolhaiksong@gmail.com");
    }else
        QMessageBox::information(this, "Erro", "Favor preencher os campos corretamente sem deixar nenhum em branco.");
}


bool Formulario_de_Cadastro::checa_campos_preenchidos(){
    if(ui->line_email1->text().isEmpty() || ui->line_email2->text().isEmpty() || ui->line_nomecompleto->text().isEmpty()
            || ui->line_senha1->text().isEmpty() || ui->line_senha2->text().isEmpty() || ui->line_username->text().isEmpty())
        return false;
    else
        return true;
}

bool Formulario_de_Cadastro::enviar_email(){
    bool enviado = false;

    //adicionamos o servidor SMTP
    SmtpClient smtp("examplo.smtp.com.br", 587); //valor ficticio de SMTP

    //configurando usuario e senha
    smtp.setUser("exemplo@email.com.br"); //valor ficticio para email
    smtp.setPassword("SENHA"); //valor ficticio para senha

    //Criando o MIME Object

    MimeMessage message;

    message.setSender(new EmailAddress("exemplo@email.com.br", "MV bugTracker"));
    message.addRecipient(new EmailAddress(ui->line_email1->text(), ui->line_nomecompleto->text()));
    message.setSubject("Validação de Cadastro - MV bugTracker");

    // Adicionando texto ao objeto
    MimeText text;

    text.setText("Obrigado por se cadastrar no meu projeto.\n\nPara qualquer dúvida ou sugestão entre em contato em: marcusviniciusdrl@gmail.com\n\nPara continuar utilizando a aplicação favor validar seu cadastro com o link: www.mvbugtracker.com.br/views/login.html \nBasta realizar login com as credenciais cadastradas que você estará pronto para começar a utilizar o bug tracker.\n\n\nAgradeço novamente, atenciosamente Marcus Vinicius.");

    // Adiciona o texto ao corpo do e-mail

    message.addPart(&text);

    // Enviar o e-mail

    smtp.connectToHost();
    smtp.login();
    if(smtp.sendMail(message)){
        enviado = true;
    }
    smtp.quit();

    return enviado;
}


