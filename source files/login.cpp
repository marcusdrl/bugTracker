#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->stackedWidget);
    this->setWindowIcon(QIcon(":/logo/C:/Users/Marcus Vinicius/Downloads/bug_logo.png"));
    this->setWindowTitle("MV bugTracker");

    //configurações de conexão com o DATABASE (valores ficticios)
    database.setHostName("enderecodb@hostname");
    database.setUserName("username");
    database.setPassword("password");
    database.setPort(3306);
    database.setDatabaseName("DBname");
    database.setConnectOptions("MYSQL_OPT_RECONNECT=TRUE"); //connectar automaticamente ao DB

    //checa se foi possivel abrir a database
    if(!database.open())
        QMessageBox::warning(this, "Erro", "Não foi possível se conectar ao DB!");

    //inicializar na pagina de login
    ui->stackedWidget->setCurrentIndex(0);

    //link com outras stackedWidgets
    ui->stackedWidget->insertWidget(1, &formulario);
    ui->stackedWidget->insertWidget(2, &gerenciador);
    ui->stackedWidget->insertWidget(3, &recuperador);

    //conexões
    connect(&gerenciador, SIGNAL(home_buttonClicked()), this, SLOT(home_button()));
    connect(&formulario, SIGNAL(home_buttonClicked()), this, SLOT(home_button()));
    connect(&recuperador, SIGNAL(home_buttonClicked()), this, SLOT(home_button()));
}

Login::~Login()
{
    delete ui;
}

void Login::home_button(){
    ui->stackedWidget->setCurrentIndex(0);
}

void Login::on_button_entrar_clicked()
{
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply* reply = nam.get(req);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->bytesAvailable()){
        QSqlQuery query;
        query.prepare("select * from registro_programadores where nome_usuario='"+ui->line_username->text()+"'");

        if(query.exec()){
            int count = 0;

            while(query.next()){
                count++;
            }
            if(count == 1){
                query.first();
                QString senha = QCryptographicHash::hash((query.value(3).toString() + ui->line_password->text() + pepper).toLocal8Bit(),
                                                         QCryptographicHash::Md5).toHex();
                //checa se a senha confere
                if(senha == query.value(4).toString() && query.value(10) == true){
                    //limpa campos preenchidos
                    ui->line_username->clear();
                    ui->line_password->clear();
                    ui->label_erro->clear();
                    ui->label_erro2->clear();

                    //busca dados para tela inicial do aplicativo
                    gerenciador.iniciaTimer();

                    //troca a pagina do stackedWidget
                    ui->stackedWidget->setCurrentIndex(2);

                    //atualiza o usuario logado
                    gerenciador.atualizaLogado(query);
                    gerenciador.atualizaTickets();
                    gerenciador.atualizaContagem();

                }else{
                    ui->line_username->setFocus();
                    ui->label_erro->setText("Senha incorreta ou você ainda não ativou sua conta");
                    ui->label_erro->setStyleSheet("color: red;");

                    ui->label_erro2->setText("\nAcesse <a href=\"http://mvbugtracker.com.br/views/login.html\">nosso site</a> para ativar");
                    ui->label_erro2->setTextFormat(Qt::RichText);
                    ui->label_erro2->setTextInteractionFlags(Qt::TextBrowserInteraction);
                    ui->label_erro2->setOpenExternalLinks(true);
                    ui->label_erro2->setStyleSheet("color: red;");
                }
            }else{
                ui->label_erro->setText("Nome de usuário e(ou) senha incorretos");
                ui->label_erro->setStyleSheet("color: red");
            }
        }
    }else{
        ui->label_erro->setText("Usuário não possui conexão com a internet");
        ui->label_erro->setStyleSheet("color: red;");
    }
}

void Login::on_button_cadastrar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Login::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(this, "Confimar para encerrar", "Deseja encerrar o programa?", QMessageBox::Yes | QMessageBox::No);
    if(botao == QMessageBox::Yes){
        event->accept();
        QCoreApplication::quit();
    }else
        return;
}


void Login::on_button_forgotpassword_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

