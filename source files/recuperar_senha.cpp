#include "recuperar_senha.h"
#include "ui_recuperar_senha.h"

Recuperar_Senha::Recuperar_Senha(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Recuperar_Senha)
{
    ui->setupUi(this);
    ui->line_user->setPlaceholderText("Digite seu nome de usuário cadastrado");
    ui->line_email->setPlaceholderText("Digite o e-mail cadastrado");
}

Recuperar_Senha::~Recuperar_Senha()
{
    delete ui;
}

void Recuperar_Senha::on_button_voltar_clicked()
{
    emit home_buttonClicked();
}


void Recuperar_Senha::on_button_recuperar_senha_clicked()
{

    QSqlQuery procurar_email;
    procurar_email.prepare("SELECT * FROM registro_programadores WHERE email='"+ui->line_email->text()+"' AND nome_usuario='"+ui->line_user->text()+"'");

    if(procurar_email.exec()){
        int count = 0;

        while(procurar_email.next())
            count++;

        if(count == 1){
            senha_temp = this->GetRandomString();
            quint32 salt = QRandomGenerator::system()->generate();
            QString salt_senha_pepper = QString::number(salt) + senha_temp + pepper;
            QByteArray hash = QCryptographicHash::hash(salt_senha_pepper.toLocal8Bit(), QCryptographicHash::Md5).toHex();

            QSqlQuery atualizar_senha;
            atualizar_senha.prepare("UPDATE registro_programadores SET salt='"+QString::number(salt)+"', senha='"+hash+"' WHERE email='"+ui->line_email->text()+"' AND nome_usuario='"+ui->line_user->text()+"'");
            if(atualizar_senha.exec() && this->enviar_email()){
                QMessageBox::information(this, "Verificar e-mail", "Uma senha temporária foi enviada para seu e-mail, utilize-a para\nentrar na aplicação e modifique-a para uma nova senha");
                ui->line_email->clear();
                ui->line_user->clear();
                emit home_buttonClicked();
            }else{
                QMessageBox::information(this, "ERRO", "Um erro ocorreu ao modificar sua senha, tente novamente mais tarde");
            }
        }else{
            ui->label_erro->setText("E-mail/Usuário não encontrado no nosso banco de dados");
        }
    }
}

bool Recuperar_Senha::enviar_email(){
    bool enviado = false;

    //adicionamos o servidor SMTP
    SmtpClient smtp("smtp.example.com.br", 587); //valor ficticio

    //configurando usuario e senha
    smtp.setUser("exemplo@email.com.br");//valor ficticio
    smtp.setPassword("SENHA");//valor ficticio

    //Criando o MIME Object

    MimeMessage message;

    message.setSender(new EmailAddress("exemplo@email.com.br", "MV bugTracker"));
    message.addRecipient(new EmailAddress(ui->line_email->text(), "Usuário MV bugTracker"));
    message.setSubject("Recuperar Senha - MV bugTracker");

    // Adicionando texto ao objeto
    MimeText text;

    text.setText("Estaremos providenciando uma nova senha temporária para que você possa estar realizando login.\n\nSENHA TEMPORÁRIA: "+senha_temp+"\n\nSugerimos que modifique sua senha assim que você se conectar na aplicação desktop.\nPara modificar sua senha basta realizar o login e acessar o botão 'Configurações de Conta'.\n\n\nPara qualquer dúvida ou sugestão entre em contato em: marcusviniciusdrl@gmail.com");

    // Adiciona o texto ao corpo do e-mail

    message.addPart(&text);

    // Enviar o e-mail

    smtp.connectToHost();
    smtp.login();
    if(smtp.sendMail(message)){
        enviado = true;
    }
    smtp.quit();
    senha_temp.clear();
    return enviado;
}

QString Recuperar_Senha::GetRandomString() const{
    std::uniform_int_distribution<int> distribution(0, 9);
    QString nova_senha;
    for(int i = 0; i < 8; i++){
        int letra = distribution(*QRandomGenerator::global());
        nova_senha.append(QString::number(letra));
    }
    return nova_senha;
}
