#include "gerenciador.h"
#include "ui_gerenciador.h"
#include<QThread>

Gerenciador::Gerenciador(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gerenciador)
{
    ui->setupUi(this);

    //pre-definicoes de inicialização geral
    ui->button_novo->setVisible(false);
    ui->label_19->setVisible(false);
    ui->combo_desenvolvedoredit->setVisible(false);
    ui->button_gerenciador_projetos->setVisible(false);
    ui->button_regprojeto->setVisible(false);
    ui->button_tickets->setVisible(false);
    ui->button_projetos->setVisible(false);
    ui->button_solved_equipe->setVisible(false);
    ui->button_solved->setVisible(false);
    ui->button_funcionarios->setVisible(false);
    ui->button_meusprojetos->setVisible(false);
    ui->button_tickets_equipe->setVisible(false);
    ui->text_descricaoedit->setReadOnly(true);
    ui->button_geraplic->setVisible(false);
    ui->text_projdesc->setReadOnly(true);
    ui->stacked_gerenciador->setCurrentIndex(0);
    ui->button_proj_modmanager->setVisible(false);
    ui->combo_projmanager->setEnabled(false);
    ui->label_32->setWordWrap(true);
    ui->label_68->setWordWrap(true);
    ui->label_61->setWordWrap(true);

    //pre-definicoes para registro de Qfile
    auto fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath(QDir::rootPath());
    ui->line_filename->setCompleter(new QCompleter(fileSystemModel, this));
    ui->line_filename->setReadOnly(true);

    //pre-definicoes para registro de tickets
    ui->combo_projeto->setPlaceholderText("Escolha um projeto");
    ui->combo_desenvolvedor->setPlaceholderText("Escolha um desenvolvedor");
    ui->line_filename->setPlaceholderText("Máximo de 16 MB para o arquivo");
    ui->text_descricao->setReadOnly(true);

    //pre-definicoes para registro de empresa/cadastro para entrar em empresa
    ui->line_nomregempresa->setPlaceholderText("Digite o nome da sua empresa");
    ui->text_descricaoempresa->setPlaceholderText("Digite a descrição da empresa.\nEx: setor de atuação, quais tipos de projeto está trabalhando ou pretende trabalhar");

    //pre-definicoes para dados de funcionarios
    ui->line_funcnome->setReadOnly(true);
    ui->line_funccargo->setReadOnly(true);
    ui->line_funcemail->setReadOnly(true);
    ui->line_funcid->setReadOnly(true);

    //pre-definicoes para editar ticket
    ui->button_enviaredit->setDisabled(true);

    //pre-definicoes para procurar empresa
    ui->line_procurarempresa->setPlaceholderText("Digite o nome/nº de registro da empresa");

    //definição de timer para query a cada 30s, se encontrar algo retorna uma mensagem com o evento registrado no DB
    tempo_sessao = new QTimer(this);
    tempo_sessao->start(1000); //1000 equivale a 1segundo, se diminuir o tempo passa mais rapido

    //conexões
    connect(&aba_comentario, SIGNAL(enviar_comentario()), this, SLOT(envio_de_comentario()));
    connect(&configuracoes, SIGNAL(envia_modificacoes()), this, SLOT(envio_de_modificacoes()));
    connect(&definir_senha, SIGNAL(enviar_nova_senha()), this, SLOT(envio_de_nova_senha()));
    connect(tempo_sessao, SIGNAL(timeout()), this, SLOT(atualiza_temporizador()));
}

Gerenciador::~Gerenciador()
{
    delete ui;
    delete tempo_sessao;
    delete tempo_query;
}

void Gerenciador::atualizaLogado(QSqlQuery query){
    query.first();
    usuario_conectado.setNome(query.value(2).toString());
    usuario_conectado.setCargo(query.value(6).toString());
    usuario_conectado.setId(query.value(0).toString());
    usuario_conectado.setEmpresa(query.value(7).toString());
    usuario_conectado.setEmail(query.value(5).toString());
    usuario_conectado.setStatus(query.value(8).toString());

    ui->label_welcome->setText("Bem vindo, "+usuario_conectado.getNome());
    ui->label_cargo->setText("Logado como: "+usuario_conectado.getCargo());

    if(usuario_conectado.getCargo() == "Admin" && usuario_conectado.getEmpresa() != ""){
        ui->button_tickets->setVisible(true);
        ui->button_projetos->setVisible(true);
        ui->button_solved->setVisible(true);
        ui->button_tickets_equipe->setVisible(true);
        ui->button_novo->setVisible(true);
        ui->label_19->setVisible(true);
        ui->combo_desenvolvedoredit->setVisible(true);
        ui->button_solved_equipe->setVisible(true);
        ui->button_gerenciador_projetos->setVisible(true);
        ui->button_regprojeto->setVisible(true);
        ui->button_meusprojetos->setVisible(true);
        ui->button_funcionarios->setVisible(true);
        ui->button_dadosempresa->setVisible(false);
        ui->combo_projmanager->setEnabled(true);
        ui->button_proj_modmanager->setVisible(true);
        ui->button_geraplic->setVisible(true);
    }else if(usuario_conectado.getCargo() == "Manager" && usuario_conectado.getEmpresa() != ""){
        ui->button_tickets->setVisible(true);
        ui->button_projetos->setVisible(true);
        ui->button_solved->setVisible(true);
        ui->button_tickets_equipe->setVisible(true);
        ui->button_solved_equipe->setVisible(true);
        ui->button_novo->setVisible(true);
        ui->label_19->setVisible(true);
        ui->combo_desenvolvedoredit->setVisible(true);
        ui->button_meusprojetos->setVisible(true);
        ui->button_gerenciador_projetos->setVisible(true);
        ui->button_regprojeto->setVisible(true);
    }else if(usuario_conectado.getCargo() == "Admin" && usuario_conectado.getEmpresa() == ""){
        ui->stacked_gerenciador->setCurrentIndex(8);
    }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." && usuario_conectado.getEmpresa() != ""){
        ui->button_tickets->setVisible(true);
        ui->button_projetos->setVisible(true);
        ui->button_solved->setVisible(true);
        ui->button_meusprojetos->setVisible(true);
    }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." && usuario_conectado.getEmpresa() == "" && usuario_conectado.getStatus() == "Pendente"){
        QMessageBox::information(this, "Pedido em análise", "Seu pedido continua em análise, tente entrar em contato com seu manager para agilizar o processo.\nCaso prefire cancelar o pedido e realizar outro, acesse o botão 'Configurações'");
        ui->button_tickets->setVisible(true);
        ui->button_projetos->setVisible(true);
        ui->button_solved->setVisible(true);
        ui->button_meusprojetos->setVisible(true);
    }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." && usuario_conectado.getEmpresa() == ""){
        ui->stacked_gerenciador->setCurrentIndex(9);
    }else if(usuario_conectado.getCargo() == "Desenvolvedor Sr." && usuario_conectado.getEmpresa() != ""){
        ui->button_tickets->setVisible(true);
        ui->button_projetos->setVisible(true);
        ui->button_solved->setVisible(true);
        ui->button_meusprojetos->setVisible(true);
    }else if(usuario_conectado.getCargo() == "Desenvolvedor Sr." && usuario_conectado.getEmpresa() == "" && usuario_conectado.getStatus() == "Pendente"){
        QMessageBox::information(this, "Pedido em análise", "Seu pedido continua em análise, tente entrar em contato com seu manager para agilizar o processo.\nCaso prefire cancelar o pedido e realizar outro, acesse o botão 'Configurações'");
        ui->button_tickets->setVisible(true);
        ui->button_projetos->setVisible(true);
        ui->button_solved->setVisible(true);
        ui->button_meusprojetos->setVisible(true);
    }else if(usuario_conectado.getCargo() == "Desenvolvedor Sr." && usuario_conectado.getEmpresa() == ""){
        ui->stacked_gerenciador->setCurrentIndex(9);
    }

    this->busca_evento();
}

void Gerenciador::atualizaTemporario(QString nome){
    QSqlQuery query;
    query.prepare("select * from registro_programadores where nome_programador='"+nome+"'");
    if(query.exec()){
        int count = 0;
        while(query.next())
            count++;

        if(count == 1){
            query.first();
            temporario_designado.setNome(query.value(2).toString());
            temporario_designado.setCargo(query.value(4).toString());
            temporario_designado.setId(query.value(0).toString());
        }
    }
}

void Gerenciador::atualizaContagem(){
    QSqlQuery query_bugs;
    if(usuario_conectado.getCargo() == "Admin"){
        //query para encontrar todos os bugs que estao relacionados com o programador logado
        query_bugs.prepare("select * from registro_bugs where empresa='"+usuario_conectado.getEmpresa()+"'");

        if(query_bugs.exec()){
            while(query_bugs.next()){
                //irá atualizar a contagem para o tanto de tickets que se encontram "PENDENTES"
                if(query_bugs.value(5).toString() == "Baixa" && query_bugs.value(6).toString() == "Aberto")
                    count_baixo++;
                else if(query_bugs.value(5).toString() == "Media" && query_bugs.value(6).toString() == "Aberto")
                    count_medio++;
                else if(query_bugs.value(5).toString() == "Alta" && query_bugs.value(6).toString() == "Aberto")
                    count_alto++;
            }
        }
        //atualiza o resumo de tickets
        ui->baixa_prioridade->setText(QString::number(count_baixo));
        ui->media_prioridade->setText(QString::number(count_medio));
        ui->alta_prioridade->setText(QString::number(count_alto));
    }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." || usuario_conectado.getCargo() == "Desenvolvedor Sr."){
        //query para encontrar todos os bugs que estao relacionados com o programador logado
        query_bugs.prepare("SELECT * FROM registro_bugs WHERE id_programador='"+usuario_conectado.getId()+"' AND empresa='"+usuario_conectado.getEmpresa()+"'");

        if(query_bugs.exec()){
            while(query_bugs.next()){
                //irá atualizar a contagem para o tanto de tickets que se encontram "PENDENTES"
                if(query_bugs.value(5).toString() == "Baixa" && query_bugs.value(6).toString() == "Aberto")
                    count_baixo++;
                else if(query_bugs.value(5).toString() == "Media" && query_bugs.value(6).toString() == "Aberto")
                    count_medio++;
                else if(query_bugs.value(5).toString() == "Alta" && query_bugs.value(6).toString() == "Aberto")
                    count_alto++;
            }
        }
        //atualiza o resumo de tickets
        ui->baixa_prioridade->setText(QString::number(count_baixo));
        ui->media_prioridade->setText(QString::number(count_medio));
        ui->alta_prioridade->setText(QString::number(count_alto));
    }else if(usuario_conectado.getCargo() == "Manager"){
        query_bugs.prepare("SELECT * FROM registro_bugs WHERE nome_projeto in (SELECT nome_projeto FROM registro_projetos WHERE id_manager='"+usuario_conectado.getId()+"')");

        if(query_bugs.exec()){
            while(query_bugs.next()){
                if(query_bugs.value(5).toString() == "Baixa" && query_bugs.value(6).toString() == "Aberto")
                    count_baixo++;
                else if(query_bugs.value(5).toString() == "Media" && query_bugs.value(6).toString() == "Aberto")
                    count_medio++;
                else if(query_bugs.value(5).toString() == "Alta" && query_bugs.value(6).toString() == "Aberto")
                    count_alto++;
            }
        }

        //para managers será necessario outra query para adicionar os tickets pessoas do desenvolvedor com cargo de manager
        QSqlQuery tickets_pessoais;
        tickets_pessoais.prepare("SELECT * FROM registro_bugs WHERE id_programador='"+usuario_conectado.getId()+"'");
        if(tickets_pessoais.exec()){
            while(tickets_pessoais.next()){
                if(tickets_pessoais.value(5).toString() == "Baixa" && tickets_pessoais.value(6).toString() == "Aberto")
                    count_baixo++;
                else if(tickets_pessoais.value(5).toString() == "Media" && tickets_pessoais.value(6).toString() == "Aberto")
                    count_medio++;
                else if(tickets_pessoais.value(5).toString() == "Alta" && tickets_pessoais.value(6).toString() == "Aberto")
                    count_alto++;
            }
        }

        //atualiza o resumo de tickets
        ui->baixa_prioridade->setText(QString::number(count_baixo));
        ui->media_prioridade->setText(QString::number(count_medio));
        ui->alta_prioridade->setText(QString::number(count_alto));
    }
}

void Gerenciador::atualiza_temporizador(){
    tempo_query->setHMS(0, 0, tempo_query->addSecs(-1).second());

    QString texto_tempo = tempo_query->toString("mm:ss");

    QTime limite(0, 0, 0);
    QString tempo_limite = limite.toString("mm:ss");

    if(texto_tempo == tempo_limite){
        this->busca_evento();
        this->iniciaTimer();

        if(this->checar_conexao())
            return;
        else{
            QMessageBox::information(this, "Sem conexão", "Você não possui conexão com a internet, tente se conectar novamente");
            on_button_desconectar_clicked();
        }
    }
}


void Gerenciador::iniciaTimer(){
    tempo_query = new QTime(0,0,30);
}

void Gerenciador::busca_evento(){
    QVector<int> id_eventos;

    QSqlQuery buscar_eventos;
    buscar_eventos.prepare("SELECT * FROM registro_eventos WHERE id_programador='"+usuario_conectado.getId()+"'");
    if(buscar_eventos.exec()){
        //cria as mensagens dos eventos para o usuário
        while(buscar_eventos.next()){
            QMessageBox::information(this, buscar_eventos.value(2).toString(), buscar_eventos.value(3).toString());
            id_eventos.push_back(buscar_eventos.value(0).toInt());
            this->atualizaTickets();
        }

        //deleta as mensagens que foram enviadas
        for (auto pos:id_eventos){
            QSqlQuery deleta_evento;
            deleta_evento.prepare("DELETE FROM registro_eventos WHERE id_evento='"+QString::number(pos)+"'");
            deleta_evento.exec();
        }
    }
}

void Gerenciador::resetaContagem(){
    this->count_alto = 0;
    this->count_baixo = 0;
    this->count_medio = 0;
}

bool Gerenciador::checar_conexao(){
    bool conexao_ativa;

    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply* reply = nam.get(req);
    QEventLoop loop;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->bytesAvailable())
        conexao_ativa = true;
    else
        conexao_ativa = false;

    return conexao_ativa;
}

void Gerenciador::atualizaTickets(){
//    ui->lista_ultimasreq->setRowCount(0);
//    ui->lista_ultimasreq->setColumnCount(0);
    if(usuario_conectado.getCargo() == "Admin"){
        QSqlQuery admin_query;
        admin_query.prepare("SELECT * FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"' AND status='Aberto' ORDER BY id_bug DESC");
        if(admin_query.exec()){
            int count = 0;
            int qntd = 10;

            ui->lista_ultimasreq->setRowCount(0);
            ui->lista_ultimasreq->setColumnCount(3);

            while(admin_query.next() && qntd != 0){
                ui->lista_ultimasreq->insertRow(count);
                ui->lista_ultimasreq->setItem(count, 0, new QTableWidgetItem(admin_query.value(8).toString()));
                ui->lista_ultimasreq->setItem(count, 1, new QTableWidgetItem(admin_query.value(1).toString()));
                ui->lista_ultimasreq->setItem(count, 2, new QTableWidgetItem(admin_query.value(9).toString()));
                ui->lista_ultimasreq->setRowHeight(count, 25);
                count++;
                qntd--;
            }

            //tamanho das colunas
            ui->lista_ultimasreq->setColumnWidth(0, 200);
            ui->lista_ultimasreq->setColumnWidth(1, 200);
            ui->lista_ultimasreq->setColumnWidth(2, 550);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket"};
            ui->lista_ultimasreq->setHorizontalHeaderLabels(cabecalho);
            ui->lista_ultimasreq->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->lista_ultimasreq->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->lista_ultimasreq->verticalHeader()->setVisible(false);
            ui->lista_ultimasreq->setStyleSheet("QTableView {selection-background-color: blue;}");
        }
    }else if(usuario_conectado.getCargo() == "Manager"){
        QVector<int>id_bugs_manager;
        QSqlQuery procurar_tickets_equipe;
        procurar_tickets_equipe.prepare("SELECT * FROM registro_bugs WHERE nome_projeto in (SELECT nome_projeto FROM registro_projetos WHERE id_manager='"+usuario_conectado.getId()+"') AND status='Aberto'");
        if(procurar_tickets_equipe.exec()){
            while(procurar_tickets_equipe.next())
                id_bugs_manager.push_back(procurar_tickets_equipe.value(0).toInt());
        }

        QSqlQuery procurar_tickets_pessoais;
        procurar_tickets_pessoais.prepare("SELECT * FROM registro_bugs WHERE id_programador='"+usuario_conectado.getId()+"' AND status='Aberto'");
        if(procurar_tickets_pessoais.exec()){
            while(procurar_tickets_pessoais.next())
                id_bugs_manager.push_back(procurar_tickets_pessoais.value(0).toInt());
        }

        //sort vector
        std::sort(id_bugs_manager.begin(), id_bugs_manager.end(), std::greater<int>());

        for(auto bug: id_bugs_manager)
            qDebug() << bug;

        QSqlQuery manager_query;

        //configurações de layout do QTableWidget
        ui->lista_ultimasreq->setRowCount(0);
        ui->lista_ultimasreq->setColumnCount(3);

        ui->lista_ultimasreq->setColumnWidth(0, 200);
        ui->lista_ultimasreq->setColumnWidth(1, 200);
        ui->lista_ultimasreq->setColumnWidth(2, 550);

        //formatação de nomes,triggers e css
        QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket"};
        ui->lista_ultimasreq->setHorizontalHeaderLabels(cabecalho);
        ui->lista_ultimasreq->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->lista_ultimasreq->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->lista_ultimasreq->verticalHeader()->setVisible(false);
        ui->lista_ultimasreq->setStyleSheet("QTableView {selection-background-color: blue;}");

        //encontra todos os bugs para o manager (pessoais e da equipe que ele gerencia)
        for(int i = 0; i < id_bugs_manager.size(); i++){
            manager_query.prepare("SELECT * FROM registro_bugs WHERE id_bug='"+QString::number(id_bugs_manager[i])+"'");
            if(manager_query.exec()){
                manager_query.first();
                ui->lista_ultimasreq->insertRow(i);
                ui->lista_ultimasreq->setItem(i, 0, new QTableWidgetItem(manager_query.value(8).toString()));
                ui->lista_ultimasreq->setItem(i, 1, new QTableWidgetItem(manager_query.value(1).toString()));
                ui->lista_ultimasreq->setItem(i, 2, new QTableWidgetItem(manager_query.value(9).toString()));
                ui->lista_ultimasreq->setRowHeight(i, 25);
            }
        }
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM registro_bugs WHERE id_programador='"+usuario_conectado.getId()+"'"
                     " AND empresa='"+usuario_conectado.getEmpresa()+"' AND status='Aberto' ORDER BY id_bug DESC");
        if(query.exec()){
            int count = 0;
            int qntd = 10;

            ui->lista_ultimasreq->setRowCount(0);
            ui->lista_ultimasreq->setColumnCount(3);

            while(query.next() && qntd != 0){
                ui->lista_ultimasreq->insertRow(count);
                ui->lista_ultimasreq->setItem(count, 0, new QTableWidgetItem(query.value(8).toString()));
                ui->lista_ultimasreq->setItem(count, 1, new QTableWidgetItem(query.value(1).toString()));
                ui->lista_ultimasreq->setItem(count, 2, new QTableWidgetItem(query.value(9).toString()));
                ui->lista_ultimasreq->setRowHeight(count, 25);
                count++;
                qntd--;
            }

            //tamanho das colunas
            ui->lista_ultimasreq->setColumnWidth(0, 200);
            ui->lista_ultimasreq->setColumnWidth(1, 200);
            ui->lista_ultimasreq->setColumnWidth(2, 550);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket"};
            ui->lista_ultimasreq->setHorizontalHeaderLabels(cabecalho);
            ui->lista_ultimasreq->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->lista_ultimasreq->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->lista_ultimasreq->verticalHeader()->setVisible(false);
            ui->lista_ultimasreq->setStyleSheet("QTableView {selection-background-color: blue;}");
        }
    }
}

void Gerenciador::on_button_desconectar_clicked()
{
    this->resetaContagem();
    usuario_conectado.limpaCache();
    temporario_designado.limpaCache();
    ui->button_novo->setVisible(false);
    ui->combo_projeto->clear();
    ui->combo_desenvolvedor->clear();
    ui->stacked_gerenciador->setCurrentIndex(0);
    ui->label_19->setVisible(false);
    ui->button_meusprojetos->setVisible(false);
    ui->button_tickets->setVisible(false);
    ui->button_solved->setVisible(false);
    ui->button_tickets_equipe->setVisible(false);
    ui->button_solved_equipe->setVisible(false);
    ui->button_projetos->setVisible(false);
    ui->combo_desenvolvedoredit->setVisible(false);
    ui->button_gerenciador_projetos->setVisible(false);
    ui->button_regprojeto->setVisible(false);
    ui->button_funcionarios->setVisible(false);
    ui->button_geraplic->setVisible(false);
    ui->button_dadosempresa->setVisible(true);
    ui->combo_desenvolvedoredit->clear();
    ui->lista_tickets->clear();
    ui->button_proj_modmanager->setVisible(false);
    ui->lista_ultimasreq->clear();
    emit home_buttonClicked();
}

void Gerenciador::on_button_enviatkt_clicked()
{
    QDateTime dia_hora = QDateTime::currentDateTime();
    QString texto_diah = dia_hora.toString("dd/MM/yyyy hh:mm:ss");

    QSqlQuery query;
    query.prepare("insert into registro_bugs(nome_projeto, nome_emissor_ticket,id_programador, nome_designado_ticket, prioridade, "
                    "status, descricao_bug, data_hora, titulo, arquivo, nome_arquivo, empresa) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    //adiciona valores para query
    query.addBindValue(ui->combo_projeto->currentText());
    query.addBindValue(usuario_conectado.getNome());
    query.addBindValue(temporario_designado.getId());
    query.addBindValue(ui->combo_desenvolvedor->currentText());
    query.addBindValue(ui->combo_prioridade->currentText());
    query.addBindValue("Aberto");
    query.addBindValue(ui->textEdit->toPlainText());
    query.addBindValue(texto_diah);
    query.addBindValue(ui->line_titulo->text());
    query.addBindValue(dados);
    query.addBindValue(nomeArquivo);
    query.addBindValue(usuario_conectado.getEmpresa());

    //determina data/hora e titulo para o bug
    _bug.setData_hora(texto_diah);
    _bug.setTitulo(ui->line_titulo->text());

    //adiciona ao DB o novo ticket e limpa dados antigos
    if(ui->line_filename->text() == "" && arquivo.fileName()== ""){
        QMessageBox::StandardButton resposta;
        resposta = QMessageBox::information(this, "Arquivos não anexados", "Você está enviando um ticket sem um arquivo de anexo"
                                 " deseja prosseguir?", QMessageBox::Yes | QMessageBox::No);
        if(resposta == QMessageBox::Yes){
            if(query.exec()){
                QMessageBox::information(this, "Ticket Aberto", "Novo ticket cadastrado com sucesso");

                //criar evento
                QSqlQuery criar_evento;
                criar_evento.prepare("INSERT INTO registro_eventos (id_programador, titulo_evento, mensagem_evento) values(?, ?, ?)");
                criar_evento.addBindValue(temporario_designado.getId());
                criar_evento.addBindValue("Novo ticket registrado");
                criar_evento.addBindValue("Um novo ticket foi registrado para você.\nConfira a página meus tickets para informações mais detalhadas.");
                criar_evento.exec();

                //limpa cache de dados obsoletos
                ui->combo_desenvolvedor->setCurrentIndex(0);
                ui->combo_projeto->setCurrentIndex(0);
                ui->combo_prioridade->setCurrentIndex(0);
                ui->textEdit->clear();
                ui->line_titulo->clear();
                ui->line_filename->clear();
                nomeArquivo = "";
                dados.clear();
            }
        }else{
            return;
        }
    }else{
        if(query.exec()){
            QMessageBox::information(this, "Ticket Aberto", "Novo ticket cadastrado com sucesso");

            //criar evento
            QSqlQuery criar_evento;
            criar_evento.prepare("INSERT INTO registro_eventos (id_programador, titulo_evento, mensagem_evento) values(?, ?, ?)");
            criar_evento.addBindValue(temporario_designado.getId());
            criar_evento.addBindValue("Novo ticket registrado");
            criar_evento.addBindValue("Um novo ticket foi registrado para você.\nConfira a página meus tickets para informações mais detalhadas.");
            criar_evento.exec();

            //limpa cache de dados obsoletos
            ui->combo_desenvolvedor->setCurrentIndex(0);
            ui->combo_projeto->setCurrentIndex(0);
            ui->combo_prioridade->setCurrentIndex(0);
            ui->textEdit->clear();
            ui->line_titulo->clear();
            ui->line_filename->clear();
            nomeArquivo = "";
            dados.clear();
        }
    }
    ui->combo_desenvolvedor->clear();
    ui->combo_projeto->clear();
}

//void Gerenciador::registros_ProgBug(){
//    QSqlQuery busca_bug;
//    busca_bug.prepare("SELECT * FROM registro_bugs WHERE titulo='"+_bug.getTitulo()+"' AND data_hora='"+_bug.getData_hora()+"' AND empresa='"+usuario_conectado.getEmpresa()+"'");
//    if(busca_bug.exec()){
//        int count = 0;
//        while(busca_bug.next())
//            count++;

//        if(count == 1){
//            busca_bug.first();
//            QSqlQuery query;
//            query.prepare("insert into programadores_bugs(id_programador, id_bug) values(?, ?)");
//            query.addBindValue(temporario_designado.getId());
//            _bug.setBug_id(busca_bug.value(0).toString());
//            query.addBindValue(_bug.getBug_id());

//            query.exec();
//        }
//    }
//    temporario_designado.limpaCache();
//}

//void Gerenciador::registros_ProjBug(QString nome_do_projeto){
//    QSqlQuery busca_projeto;
//    busca_projeto.prepare("SELECT * FROM registro_projetos WHERE nome_projeto='"+nome_do_projeto+"' AND empresa='"+usuario_conectado.getEmpresa()+"'");
//    if(busca_projeto.exec()){
//        int count = 0;
//        while(busca_projeto.next())
//            count++;

//        if(count == 1){
//            busca_projeto.first();
//            QSqlQuery query;
//            query.prepare("insert into bugs_projetos(id_projeto, id_bug) values(?, ?)");
//            query.addBindValue(busca_projeto.value(0));
//            query.addBindValue(_bug.getBug_id());

//            query.exec();
//        }
//    }
//    _bug.limpaCache();
//}

void Gerenciador::on_button_addfile_clicked()
{
    QString filtro = "Todos Arquivos (*.*);; Arquivos Cpp (*.cpp *.h *.hpp)";
    QString abrirArquivo = QFileDialog::getOpenFileName(this, "Escolha o arquivo", "C://", filtro);
    arquivo.setFileName(abrirArquivo);

    if(!arquivo.open(QFile::ReadWrite)){
        QMessageBox::warning(this, "Erro", "Erro ao abrir o arquivo selecionado");
    }else{
        dados = arquivo.readAll();
        info.setFile(arquivo);
        nomeArquivo = info.fileName();
        ui->line_filename->setText(abrirArquivo);

        arquivo.close();
    }
}

void Gerenciador::on_button_projetos_clicked()
{
    this->resetaContagem();
    ui->lista_ultimasreq->clear();
    this->atualizaContagem();
    this->atualizaTickets();
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(0);
}

void Gerenciador::on_button_novo_clicked()
{
    //resetar variaveis e trocar index do stakedwidget
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(2);

    QSqlQuery projetos;
    if(usuario_conectado.getCargo() == "Admin"){
        projetos.prepare("SELECT * FROM registro_projetos WHERE empresa='"+usuario_conectado.getEmpresa()+"'");
        if(projetos.exec()){
            while(projetos.next())
                ui->combo_projeto->addItem(projetos.value(1).toString());
        }
    }else if(usuario_conectado.getCargo() == "Manager"){
        projetos.prepare("SELECT * FROM registro_projetos WHERE empresa='"+usuario_conectado.getEmpresa()+"' AND id_manager='"+usuario_conectado.getId()+"'");
        if(projetos.exec()){
            while(projetos.next())
                ui->combo_projeto->addItem(projetos.value(1).toString());
        }
    }
}


void Gerenciador::on_combo_projeto_currentTextChanged(const QString &arg1)
{
    ui->combo_desenvolvedor->clear();
    ids_programadores.clear();
    QSqlQuery projeto;
    projeto.prepare("SELECT * FROM registro_projetos WHERE nome_projeto='"+arg1+"' AND empresa='"+usuario_conectado.getEmpresa()+"'");
    if(projeto.exec()){
        int count = 0;
        while(projeto.next())
            count++;

        if(count == 1){
            projeto.first();
            id = projeto.value(0).toInt();
            QSqlQuery desenvolvedores;
            desenvolvedores.prepare("SELECT * FROM programadores_projetos WHERE id_projeto='"+QString::number(id)+"'");
            if(desenvolvedores.exec()){
                //encontra a relacao de desenvolvedores no projeto selecionado
                while(desenvolvedores.next()){
                    ids_programadores.push_back(desenvolvedores.value(0).toInt());
                }
            }
        }

        //encontra o desenvolvedor e adiciona o nome ao combobox
        QSqlQuery busca;
        for(int i = 0; i < ids_programadores.size(); i++){
            busca.prepare("select * from registro_programadores where "
                          "id_programador='"+QString::number(ids_programadores[i])+"' AND empresa='"+usuario_conectado.getEmpresa()+"' AND status_aplicacao='Aceito'");
            if(busca.exec()){
                int n = 0;
                while(busca.next())
                    n++;

                if(n == 1){
                    busca.first();
                    ui->combo_desenvolvedor->addItem(busca.value(2).toString());
                }
            }
        }
    }
}

//sempre que o usuario trocar o nome do desenvolvedor selecionado, atualizar a variavel temporaria
void Gerenciador::on_combo_desenvolvedor_currentTextChanged(const QString &arg1)
{
    this->atualizaTemporario(arg1);
}

void Gerenciador::limpaCache_ticket(){
    ui->label_idticket->clear();
    ui->label_autor->clear();
    ui->label_nomeprojeto->clear();
    ui->label_prioridade->clear();
    ui->label_tituloticket_2->clear();
    ui->text_descricao->clear();
}

void Gerenciador::on_lista_ultimasreq_itemDoubleClicked(QTableWidgetItem *item)
{
    //limpa cache e adiciona os valores para _bug
    _bug.limpaCache();
    this->limpaCache_ticket();
    item = ui->lista_ultimasreq->item(ui->lista_ultimasreq->currentRow(), 0);
    _bug.setData_hora(item->data(Qt::DisplayRole).toString());
    item = ui->lista_ultimasreq->item(ui->lista_ultimasreq->currentRow(), 2);
    _bug.setTitulo(item->data(Qt::DisplayRole).toString());

    if(usuario_conectado.getCargo() == "Admin" || usuario_conectado.getCargo() == "Manager")
        ui->combo_prioridadeedit->setEnabled(true);
    else
        ui->combo_prioridadeedit->setEnabled(false);

    QSqlQuery abrir_ticket;
    abrir_ticket.prepare("select * from registro_bugs where titulo='"+_bug.getTitulo()+"' and data_hora='"+_bug.getData_hora()+"'");
    if(abrir_ticket.exec()){
        int count = 0;
        while(abrir_ticket.next())
            count++;

        if(count == 1){
            abrir_ticket.first();

            //inserir valores nas labels posicionadas
            ui->label_idticket->setText(abrir_ticket.value(0).toString());
            ui->label_autor->setText(abrir_ticket.value(2).toString());
            ui->label_nomeprojeto->setText(abrir_ticket.value(1).toString());
            ui->label_prioridade->setText(abrir_ticket.value(5).toString());
            ui->label_tituloticket_2->setText(abrir_ticket.value(9).toString());
            ui->text_descricao->setText(abrir_ticket.value(7).toString());
            ui->label_status->setText(abrir_ticket.value(6).toString());
            ui->label_nomedesignadoedit->setText(abrir_ticket.value(4).toString());
            ui->stacked_gerenciador->setCurrentIndex(3);

            //se não existirem arquivos anexados, não mostrar botão!
            if(abrir_ticket.value(11) == "")
                ui->button_download->setHidden(true);

            //procurar por comentários no ticket
            QSqlQuery busca_comentario;
            busca_comentario.prepare("SELECT * FROM registro_comentarios WHERE id_bug='"+ui->label_idticket->text()+"' ORDER BY id_comentario DESC");
            if(busca_comentario.exec()){
                int count = 0;
                ui->table_comentarios->setColumnCount(4);
                ui->table_comentarios->setRowCount(0);
                while(busca_comentario.next()){
                    ui->table_comentarios->insertRow(count);
                    ui->table_comentarios->setItem(count, 0, new QTableWidgetItem(busca_comentario.value(4).toString()));
                    ui->table_comentarios->setItem(count, 1, new QTableWidgetItem(busca_comentario.value(2).toString()));
                    ui->table_comentarios->setItem(count, 2, new QTableWidgetItem(busca_comentario.value(3).toString()));
                    ui->table_comentarios->setItem(count, 3, new QTableWidgetItem(busca_comentario.value(0).toString()));
                    ui->table_comentarios->setRowHeight(count, 25);
                    count++;
                }

                //layout da tableWidget
                item = ui->lista_ultimasreq->item(ui->lista_ultimasreq->currentRow(), 0);
                QString layout_teste = item->data(Qt::DisplayRole).toString();
                if(layout_teste == ""){
                    ui->table_comentarios->setColumnWidth(0, 300);
                    ui->table_comentarios->setColumnWidth(1, 300);
                    ui->table_comentarios->setColumnWidth(2, 300);
                }else{
                    ui->table_comentarios->resizeColumnToContents(2);
                    ui->table_comentarios->resizeColumnToContents(0);
                    ui->table_comentarios->resizeColumnToContents(1);
                }

                //formatação de nomes,triggers e css
                QStringList cabecalho = {"Data/Hora", "Autor", "Comentário"};
                ui->table_comentarios->setHorizontalHeaderLabels(cabecalho);
                ui->table_comentarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->table_comentarios->setSelectionBehavior(QAbstractItemView::SelectRows);
                ui->table_comentarios->verticalHeader()->setVisible(false);
                ui->table_comentarios->setColumnHidden(3, true);
                ui->table_comentarios->setStyleSheet("QTableView {selection-background-color: grey;}");

            }
            this->refresh_modificacoes();
        }
    }
}

void Gerenciador::refresh_comentarios(){
    QSqlQuery busca_comentario;
    busca_comentario.prepare("select * from registro_comentarios where id_bug='"+ui->label_idticket->text()+"'");

    if(busca_comentario.exec()){
        int count = 0;
        ui->table_comentarios->setColumnCount(3);
        ui->table_comentarios->setRowCount(0);

        while(busca_comentario.next()){
            ui->table_comentarios->insertRow(count);
            ui->table_comentarios->setItem(count, 0, new QTableWidgetItem(busca_comentario.value(4).toString()));
            ui->table_comentarios->setItem(count, 1, new QTableWidgetItem(busca_comentario.value(2).toString()));
            ui->table_comentarios->setItem(count, 2, new QTableWidgetItem(busca_comentario.value(3).toString()));
            ui->table_comentarios->setRowHeight(count, 25);
            count++;
        }

        //layout da tableWidget
        ui->table_comentarios->setColumnWidth(0, 150);
        ui->table_comentarios->setColumnWidth(1, 250);
        ui->table_comentarios->setColumnWidth(2, 400);


        //formatação de nomes,triggers e css
        QStringList cabecalho = {"Data", "Autor do Comentário", "Comentário"};
        ui->table_comentarios->setHorizontalHeaderLabels(cabecalho);
        ui->table_comentarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->table_comentarios->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_comentarios->verticalHeader()->setVisible(false);
        ui->table_comentarios->setStyleSheet("QTableView {selection-background-color: grey;}");

    }
}

void Gerenciador::on_button_download_clicked()
{
    QSqlQuery query;
    query.prepare("select * from registro_bugs where id_bug='"+ui->label_idticket->text()+"'");
    if(query.exec()){
        int count = 0;
        while(query.next())
            count++;

        //se encontrar o resultado, criar o arquivo
        if(count == 1){
            query.first();
            nomeArquivo = query.value(11).toString();
            QString filtro = "Todos Arquivos (*.*) ;; Arquivos C++(*.cpp *.h *.hpp)";
            QFileDialog savefile;
            QString abrirArquivo = savefile.getSaveFileName(this, "Escolha onde salvar o arquivo", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + nomeArquivo, filtro);
            arquivo.setFileName(abrirArquivo);

            if(!arquivo.open(QIODevice::WriteOnly)){
                QMessageBox::information(this, "Arquivo não salvo", "Não foi possível salvar o arquivo, tente novamente.");
            }else{
                QByteArray data = query.value(10).toByteArray();
                arquivo.write(data);
                arquivo.close();
            }

        }
    }
}

void Gerenciador::on_button_addcoment_clicked()
{
    aba_comentario.show();
}

void Gerenciador::envio_de_comentario(){
    QSqlQuery encontra_bug;
    encontra_bug.prepare("select * from registro_bugs where id_bug='"+ui->label_idticket->text()+"'");

    if(encontra_bug.exec()){
        int count = 0;

        while(encontra_bug.next())
            count++;

        if(count == 1){
            encontra_bug.first();
            QDateTime timer = QDateTime::currentDateTime();
            QString data_hora = timer.toString("dd/MM/yyyy hh:mm:ss");
            QSqlQuery comentario;
            comentario.prepare("insert into registro_comentarios (id_bug, autor_comentario, comentario, data_hora) values (?, ?, ?, ?)");
            comentario.addBindValue(encontra_bug.value(0).toString());
            comentario.addBindValue(usuario_conectado.getNome());
            comentario.addBindValue(aba_comentario.getTexto());
            comentario.addBindValue(data_hora);

            comentario.exec();
        }
    }
    this->refresh_comentarios();
}

void Gerenciador::esconde_buttons(){
    ui->button_tickets->setVisible(false);
    ui->button_meusprojetos->setVisible(false);
    ui->button_tickets_equipe->setVisible(false);
    ui->button_solved_equipe->setVisible(false);
    ui->button_novo->setVisible(false);
    ui->button_solved->setVisible(false);
    ui->button_projetos->setVisible(false);
    ui->button_regprojeto->setVisible(false);
    ui->button_gerenciador_projetos->setVisible(false);
    ui->button_funcionarios->setVisible(false);
    ui->button_geraplic->setVisible(false);
}

void Gerenciador::envio_de_modificacoes(){
    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(&configuracoes, "Escolha uma opção", "Lembre-se de que se você for Admin de uma empresa e escolheu modificar \nseu cargo, sua empresa será removida do nosso banco de dados\njuntamente com todos os tickets/projetos criados.\nDeseja enviar as modificações realizadas?", QMessageBox::Yes | QMessageBox::No);
    if(botao == QMessageBox::Yes){
        if(configuracoes.getEmail_mod() != "" && configuracoes.getCargo_mod() != usuario_conectado.getCargo()){

            if(this->checa_emailExistente(configuracoes.getEmail_mod()))
                QMessageBox::information(this, "E-mail já registrado", "O e-mail digitado já está sendo utilizado");
            else{
                if(usuario_conectado.getCargo() == "Admin"){
                    QSqlQuery deletar_dados;
                    deletar_dados.exec("DELETE FROM registro_empresas WHERE id='"+usuario_conectado.getEmpresa()+"'");
                    deletar_dados.exec("DELETE FROM registro_comentarios WHERE id_bug in (SELECT id_bug FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"')");
                    deletar_dados.exec("DELETE FROM registro_modificacoes_tickets WHERE id_bug in (SELECT id_bug FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"')");
                    deletar_dados.exec("DELETE FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"'");
                    deletar_dados.exec("DELETE FROM programadores_projetos WHERE id_projeto in(SELECT id_projeto FROM registro_projetos WHERE empresa='"+usuario_conectado.getEmpresa()+"')");
                    deletar_dados.exec("DELETE FROM registro_projetos WHERE empresa='"+usuario_conectado.getEmpresa()+"'");

                    QVector<QString>ids_evento;
                    QSqlQuery encontra_devs;
                    encontra_devs.prepare("SELECT * FROM registro_programadores WHERE empresa='"+usuario_conectado.getEmpresa()+"' AND permissao!='Admin'");
                    if(encontra_devs.exec()){
                        while(encontra_devs.next())
                            ids_evento.push_back(encontra_devs.value(0).toString());
                    }

                    QSqlQuery cria_evento;
                    for(int i = 0; i < ids_evento.size(); i++){
                        cria_evento.prepare("INSERT INTO registro_eventos (id_programador,titulo_evento,mensagem_evento) values(?, ?, ?)");
                        cria_evento.addBindValue(ids_evento[i]);
                        cria_evento.addBindValue("Empresa não existe");
                        cria_evento.addBindValue("A empresa que você está registrado não existe mais em nosso banco de dados, acesse o botão 'Configurações de Conta' para realizar novas aplicações");
                        cria_evento.exec();
                    }

                    QSqlQuery remove_empresa;
                    remove_empresa.prepare("UPDATE registro_programadores SET empresa=NULL, status_aplicacao=NULL WHERE empresa='"+usuario_conectado.getEmpresa()+"'");
                    remove_empresa.exec();
                }
                QSqlQuery enviar_modificacoes;
                enviar_modificacoes.prepare("UPDATE registro_programadores SET email='"+configuracoes.getEmail_mod()+"', permissao='"+configuracoes.getCargo_mod()+"' WHERE id_programador='"+usuario_conectado.getId()+"'");
                if(enviar_modificacoes.exec()){
                    QMessageBox::information(this, "Modificações enviadas", "Suas modificações foram registradas com sucesso.");
                    usuario_conectado.setCargo(configuracoes.getCargo_mod());
                    ui->label_cargo->setText("Logado como: "+usuario_conectado.getCargo());
                    ui->label_dadoscargo->setText(usuario_conectado.getCargo());
                    usuario_conectado.setEmail(configuracoes.getEmail_mod());
                    ui->label_dadosemail->setText(usuario_conectado.getEmail());

                    QSqlQuery atualizar_dados;
                    atualizar_dados.prepare("SELECT * FROM registro_programadores WHERE id_programador='"+usuario_conectado.getId()+"'");
                    if(atualizar_dados.exec()){
                        usuario_conectado.limpaCache();
                        this->atualizaLogado(atualizar_dados);
                    }

                    if(usuario_conectado.getCargo() == "Admin" && usuario_conectado.getEmpresa() == ""){
                        this->esconde_buttons();
                        ui->stacked_gerenciador->setCurrentIndex(8);
                    }else if(usuario_conectado.getCargo() == "Admin" && usuario_conectado.getEmpresa() != ""){
                        QSqlQuery update_admin;
                        update_admin.prepare("UPDATE registro_programadores SET empresa=NULL,status_aplicacao=NULL WHERE id_programador='"+usuario_conectado.getId()+"'");
                        if(update_admin.exec()){
                            this->esconde_buttons();
                            ui->stacked_gerenciador->setCurrentIndex(8);
                        }
                    }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." && usuario_conectado.getEmpresa() == ""){
                        ui->lista_tickets->clear();
                        ui->lista_ultimasreq->clear();
                        ui->table_resolvidos->clear();
                        ui->lista_resolvidos_equipe->clear();
                        ui->lista_tickets_equipe->clear();

                        this->esconde_buttons();

                        ui->stacked_gerenciador->setCurrentIndex(9);
                    }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." && usuario_conectado.getEmpresa() != ""){
                        ui->button_dadoscadastrar->setVisible(false);
                        ui->button_regprojeto->setVisible(false);
                        ui->button_gerenciador_projetos->setVisible(false);
                        ui->button_tickets_equipe->setVisible(false);
                        ui->button_solved_equipe->setVisible(false);
                        ui->button_funcionarios->setVisible(false);
                        ui->button_dadosempresa->setVisible(true);
                        ui->button_geraplic->setVisible(false);
                        ui->button_novo->setVisible(false);

                        ui->stacked_gerenciador->setCurrentIndex(0);
                    }else if(usuario_conectado.getCargo() == "Desenvolvedor Sr." && usuario_conectado.getEmpresa() == ""){
                        ui->lista_tickets->clear();
                        ui->lista_ultimasreq->clear();
                        ui->table_resolvidos->clear();
                        ui->lista_resolvidos_equipe->clear();
                        ui->lista_tickets_equipe->clear();

                        this->esconde_buttons();

                        ui->stacked_gerenciador->setCurrentIndex(9);
                    }else if(usuario_conectado.getCargo() == "Desenvolvedor Sr." && usuario_conectado.getEmpresa() != ""){
                        ui->button_dadoscadastrar->setVisible(false);
                        ui->button_regprojeto->setVisible(false);
                        ui->button_gerenciador_projetos->setVisible(false);
                        ui->button_tickets_equipe->setVisible(false);
                        ui->button_solved_equipe->setVisible(false);
                        ui->button_funcionarios->setVisible(false);
                        ui->button_dadosempresa->setVisible(true);
                        ui->button_geraplic->setVisible(false);
                        ui->button_novo->setVisible(false);

                        ui->stacked_gerenciador->setCurrentIndex(0);
                    }

                    configuracoes.clear();
                }
            }
        }else if(configuracoes.getEmail_mod() == "" && configuracoes.getCargo_mod() != usuario_conectado.getCargo()){
            if(usuario_conectado.getCargo() == "Admin"){
                QSqlQuery deletar_dados;
                deletar_dados.exec("DELETE FROM registro_empresas WHERE id='"+usuario_conectado.getEmpresa()+"'");
                deletar_dados.exec("DELETE FROM registro_comentarios WHERE id_bug in (SELECT id_bug FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"')");
                deletar_dados.exec("DELETE FROM registro_modificacoes_tickets WHERE id_bug in (SELECT id_bug FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"')");
                deletar_dados.exec("DELETE FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"'");
                deletar_dados.exec("DELETE FROM programadores_projetos WHERE id_projeto in(SELECT id_projeto FROM registro_projetos WHERE empresa='"+usuario_conectado.getEmpresa()+"')");
                deletar_dados.exec("DELETE FROM registro_projetos WHERE empresa='"+usuario_conectado.getEmpresa()+"'");

                QVector<QString>ids_evento;
                QSqlQuery encontra_devs;
                encontra_devs.prepare("SELECT * FROM registro_programadores WHERE empresa='"+usuario_conectado.getEmpresa()+"' AND permissao!='Admin'");
                if(encontra_devs.exec()){
                    while(encontra_devs.next())
                        ids_evento.push_back(encontra_devs.value(0).toString());
                }

                QSqlQuery cria_evento;
                for(int i = 0; i < ids_evento.size(); i++){
                    cria_evento.prepare("INSERT INTO registro_eventos (id_programador,titulo_evento,mensagem_evento) values(?, ?, ?)");
                    cria_evento.addBindValue(ids_evento[i]);
                    cria_evento.addBindValue("Empresa não existe");
                    cria_evento.addBindValue("A empresa que você está registrado não existe mais em nosso banco de dados, acesse o botão 'Configurações de Conta' para realizar novas aplicações");
                    cria_evento.exec();
                }

                QSqlQuery remove_empresa;
                remove_empresa.prepare("UPDATE registro_programadores SET empresa=NULL, status_aplicacao=NULL WHERE empresa='"+usuario_conectado.getEmpresa()+"'");
                remove_empresa.exec();
            }
            QSqlQuery enviar_modificacoes;
            enviar_modificacoes.prepare("UPDATE registro_programadores SET permissao='"+configuracoes.getCargo_mod()+"' WHERE id_programador='"+usuario_conectado.getId()+"'");

            if(enviar_modificacoes.exec()){
                QMessageBox::information(this, "Modificações enviadas", "Suas modificações foram registradas com sucesso.");
                usuario_conectado.setCargo(configuracoes.getCargo_mod());
                ui->label_cargo->setText("Logado como: "+usuario_conectado.getCargo());
                ui->label_dadoscargo->setText(usuario_conectado.getCargo());

                QSqlQuery atualizar_dados;
                atualizar_dados.prepare("SELECT * FROM registro_programadores WHERE id_programador='"+usuario_conectado.getId()+"'");
                if(atualizar_dados.exec()){
                    usuario_conectado.limpaCache();
                    this->atualizaLogado(atualizar_dados);
                }

                if(usuario_conectado.getCargo() == "Admin" && usuario_conectado.getEmpresa() == ""){
                    this->esconde_buttons();
                    ui->stacked_gerenciador->setCurrentIndex(8);
                }else if(usuario_conectado.getCargo() == "Admin" && usuario_conectado.getEmpresa() != ""){
                    QSqlQuery update_admin;
                    update_admin.prepare("UPDATE registro_programadores SET empresa=NULL,status_aplicacao=NULL WHERE id_programador='"+usuario_conectado.getId()+"'");
                    if(update_admin.exec()){
                        this->esconde_buttons();
                        ui->stacked_gerenciador->setCurrentIndex(8);
                    }

                }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." && usuario_conectado.getEmpresa() == ""){
                    ui->lista_tickets->clear();
                    ui->lista_ultimasreq->clear();
                    ui->table_resolvidos->clear();
                    ui->lista_resolvidos_equipe->clear();
                    ui->lista_tickets_equipe->clear();

                    this->esconde_buttons();

                    ui->stacked_gerenciador->setCurrentIndex(9);

                }else if(usuario_conectado.getCargo() == "Desenvolvedor Jr." && usuario_conectado.getEmpresa() != ""){
                    ui->button_dadoscadastrar->setVisible(false);
                    ui->button_regprojeto->setVisible(false);
                    ui->button_gerenciador_projetos->setVisible(false);
                    ui->button_tickets_equipe->setVisible(false);
                    ui->button_solved_equipe->setVisible(false);
                    ui->button_funcionarios->setVisible(false);
                    ui->button_dadosempresa->setVisible(true);
                    ui->button_geraplic->setVisible(false);
                    ui->button_novo->setVisible(false);

                    ui->stacked_gerenciador->setCurrentIndex(0);

                }else if(usuario_conectado.getCargo() == "Desenvolvedor Sr." && usuario_conectado.getEmpresa() == ""){
                    ui->lista_tickets->clear();
                    ui->lista_ultimasreq->clear();
                    ui->table_resolvidos->clear();
                    ui->lista_resolvidos_equipe->clear();
                    ui->lista_tickets_equipe->clear();

                    this->esconde_buttons();

                    ui->stacked_gerenciador->setCurrentIndex(9);

                }else if(usuario_conectado.getCargo() == "Desenvolvedor Sr." && usuario_conectado.getEmpresa() != ""){
                    ui->button_dadoscadastrar->setVisible(false);
                    ui->button_regprojeto->setVisible(false);
                    ui->button_gerenciador_projetos->setVisible(false);
                    ui->button_tickets_equipe->setVisible(false);
                    ui->button_solved_equipe->setVisible(false);
                    ui->button_funcionarios->setVisible(false);
                    ui->button_dadosempresa->setVisible(true);
                    ui->button_geraplic->setVisible(false);
                    ui->button_novo->setVisible(false);

                    ui->stacked_gerenciador->setCurrentIndex(0);
                }

                configuracoes.clear();
            }

        }else if(configuracoes.getEmail_mod() != "" && configuracoes.getCargo_mod() == usuario_conectado.getCargo()){

            if(this->checa_emailExistente(configuracoes.getEmail_mod()))
                QMessageBox::information(this, "E-mail já registrado", "O e-mail digitado já está sendo utilizado");
            else{
                QSqlQuery enviar_modificacoes;
                enviar_modificacoes.prepare("UPDATE registro_programadores SET email='"+configuracoes.getEmail_mod()+"' WHERE id_programador='"+usuario_conectado.getId()+"'");

                if(enviar_modificacoes.exec()){
                    QMessageBox::information(this, "Modificações enviadas", "Suas modificações foram registradas com sucesso.");
                    usuario_conectado.setEmail(configuracoes.getEmail_mod());
                    ui->label_dadosemail->setText(usuario_conectado.getEmail());
                    ui->stacked_gerenciador->setCurrentIndex(0);

                    configuracoes.clear();
                }
            }
        }
    }else
        return;
}

void Gerenciador::envio_de_nova_senha(){
    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(this, "Escolha uma opção", "Deseja modificar sua senha atual para a nova senha informada?", QMessageBox::Yes | QMessageBox::No);
    if(botao == QMessageBox::Yes){
        quint32 salt = QRandomGenerator::system()->generate();
        QString salt_senha_pepper = QString::number(salt) + definir_senha.getSenha() + pepper_ger;
        QByteArray hash = QCryptographicHash::hash(salt_senha_pepper.toLocal8Bit(), QCryptographicHash::Md5).toHex();
        QSqlQuery mudar_senha;
        mudar_senha.prepare("UPDATE registro_programadores SET salt='"+QString::number(salt)+"',senha='"+hash+"' WHERE id_programador='"+usuario_conectado.getId()+"'");
        if(mudar_senha.exec()){
            QMessageBox::information(this, "Senha alterada", "Sua senha foi alterada com sucesso.");
            definir_senha.clear();
        }
    }
}

bool Gerenciador::checa_emailExistente(QString email_ex){
    QSqlQuery procura_existente;
    procura_existente.prepare("SELECT * FROM registro_programadores WHERE email='"+email_ex+"'");
    if(procura_existente.exec()){
        int count = 0;

        while(procura_existente.next())
            count++;

        if(count >= 1)
            return true;
    }
    return false;
}

void Gerenciador::on_button_tickets_clicked()
{
    //trocar index do stackedwidget e resetar variaveis
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(1);

    QSqlQuery query;
    query.prepare("SELECT * FROM registro_bugs where id_programador='"+usuario_conectado.getId()+"' "
                      "AND empresa='"+usuario_conectado.getEmpresa()+"' AND status='Aberto' ORDER BY id_bug DESC");
    if(query.exec()){
        int count = 0;

        ui->lista_tickets->setRowCount(0);
        ui->lista_tickets->setColumnCount(3);

        while(query.next()){
            ui->lista_tickets->insertRow(count);
            ui->lista_tickets->setItem(count, 0, new QTableWidgetItem(query.value(8).toString()));
            ui->lista_tickets->setItem(count, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->lista_tickets->setItem(count, 2, new QTableWidgetItem(query.value(9).toString()));
            ui->lista_tickets->setRowHeight(count, 25);
            count++;
        }

        //tamanho das colunas
        ui->lista_tickets->setColumnWidth(0, 200);
        ui->lista_tickets->setColumnWidth(1, 300);
        ui->lista_tickets->setColumnWidth(2, 600);

        //formatação de nomes,triggers e css
        QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket"};
        ui->lista_tickets->setHorizontalHeaderLabels(cabecalho);
        ui->lista_tickets->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->lista_tickets->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->lista_tickets->verticalHeader()->setVisible(false);
        ui->lista_tickets->setStyleSheet("QTableView {selection-background-color: blue;}");
    }
}

void Gerenciador::on_lista_tickets_itemDoubleClicked(QTableWidgetItem *item)
{
    //limpa cache e adiciona os valores para _bug
    _bug.limpaCache();
    this->limpaCache_ticket();
    item = ui->lista_tickets->item(ui->lista_tickets->currentRow(), 0);
    _bug.setData_hora(item->data(Qt::DisplayRole).toString());
    item = ui->lista_tickets->item(ui->lista_tickets->currentRow(), 2);
    _bug.setTitulo(item->data(Qt::DisplayRole).toString());

    if(usuario_conectado.getCargo() == "Admin" || usuario_conectado.getCargo() == "Manager")
        ui->combo_prioridadeedit->setEnabled(true);
    else
        ui->combo_prioridadeedit->setEnabled(false);

    QSqlQuery abrir_ticket;
    abrir_ticket.prepare("select * from registro_bugs where titulo='"+_bug.getTitulo()+"' and "
                             "data_hora='"+_bug.getData_hora()+"'");
    if(abrir_ticket.exec()){
        int count = 0;
        while(abrir_ticket.next())
            count++;

        if(count == 1){
            abrir_ticket.first();

            //inserir valores nas labels posicionadas
            ui->label_idticket->setText(abrir_ticket.value(0).toString());
            ui->label_autor->setText(abrir_ticket.value(2).toString());
            ui->label_nomeprojeto->setText(abrir_ticket.value(1).toString());
            ui->label_nomedesignadoedit->setText(abrir_ticket.value(4).toString());
            ui->label_prioridade->setText(abrir_ticket.value(5).toString());
            ui->label_status->setText(abrir_ticket.value(6).toString());
            ui->label_tituloticket_2->setText(abrir_ticket.value(9).toString());
            ui->text_descricao->setText(abrir_ticket.value(7).toString());
            ui->stacked_gerenciador->setCurrentIndex(3);

            //se não existirem arquivos anexados, não mostrar botão!
            if(abrir_ticket.value(11) == "")
                ui->button_download->setHidden(true);

            //procurar por comentários no ticket
            QSqlQuery busca_comentario;
            busca_comentario.prepare("SELECT * FROM registro_comentarios WHERE "
                                         "id_bug='"+ui->label_idticket->text()+"' ORDER BY id_comentario DESC");
            if(busca_comentario.exec()){
                int count = 0;
                ui->table_comentarios->setColumnCount(4);
                ui->table_comentarios->setRowCount(0);

                //preenche tableWidget lista_tickets
                while(busca_comentario.next()){
                    ui->table_comentarios->insertRow(count);
                    ui->table_comentarios->setItem(count, 0, new QTableWidgetItem(busca_comentario.value(4).toString()));
                    ui->table_comentarios->setItem(count, 1, new QTableWidgetItem(busca_comentario.value(2).toString()));
                    ui->table_comentarios->setItem(count, 2, new QTableWidgetItem(busca_comentario.value(3).toString()));
                    ui->table_comentarios->setItem(count, 3, new QTableWidgetItem(busca_comentario.value(0).toString()));
                    ui->table_comentarios->setRowHeight(count, 25);
                    count++;
                }

                //layout da tableWidget
                ui->table_comentarios->resizeColumnToContents(2);
                ui->table_comentarios->resizeColumnToContents(0);
                ui->table_comentarios->resizeColumnToContents(1);

                //formatação de nomes,triggers e css
                QStringList cabecalho = {"Data/Hora", "Autor", "Comentário"};
                ui->table_comentarios->setHorizontalHeaderLabels(cabecalho);
                ui->table_comentarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->table_comentarios->setSelectionBehavior(QAbstractItemView::SelectRows);
                ui->table_comentarios->verticalHeader()->setVisible(false);
                ui->table_comentarios->setColumnHidden(3, true);
                ui->table_comentarios->setStyleSheet("QTableView {selection-background-color: grey;}");
            }
            this->refresh_modificacoes();
        }
    }
}

void Gerenciador::on_button_solved_clicked()
{
    //resets de valores definidos durante uso hidden/add_item etc..
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(4);

    QSqlQuery query;
    query.prepare("SELECT * FROM registro_bugs WHERE id_programador='"+usuario_conectado.getId()+"' "
                  "AND empresa='"+usuario_conectado.getEmpresa()+"' AND status='Resolvido' ORDER BY data_fechamento DESC");
    if(query.exec()){
        int count = 0;

        ui->table_resolvidos->setRowCount(0);
        ui->table_resolvidos->setColumnCount(4);

        //encontra os ultimos tickets abertos para o usuario logado
        while(query.next()){
            ui->table_resolvidos->insertRow(count);
            ui->table_resolvidos->setItem(count, 0, new QTableWidgetItem(query.value(8).toString()));
            ui->table_resolvidos->setItem(count, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->table_resolvidos->setItem(count, 2, new QTableWidgetItem(query.value(9).toString()));
            ui->table_resolvidos->setItem(count, 3, new QTableWidgetItem(query.value(12).toString()));
            ui->table_resolvidos->setRowHeight(count, 25);
            count++;
        }

        //tamanho das colunas
        ui->table_resolvidos->setColumnWidth(0, 200);
        ui->table_resolvidos->setColumnWidth(1, 200);
        ui->table_resolvidos->setColumnWidth(2, 500);
        ui->table_resolvidos->setColumnWidth(3, 150);

        //formatação de nomes,triggers e css
        QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket", "Data de Fechamento"};
        ui->table_resolvidos->setHorizontalHeaderLabels(cabecalho);
        ui->table_resolvidos->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->table_resolvidos->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_resolvidos->verticalHeader()->setVisible(false);
        ui->table_resolvidos->setStyleSheet("QTableView {selection-background-color: blue;}");
    }

}

void Gerenciador::on_button_editarticket_clicked()
{
    ui->stacked_gerenciador->setCurrentIndex(5);
    ids_programadores.clear();
    ui->combo_desenvolvedoredit->clear();

    /******* VALORES PARA COMBOBOX(DESENVOLVEDORES) *********/
    QSqlQuery projeto;
    projeto.prepare("select * from registro_projetos where nome_projeto='"+ui->label_nomeprojeto->text()+"'");
    if(projeto.exec()){
        int count = 0;
        while(projeto.next())
            count++;
        if(count == 1){
            projeto.first();

            //encontra id do projeto
            QSqlQuery edit_desenvolvedores;
            edit_desenvolvedores.prepare("select * from programadores_projetos where "
                                         "id_projeto='"+projeto.value(0).toString()+"'");
            if(edit_desenvolvedores.exec()){
                //encontra a relacao de desenvolvedores no projeto selecionado
                while(edit_desenvolvedores.next())
                    ids_programadores.push_back(edit_desenvolvedores.value(0).toInt());

            }

        }
    }

    //encontra o desenvolvedor e adiciona o nome ao combobox
    QSqlQuery busca;
    for(int i = 0; i < ids_programadores.size(); i++){
        busca.prepare("select * from registro_programadores where "
                      "id_programador='"+QString::number(ids_programadores[i])+"' AND empresa='"+usuario_conectado.getEmpresa()+"'");
        if(busca.exec()){
            int n = 0;
            while(busca.next())
                n++;

            if(n == 1){
                busca.first();
                ui->combo_desenvolvedoredit->addItem(busca.value(2).toString());
            }
        }
    }

    /******* VALORES PARA LABELS/COMBOBOX(STATUS, PRIORIDADE) *********/
    abrir_ticket_edit.prepare("select * from registro_bugs where titulo='"+_bug.getTitulo()+"' and "
                         "data_hora='"+_bug.getData_hora()+"'");
    if(abrir_ticket_edit.exec()){
        int count = 0;
        while(abrir_ticket_edit.next())
            count++;

        if(count == 1){
            abrir_ticket_edit.first();

            //carrega os valores para o ticket encontrado
            ui->label_tituloedit->setText(abrir_ticket_edit.value(9).toString());
            ui->label_autoredit->setText(abrir_ticket_edit.value(2).toString());
            ui->label_idedit->setText(abrir_ticket_edit.value(0).toString());
            ui->text_descricaoedit->setText(abrir_ticket_edit.value(7).toString());

            //seleciona index de inicialização do comboBox(Status) baseado nos dados mantidos no DB
            if(abrir_ticket_edit.value(6) == "Aberto")
                ui->combo_statusedit->setCurrentIndex(0);
            else if(abrir_ticket_edit.value(6) == "Resolvido")
                ui->combo_statusedit->setCurrentIndex(1);

            //seleciona index de inicialização do comboBox(Prioridade) baseado nos dados mantidos no DB
            if(abrir_ticket_edit.value(5) == "Baixa")
                ui->combo_prioridadeedit->setCurrentIndex(0);
            else if(abrir_ticket_edit.value(5) == "Media")
                ui->combo_prioridadeedit->setCurrentIndex(1);
            else if(abrir_ticket_edit.value(5) == "Alta")
                ui->combo_prioridadeedit->setCurrentIndex(2);

            //seleciona index de inicializacao para comboBox(desenvolvedores) baseado nos registros no DB
            ui->combo_desenvolvedoredit->setCurrentIndex(ui->combo_desenvolvedoredit->
                                                         findText(abrir_ticket_edit.value(4).toString()));
            ui->button_enviaredit->setDisabled(true);
        }
    }
    //pre definicao de valores para controle de query
    var2 = ui->combo_statusedit->currentText();
    var3 = ui->combo_desenvolvedoredit->currentText();
    var1 = ui->combo_prioridadeedit->currentText();
}

void Gerenciador::on_combo_prioridadeedit_currentTextChanged(const QString &arg1)
{
    var1 = arg1;
    if(var1 != abrir_ticket_edit.value(5) || var2 != abrir_ticket_edit.value(6) || var3 != abrir_ticket_edit.value(4))
        ui->button_enviaredit->setDisabled(false);
    else
        ui->button_enviaredit->setDisabled(true);
}


void Gerenciador::on_combo_statusedit_currentTextChanged(const QString &arg1)
{
    var2 = arg1;
    if(var2 != abrir_ticket_edit.value(6) || var1 != abrir_ticket_edit.value(5) || var3 != abrir_ticket_edit.value(4))
        ui->button_enviaredit->setDisabled(false);
    else
        ui->button_enviaredit->setDisabled(true);
}

void Gerenciador::on_combo_desenvolvedoredit_currentTextChanged(const QString &arg1)
{
    var3 = arg1;
    if(var3 != abrir_ticket_edit.value(4) || var1 != abrir_ticket_edit.value(5) || var2 != abrir_ticket_edit.value(6))
        ui->button_enviaredit->setDisabled(false);
    else
        ui->button_enviaredit->setDisabled(true);

}

void Gerenciador::on_button_enviaredit_clicked()
{
    QDateTime data_hora = QDateTime::currentDateTime();
    QString texto_data_hora = data_hora.toString("dd/MM/yyyy hh:mm:ss");
    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(this, "Enviar Modificações", "Tem certeza que deseja registrar as modificações"
                                    "?", QMessageBox::Yes | QMessageBox::No);
    abrir_ticket_edit.first();

    if(botao == QMessageBox::Yes){
        QSqlQuery find_id;
        find_id.prepare("select * from registro_programadores where nome_programador='"+ui->combo_desenvolvedoredit->currentText()+"'");
        int id_prog = 0;
        if(find_id.exec()){
            int count = 0;
            while(find_id.next())
                count++;

            if(count == 1){
                find_id.first();
                id_prog = find_id.value(0).toInt();
            }
        }
        QSqlQuery query;
        //atualizar dados do ticket no DB
        query.prepare("update registro_bugs set prioridade='"+ui->combo_prioridadeedit->currentText()+"', "
                      "status='"+ui->combo_statusedit->currentText()+"', id_programador='"+QString::number(id_prog)+"',"
                      "nome_designado_ticket='"+ui->combo_desenvolvedoredit->currentText()+"' "
                      "where id_bug='"+ui->label_idedit->text()+"'");

        if(query.exec()){
            QMessageBox::information(this, "Modificações realizadas", "Suas modificações foram registradas "
                                     "com sucesso.");
        }

        if(var1 != abrir_ticket_edit.value(5)){
            //enviar modificação para o DB
            QSqlQuery prioridade_query;
            prioridade_query.prepare("insert into registro_modificacoes_tickets (id_bug, valor_modificado, valor_antigo, valor_novo, "
                          "data_modificacao) values (?, ?, ?, ?, ?)");

            prioridade_query.addBindValue(abrir_ticket_edit.value(0));
            prioridade_query.addBindValue("Prioridade");
            prioridade_query.addBindValue(abrir_ticket_edit.value(5));
            prioridade_query.addBindValue(ui->combo_prioridadeedit->currentText());
            data_hora = data_hora.addSecs(1);
            texto_data_hora = data_hora.toString("dd/MM/yyyy hh:mm:ss");
            prioridade_query.addBindValue(texto_data_hora);

            prioridade_query.exec();

            //atualiza pagina
            ui->label_prioridade->setText(var1);
        }

        if(var2 != abrir_ticket_edit.value(6) && var2 == "Resolvido"){
            //enviar modificação para o DB
            //enviando data e hora da solução do ticket
            QSqlQuery data_hora_resolvido;
            data_hora_resolvido.exec("UPDATE registro_bugs SET data_fechamento='"+texto_data_hora+"' WHERE id_bug='"+ui->label_idedit->text()+"'");

            QSqlQuery status_query;
            status_query.prepare("insert into registro_modificacoes_tickets (id_bug, valor_modificado, valor_antigo, valor_novo, "
                          "data_modificacao) values (?, ?, ?, ?, ?)");

            status_query.addBindValue(abrir_ticket_edit.value(0));
            status_query.addBindValue("Status");
            status_query.addBindValue(abrir_ticket_edit.value(6));
            status_query.addBindValue(ui->combo_statusedit->currentText());
            data_hora = data_hora.addSecs(1);
            texto_data_hora = data_hora.toString("dd/MM/yyyy hh:mm:ss");
            status_query.addBindValue(texto_data_hora);

            status_query.exec();

            //atualiza pagina
            ui->label_status->setText(var2);
        }

        if(var2 != abrir_ticket_edit.value(6) && var2 != "Resolvido"){
            //enviar modificação para o DB
            QSqlQuery status_query;
            status_query.prepare("insert into registro_modificacoes_tickets (id_bug, valor_modificado, valor_antigo, valor_novo, "
                                  "data_modificacao) values (?, ?, ?, ?, ?)");

            status_query.addBindValue(abrir_ticket_edit.value(0));
            status_query.addBindValue("Status");
            status_query.addBindValue(abrir_ticket_edit.value(6));
            status_query.addBindValue(ui->combo_statusedit->currentText());
            data_hora = data_hora.addSecs(1);
            texto_data_hora = data_hora.toString("dd/MM/yyyy hh:mm:ss");
            status_query.addBindValue(texto_data_hora);

            status_query.exec();

            //atualiza pagina
            ui->label_status->setText(var2);
        }

        if(var3 != abrir_ticket_edit.value(4)){
            //enviar modificação para o DB
            QSqlQuery desenvolvedor_query;
            desenvolvedor_query.prepare("insert into registro_modificacoes_tickets (id_bug, valor_modificado, valor_antigo, valor_novo, "
                          "data_modificacao) values (?, ?, ?, ?, ?)");

            desenvolvedor_query.addBindValue(abrir_ticket_edit.value(0));
            desenvolvedor_query.addBindValue("DesenvolvedorResponsavel");
            desenvolvedor_query.addBindValue(abrir_ticket_edit.value(4));
            desenvolvedor_query.addBindValue(ui->combo_desenvolvedoredit->currentText());
            data_hora = data_hora.addSecs(1);
            texto_data_hora = data_hora.toString("dd/MM/yyyy hh:mm:ss");
            desenvolvedor_query.addBindValue(texto_data_hora);

            desenvolvedor_query.exec();

            //atualiza pagina
            ui->label_nomedesignadoedit->setText(var3);
        }

        this->refresh_modificacoes();
        ui->stacked_gerenciador->setCurrentIndex(3);
    }else{
        return;
    }
}

void Gerenciador::resetar_variaveis(){
    //resetar valores para evitar bugs
    ui->combo_desenvolvedor->clear();
    ui->combo_projeto->clear();
    ui->button_download->setHidden(false);
    ui->button_enviaredit->setDisabled(true);
    ui->combo_desenvolvedoredit->clear();
    ui->line_addproject->clear();
    ui->text_regproject->clear();
    ui->combo_gerdevs->clear();
    ui->combo_projmanager->clear();
    ui->combo_gerproj->clear();
    nome_desenvolvedores.clear();
}

void Gerenciador::on_button_regprojeto_clicked()
{
    //reset + trocar index do stackedwidget
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(7);
}

void Gerenciador::on_button_enviarreg_clicked()
{
    //criar query e bindar valores
    QSqlQuery checa_existente;
    checa_existente.prepare("SELECT * FROM registro_projetos WHERE nome_projeto='"+ui->line_addproject->text()+"' AND empresa='"+usuario_conectado.getEmpresa()+"'");
    if(checa_existente.exec()){
        int count = 0;

        while(checa_existente.next())
            count++;

        if(count >= 1){
            QMessageBox::information(this, "Erro", "Este nome de projeto já existe.");
            ui->line_addproject->setFocus();
        }else{
            QSqlQuery adicionar_projeto;
            adicionar_projeto.prepare("insert into registro_projetos(nome_projeto, descricao_projeto, id_manager, empresa) "
                                    "values('"+ui->line_addproject->text()+"','"+ui->text_regproject->toPlainText()+"', '"+usuario_conectado.getId()+"', "
                                    "'"+usuario_conectado.getEmpresa()+"')");

            //verificar se o usuario ao menos preencheu algo nos campos
            if(ui->line_addproject->text() != "" && ui->text_regproject->toPlainText() != ""){
                if(adicionar_projeto.exec()){
                    QMessageBox::information(this, "Novo Projeto Registrado", "Seu projeto foi registrado com sucesso!\nAcesse a aba 'Gerenciar Projetos' para configurar adicionar membros à sua equipe");
                    ui->line_addproject->clear();
                    ui->text_regproject->clear();
                    ui->line_addproject->setFocus();

                    QSqlQuery adiciona_registro;
                    adiciona_registro.prepare("INSERT INTO programadores_projetos(id_programador, id_projeto) values(?, ?)");
                    adiciona_registro.addBindValue(usuario_conectado.getId());
                    adiciona_registro.addBindValue(adicionar_projeto.lastInsertId());
                    adiciona_registro.exec();
                }
            }else{
                QMessageBox::information(this, "Preencher Campos", "Favor preencher ambos os campos(nome e descrição de projeto)");
            }
        }
    }

}

void Gerenciador::on_button_gerenciador_projetos_clicked()
{
    //reset + trocar index stackedwidget
    this->resetar_variaveis();
    ids_projetos.clear();
    ids_programadores.clear();
    ui->stacked_gerenciador->setCurrentIndex(6);
    /**************Valores para combo Projetos********************/
    QSqlQuery encontrar_projetos;
    encontrar_projetos.prepare("select * from registro_projetos where id_manager='"+usuario_conectado.getId()+"'");
    if(encontrar_projetos.exec()){
        while(encontrar_projetos.next()){
            ids_projetos.push_back(encontrar_projetos.value(0).toInt());
            ui->combo_gerproj->addItem(encontrar_projetos.value(1).toString());
        }
    }
    /**************Valores para combo Desenvolvedores********************/
    QSqlQuery encontrar_devs;
    encontrar_devs.prepare("SELECT * FROM registro_programadores WHERE empresa='"+usuario_conectado.getEmpresa()+"' AND id_programador!='"+usuario_conectado.getId()+"' AND status_aplicacao='Aceito'");
    if(encontrar_devs.exec()){
        while(encontrar_devs.next()){
            ids_programadores.push_back(encontrar_devs.value(0).toInt());
            ui->combo_gerdevs->addItem(encontrar_devs.value(2).toString());
        }
    }
}

void Gerenciador::on_button_adddev_clicked()
{
    ui->list_equipe->clear();
    //adicionar desenvolvedores na equipe
    bool add_novo = true;
    for(int i = 0; i < 1; i++){
        for(int j = 0; j < nome_desenvolvedores.size(); j++){
            if(ui->combo_gerdevs->currentText() == nome_desenvolvedores[j]){
                QMessageBox::information(this, "Duplicado", "O usuário selecionado já se encontra na lista");
                add_novo = false;
                break;
            }
        }
        if(add_novo == true)
            nome_desenvolvedores.push_back(ui->combo_gerdevs->currentText());
        else
            break;
    }

    for(QString &nome:nome_desenvolvedores)
        ui->list_equipe->addItem(nome);
}

void Gerenciador::on_button_gerenviar_clicked()
{
    if(ui->list_equipe->count() > 0){
        QSqlQuery encontra_projeto;
        encontra_projeto.prepare("select * from registro_projetos where nome_projeto='"+ui->combo_gerproj->currentText()+"'");
        if(encontra_projeto.exec()){
            encontra_projeto.first();
            QSqlQuery encontra_dev;
            for(int i = 0; i < nome_desenvolvedores.size(); i++){
                encontra_dev.prepare("select * from registro_programadores where nome_programador='"+nome_desenvolvedores[i]+"'");
                if(encontra_dev.exec()){
                        encontra_dev.first();
                        QSqlQuery add_dev;
                        add_dev.prepare("insert into programadores_projetos(id_programador, id_projeto) values(?, ?)");
                        add_dev.addBindValue(encontra_dev.value(0));
                        add_dev.addBindValue(encontra_projeto.value(0));

                        add_dev.exec();
                    }
                }
            }
        QMessageBox::information(this, "Modificações Efetuadas", "Suas modificações foram efetuadas com sucesso, você agora pode começar a distribuir tickets");
    }
    nome_desenvolvedores.clear();
    ui->combo_gerdevs->setCurrentIndex(0);
    ui->combo_gerproj->setCurrentIndex(0);
    ui->list_equipe->clear();
}

void Gerenciador::on_button_enviacademp_clicked()
{
    if(ui->line_nomregempresa->text() != "" && ui->text_descricaoempresa->toPlainText() != ""){
        QSqlQuery checa_existente;
        checa_existente.prepare("select * from registro_empresas where nome_empresa='"+ui->line_nomregempresa->text()+"'");
        if(checa_existente.exec()){
            int count = 0;

            while(checa_existente.next())
                count++;

            if(count >= 1){
                QMessageBox::information(this, "Erro", "O nome da empresa já existe, por favor escolha outro.");
            }else{
                QSqlQuery registro;
                registro.prepare("insert into registro_empresas(nome_empresa, descricao_empresa) values(?, ?)");

                registro.addBindValue(ui->line_nomregempresa->text());
                registro.addBindValue(ui->text_descricaoempresa->toPlainText());

                if(registro.exec()){
                    QMessageBox::StandardButton confirmar;
                    confirmar = QMessageBox::information(this, "Registro Efetuado", "Sua empresa foi registrada com sucesso.\n"
                                                        "A partir de agora você poderá aceitar aplicações de usuários para sua"
                                                        "empresa.\nTambém poderá designar Managers que irão te auxiliar no processo"
                                                        " de desenvolvimento do seu projeto, abrir tickets, criar projetos e etc.", QMessageBox::Ok);
                    if(confirmar == QMessageBox::Ok){
                        ui->button_tickets->setVisible(true);
                        ui->button_projetos->setVisible(true);
                        ui->button_solved->setVisible(true);
                        ui->button_tickets_equipe->setVisible(true);
                        ui->button_solved_equipe->setVisible(true);
                        ui->button_novo->setVisible(true);
                        ui->label_19->setVisible(true);
                        ui->combo_desenvolvedoredit->setVisible(true);
                        ui->button_gerenciador_projetos->setVisible(true);
                        ui->button_regprojeto->setVisible(true);
                        ui->button_funcionarios->setVisible(true);
                        ui->button_geraplic->setVisible(true);
                        ui->button_proj_modmanager->setVisible(true);
                        ui->combo_projmanager->setEnabled(true);
                        ui->button_dadosempresa->setVisible(false);

                        //Adiciona o ID da empresa para o desenvolvedor que a criou
                        QSqlQuery registro_empresa_dev;
                        registro_empresa_dev.prepare("UPDATE registro_programadores SET empresa='"+registro.lastInsertId().toString()+"', permissao='Admin', status_aplicacao='Aceito' WHERE id_programador='"+usuario_conectado.getId()+"'");
                        if(registro_empresa_dev.exec()){
                            ui->stacked_gerenciador->setCurrentIndex(0);
                            ui->line_nomregempresa->clear();
                            ui->text_descricaoempresa->clear();
                        }

                        QSqlQuery atualiza_usuario;
                        atualiza_usuario.prepare("SELECT * FROM registro_programadores WHERE id_programador='"+usuario_conectado.getId()+"'");
                        if(atualiza_usuario.exec())
                            this->atualizaLogado(atualiza_usuario);
                    }
                }
            }
        }
    }else
        QMessageBox::information(this, "Erro", "É necessário o preenchimento dos campos de nome e descrição da empresa para continuar.");

}

void Gerenciador::on_line_procurarempresa_textChanged(const QString &arg1)
{
    if(ui->combo_metodobusca->currentIndex() == 0 && arg1 != ""){
        QSqlQuery procurar_empresas;
        procurar_empresas.prepare("SELECT * FROM registro_empresas WHERE nome_empresa LIKE :partial");
        procurar_empresas.bindValue(":partial", QString("%%1%").arg(arg1));

        //encontra valores e adiciona ao tablewidget
        if(procurar_empresas.exec()){
            int n = 0; //row number

            ui->table_listaempresas->setRowCount(0);
            ui->table_listaempresas->setColumnCount(2);

            while(procurar_empresas.next()){
                ui->table_listaempresas->insertRow(n);
                ui->table_listaempresas->setItem(n, 0, new QTableWidgetItem(procurar_empresas.value(0).toString()));
                ui->table_listaempresas->setItem(n, 1, new QTableWidgetItem(procurar_empresas.value(1).toString()));
                ui->table_listaempresas->setRowHeight(n, 25);
                n++;
            }

            //tamanho das colunas
            ui->table_listaempresas->setColumnWidth(0, 150);
            ui->table_listaempresas->setColumnWidth(1, 300);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"ID da Empresa", "Nome da Empresa"};
            ui->table_listaempresas->setHorizontalHeaderLabels(cabecalho);
            ui->table_listaempresas->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->table_listaempresas->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->table_listaempresas->verticalHeader()->setVisible(false);
            ui->table_listaempresas->setStyleSheet("QTableView {selection-background-color: blue; background-color:rgb(211, 225, 255);}");
        }
    }else if(ui->combo_metodobusca->currentIndex() == 1 && arg1 != ""){
        QSqlQuery procurar_empresas;
        procurar_empresas.prepare("SELECT * FROM registro_empresas WHERE id LIKE :partial");
        procurar_empresas.bindValue(":partial", QString("%%1%").arg(arg1));

        //encontra valores e adiciona ao tablewidget
        if(procurar_empresas.exec()){
            int n = 0; //row number

            ui->table_listaempresas->setRowCount(0);
            ui->table_listaempresas->setColumnCount(2);

            while(procurar_empresas.next()){
                ui->table_listaempresas->insertRow(n);
                ui->table_listaempresas->setItem(n, 0, new QTableWidgetItem(procurar_empresas.value(0).toString()));
                ui->table_listaempresas->setItem(n, 1, new QTableWidgetItem(procurar_empresas.value(1).toString()));
                ui->table_listaempresas->setRowHeight(n, 25);
                n++;
            }

            //tamanho das colunas
            ui->table_listaempresas->setColumnWidth(0, 150);
            ui->table_listaempresas->setColumnWidth(1, 299);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"ID da Empresa", "Nome da Empresa"};
            ui->table_listaempresas->setHorizontalHeaderLabels(cabecalho);
            ui->table_listaempresas->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->table_listaempresas->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->table_listaempresas->verticalHeader()->setVisible(false);
            ui->table_listaempresas->setStyleSheet("QTableView {selection-background-color: blue; background-color: white;}");
        }
    }

    if(ui->line_procurarempresa->text() == "")
        ui->table_listaempresas->setRowCount(0);
}

void Gerenciador::on_button_registrarempdev_clicked()
{
    ui->stacked_gerenciador->setCurrentIndex(8);
}

void Gerenciador::on_button_funcionarios_clicked()
{
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(10);

    QSqlQuery lista_funcionarios;
    lista_funcionarios.prepare("SELECT * FROM registro_programadores WHERE empresa='"+usuario_conectado.getEmpresa()+"' AND permissao!='Admin' AND status_aplicacao='Aceito' ORDER BY nome_programador ASC");
    if(lista_funcionarios.exec()){
        int count = 0;

        //pre-definição do tableWidget
        ui->table_funcionarios->setRowCount(0);
        ui->table_funcionarios->setColumnCount(3);

        while(lista_funcionarios.next()){
            ui->table_funcionarios->insertRow(count);
            ui->table_funcionarios->setItem(count, 0, new QTableWidgetItem(lista_funcionarios.value(0).toString()));
            ui->table_funcionarios->setItem(count, 1, new QTableWidgetItem(lista_funcionarios.value(2).toString()));
            ui->table_funcionarios->setItem(count, 2, new QTableWidgetItem(lista_funcionarios.value(6).toString()));
            ui->table_funcionarios->setRowHeight(0, 25);
            count++;
        }
        //tamanho das colunas
        ui->table_funcionarios->setColumnWidth(0, 125);
        ui->table_funcionarios->setColumnWidth(1, 400);
        ui->table_funcionarios->setColumnWidth(2, 250);

        //formatação de nomes,triggers e css
        QStringList cabecalho = {"ID de Registro", "Nome da Funcionário", "Cargo de Cadastro"};
        ui->table_funcionarios->setHorizontalHeaderLabels(cabecalho);
        ui->table_funcionarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->table_funcionarios->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_funcionarios->verticalHeader()->setVisible(false);
        ui->table_funcionarios->setStyleSheet("QTableView {selection-background-color: blue;}");
    }
}

void Gerenciador::on_table_funcionarios_itemDoubleClicked(QTableWidgetItem *item)
{
    ui->stacked_gerenciador->setCurrentIndex(11);

    item = ui->table_funcionarios->item(ui->table_funcionarios->currentRow(), 0);
    QString id = item->data(Qt::DisplayRole).toString();

    QSqlQuery carregar_dados;
    carregar_dados.prepare("select * from registro_programadores where id_programador='"+id+"'");
    if(carregar_dados.exec()){
        int count = 0;

        while(carregar_dados.next())
            count++;

        if(count == 1){
            carregar_dados.first();
            ui->line_funcid->setText(carregar_dados.value(0).toString());
            ui->line_funcnome->setText(carregar_dados.value(2).toString());
            ui->line_funccargo->setText(carregar_dados.value(6).toString());
            ui->line_funcemail->setText(carregar_dados.value(5).toString());
        }
    }
}

void Gerenciador::on_button_desligarfunc_clicked()
{
    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(this, "Confirme para prosseguir", "Deseja remover o funcionário da lista de empregados da empresa?", QMessageBox::Yes | QMessageBox::No);

    if(botao == QMessageBox::Yes){
        QSqlQuery atualiza_usuario;
        atualiza_usuario.prepare("UPDATE registro_programadores SET permissao='Desenvolvedor Jr.',empresa=NULL,status_aplicacao=NULL WHERE id_programador='"+ui->line_funcid->text()+"'");
        if(atualiza_usuario.exec()){
            QSqlQuery deletar_conexoes;
            deletar_conexoes.exec("DELETE FROM programadores_projetos WHERE id_programador='"+ui->line_funcid->text()+"'");

            QSqlQuery criar_evento;
            criar_evento.prepare("INSERT INTO registro_eventos (id_programador, titulo_evento, mensagem_evento) values(?, ?, ?)");
            criar_evento.addBindValue(ui->line_funcid->text());
            criar_evento.addBindValue("Removido da Empresa");
            criar_evento.addBindValue("Lamentamos em informar que você foi removido da empresa em que estava registrado.\nPor padrão modificamos seu cargo para Desenvolvedor Jr. no botão \nConfigurações de Conta é possível modificar o cargo e realizar uma aplicação para uma nova empresa");

            if(criar_evento.exec()){
                QMessageBox::information(this, "Usuario Removido", "O usuário foi removido da lista de funcionários");
                this->on_button_funcionarios_clicked();
            }
        }
    }else
        return;
}

void Gerenciador::on_button_promoverfunc_clicked()
{
    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(this, "Confirme para prosseguir", "Deseja modificar o cargo do usuario: '"+ui->line_funcnome->text()+"' para MANAGER", QMessageBox::Yes | QMessageBox::No);

    if(botao == QMessageBox::Yes){
        QSqlQuery atualiza_usuario;
        atualiza_usuario.prepare("UPDATE registro_programadores SET permissao='Manager' WHERE id_programador='"+ui->line_funcid->text()+"'");
        if(atualiza_usuario.exec())
            QMessageBox::information(this, "Usuário promovido", "O usuário foi promovido ao cargo de manager");
        ui->stacked_gerenciador->setCurrentIndex(10);
    }else
        return;
}

void Gerenciador::on_button_meusprojetos_clicked()
{
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(12);

    if(usuario_conectado.getCargo() == "Admin"){
        QSqlQuery busca_projetos;
        busca_projetos.prepare("SELECT * FROM registro_projetos WHERE empresa='"+usuario_conectado.getEmpresa()+"' ORDER BY id_projeto");

        if(busca_projetos.exec()){
            int count = 0;

            ui->table_meusprojetos->setRowCount(0);
            ui->table_meusprojetos->setColumnCount(3);

            while(busca_projetos.next()){
                ui->table_meusprojetos->insertRow(count);
                ui->table_meusprojetos->setItem(count, 0, new QTableWidgetItem(busca_projetos.value(0).toString()));
                ui->table_meusprojetos->setItem(count, 1, new QTableWidgetItem(busca_projetos.value(1).toString()));
                ui->table_meusprojetos->setItem(count, 2, new QTableWidgetItem(busca_projetos.value(2).toString()));
                ui->table_meusprojetos->setBackgroundRole(QPalette::Dark);
                ui->table_meusprojetos->setRowHeight(count, 25);
                count++;
            }

            //tamanho das colunas
            ui->table_meusprojetos->setColumnWidth(0, 100);
            ui->table_meusprojetos->setColumnWidth(1, 200);
            ui->table_meusprojetos->setColumnWidth(2, 350);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"ID Projeto", "Nome do Projeto", "Descrição do Projeto"};
            ui->table_meusprojetos->setHorizontalHeaderLabels(cabecalho);
            ui->table_meusprojetos->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->table_meusprojetos->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->table_meusprojetos->verticalHeader()->setVisible(false);
            ui->table_meusprojetos->setStyleSheet("QTableView {selection-background-color: blue; background-color: white;}");
        }
    }else if(usuario_conectado.getCargo() == "Manager"){
        QSqlQuery busca_projetos;
        busca_projetos.prepare("SELECT * FROM registro_projetos WHERE id_manager='"+usuario_conectado.getId()+"' ORDER BY id_projeto");

        if(busca_projetos.exec()){
            int count = 0;

            ui->table_meusprojetos->setRowCount(0);
            ui->table_meusprojetos->setColumnCount(3);

            while(busca_projetos.next()){
                ui->table_meusprojetos->insertRow(count);
                ui->table_meusprojetos->setItem(count, 0, new QTableWidgetItem(busca_projetos.value(0).toString()));
                ui->table_meusprojetos->setItem(count, 1, new QTableWidgetItem(busca_projetos.value(1).toString()));
                ui->table_meusprojetos->setItem(count, 2, new QTableWidgetItem(busca_projetos.value(2).toString()));
                ui->table_meusprojetos->setBackgroundRole(QPalette::Dark);
                ui->table_meusprojetos->setRowHeight(count, 25);
                count++;
            }

            //tamanho das colunas
            ui->table_meusprojetos->setColumnWidth(0, 100);
            ui->table_meusprojetos->setColumnWidth(1, 200);
            ui->table_meusprojetos->setColumnWidth(2, 350);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"ID Projeto", "Nome do Projeto", "Descrição do Projeto"};
            ui->table_meusprojetos->setHorizontalHeaderLabels(cabecalho);
            ui->table_meusprojetos->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->table_meusprojetos->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->table_meusprojetos->verticalHeader()->setVisible(false);
            ui->table_meusprojetos->setStyleSheet("QTableView {selection-background-color: blue; background-color: white;}");
        }
    }else{
        QSqlQuery busca_projetos;
        busca_projetos.prepare("SELECT * FROM registro_projetos WHERE id_projeto in (SELECT id_projeto FROM programadores_projetos WHERE id_programador='"+usuario_conectado.getId()+"' )");

        if(busca_projetos.exec()){
            int count = 0;

            ui->table_meusprojetos->setRowCount(0);
            ui->table_meusprojetos->setColumnCount(3);

            while(busca_projetos.next()){
                ui->table_meusprojetos->insertRow(count);
                ui->table_meusprojetos->setItem(count, 0, new QTableWidgetItem(busca_projetos.value(0).toString()));
                ui->table_meusprojetos->setItem(count, 1, new QTableWidgetItem(busca_projetos.value(1).toString()));
                ui->table_meusprojetos->setItem(count, 2, new QTableWidgetItem(busca_projetos.value(2).toString()));
                ui->table_meusprojetos->setBackgroundRole(QPalette::Dark);
                ui->table_meusprojetos->setRowHeight(count, 25);
                count++;
            }

            //tamanho das colunas
            ui->table_meusprojetos->setColumnWidth(0, 100);
            ui->table_meusprojetos->setColumnWidth(1, 200);
            ui->table_meusprojetos->setColumnWidth(2, 350);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"ID Projeto", "Nome do Projeto", "Descrição do Projeto"};
            ui->table_meusprojetos->setHorizontalHeaderLabels(cabecalho);
            ui->table_meusprojetos->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->table_meusprojetos->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->table_meusprojetos->verticalHeader()->setVisible(false);
            ui->table_meusprojetos->setStyleSheet("QTableView {selection-background-color: blue; background-color: white;}");

        }
    }
}

void Gerenciador::on_table_resolvidos_itemDoubleClicked(QTableWidgetItem *item)
{
    //limpa cache e adiciona os valores para _bug
    _bug.limpaCache();
    this->limpaCache_ticket();
    item = ui->table_resolvidos->item(ui->table_resolvidos->currentRow(), 0);
    _bug.setData_hora(item->data(Qt::DisplayRole).toString());
    item = ui->table_resolvidos->item(ui->table_resolvidos->currentRow(), 2);
    _bug.setTitulo(item->data(Qt::DisplayRole).toString());

    if(usuario_conectado.getCargo() == "Admin" || usuario_conectado.getCargo() == "Manager")
        ui->combo_prioridadeedit->setEnabled(true);
    else
        ui->combo_prioridadeedit->setEnabled(false);

    QSqlQuery abrir_ticket;
    abrir_ticket.prepare("select * from registro_bugs where titulo='"+_bug.getTitulo()+"' and data_hora='"+_bug.getData_hora()+"'");
    if(abrir_ticket.exec()){
        int count = 0;
        while(abrir_ticket.next())
            count++;

        if(count == 1){
            abrir_ticket.first();

            //inserir valores nas labels posicionadas
            ui->label_idticket->setText(abrir_ticket.value(0).toString());
            ui->label_autor->setText(abrir_ticket.value(2).toString());
            ui->label_nomeprojeto->setText(abrir_ticket.value(1).toString());
            ui->label_prioridade->setText(abrir_ticket.value(5).toString());
            ui->label_tituloticket_2->setText(abrir_ticket.value(9).toString());
            ui->label_nomedesignadoedit->setText(abrir_ticket.value(4).toString());
            ui->text_descricao->setText(abrir_ticket.value(7).toString());
            ui->label_status->setText(abrir_ticket.value(6).toString());
            ui->stacked_gerenciador->setCurrentIndex(3);

            //se não existirem arquivos anexados, não mostrar botão!
            if(abrir_ticket.value(11) == "")
                ui->button_download->setHidden(true);

            //procurar por comentários no ticket
            QSqlQuery busca_comentario;
            busca_comentario.prepare("SELECT * FROM registro_comentarios WHERE id_bug='"+ui->label_idticket->text()+"' ORDER BY id_comentario DESC");
            if(busca_comentario.exec()){
                int count = 0;
                ui->table_comentarios->setColumnCount(4);
                ui->table_comentarios->setRowCount(0);
                while(busca_comentario.next()){
                    ui->table_comentarios->insertRow(count);
                    ui->table_comentarios->setItem(count, 0, new QTableWidgetItem(busca_comentario.value(4).toString()));
                    ui->table_comentarios->setItem(count, 1, new QTableWidgetItem(busca_comentario.value(2).toString()));
                    ui->table_comentarios->setItem(count, 2, new QTableWidgetItem(busca_comentario.value(3).toString()));
                    ui->table_comentarios->setItem(count, 3, new QTableWidgetItem(busca_comentario.value(0).toString()));
                    ui->table_comentarios->setRowHeight(count, 25);
                    count++;
                }

                //layout da tableWidget
                item = ui->table_resolvidos->item(ui->table_resolvidos->currentRow(), 0);
                QString layout_teste = item->data(Qt::DisplayRole).toString();
                if(layout_teste == ""){
                    ui->table_comentarios->setColumnWidth(0, 300);
                    ui->table_comentarios->setColumnWidth(1, 300);
                    ui->table_comentarios->setColumnWidth(2, 300);
                }else{
                    ui->table_comentarios->resizeColumnToContents(2);
                    ui->table_comentarios->resizeColumnToContents(0);
                    ui->table_comentarios->resizeColumnToContents(1);
                }

                //formatação de nomes,triggers e css
                QStringList cabecalho = {"Data/Hora", "Autor", "Comentário"};
                ui->table_comentarios->setHorizontalHeaderLabels(cabecalho);
                ui->table_comentarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->table_comentarios->setSelectionBehavior(QAbstractItemView::SelectRows);
                ui->table_comentarios->verticalHeader()->setVisible(false);
                ui->table_comentarios->setColumnHidden(3, true);
                ui->table_comentarios->setStyleSheet("QTableView {selection-background-color: grey;}");

            }
            this->refresh_modificacoes();
        }
    }
}

void Gerenciador::refresh_modificacoes(){
    QSqlQuery busca_modificacoes;
    busca_modificacoes.prepare("SELECT * FROM registro_modificacoes_tickets WHERE "
                                    "id_bug='"+ui->label_idticket->text()+"' ORDER BY id_modificacao DESC");

    if(busca_modificacoes.exec()){
        int count = 0;

        ui->table_historico_modificacoes->setRowCount(0);
        ui->table_historico_modificacoes->setColumnCount(4);

        while(busca_modificacoes.next()){
            ui->table_historico_modificacoes->insertRow(count);
            ui->table_historico_modificacoes->setItem(count, 0, new QTableWidgetItem(busca_modificacoes.value(2).toString()));
            ui->table_historico_modificacoes->setItem(count, 1, new QTableWidgetItem(busca_modificacoes.value(3).toString()));
            ui->table_historico_modificacoes->setItem(count, 2, new QTableWidgetItem(busca_modificacoes.value(4).toString()));
            ui->table_historico_modificacoes->setItem(count, 3, new QTableWidgetItem(busca_modificacoes.value(5).toString()));
            ui->table_historico_modificacoes->setRowHeight(count, 25);
            count++;
        }

        //layout da tableWidget
        ui->table_historico_modificacoes->setColumnWidth(0, 175);
        ui->table_historico_modificacoes->setColumnWidth(1, 250);
        ui->table_historico_modificacoes->setColumnWidth(2, 250);
        ui->table_historico_modificacoes->setColumnWidth(3, 200);

        //formatação de nomes,triggers e css
        QStringList cabecalho = {"Modificado", "Valor Antigo", "Valor Novo", "Data Modificacao"};
        ui->table_historico_modificacoes->setHorizontalHeaderLabels(cabecalho);
        ui->table_historico_modificacoes->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->table_historico_modificacoes->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_historico_modificacoes->verticalHeader()->setVisible(false);
        ui->table_historico_modificacoes->setStyleSheet("QTableView {selection-background-color: grey;}");
    }
}

void Gerenciador::on_button_enviarpedido_clicked()
{
    if (ui->table_listaempresas->selectedItems().length() != 0){
        QString id;
        QString nome;
        QTableWidgetItem *item = ui->table_listaempresas->item(ui->table_listaempresas->currentRow(), 0);
        id = item->data(Qt::DisplayRole).toString();
        item = ui->table_listaempresas->item(ui->table_listaempresas->currentRow(), 1);
        nome = item->data(Qt::DisplayRole).toString();

        if(ui->table_listaempresas->selectionModel()->selectedRows().length() > 1){
            QMessageBox::information(this, "Erro", "Somente é possivel realizar solicitação a uma empresa");
            ui->line_procurarempresa->clear();
            ui->line_procurarempresa->setFocus();
        }else if(ui->table_listaempresas->selectionModel()->selectedRows().length() == 1){
            QMessageBox::StandardButton botao;
            botao = QMessageBox::question(this, "Confirmar escolha", "Você está se aplicando para a empresa de ID:'"+id+"'\nNome da Empresa: '"+nome+"'\nDeseja continuar?", QMessageBox::Yes | QMessageBox::No);

            if(botao == QMessageBox::Yes){
                QSqlQuery encaminha_pedido;
                encaminha_pedido.prepare("UPDATE registro_programadores SET status_aplicacao='Pendente',id_aplicado='"+id+"' WHERE id_programador='"+usuario_conectado.getId()+"'");

                if(encaminha_pedido.exec()){
                    QMessageBox::information(this, "Pedido Enviado", "Sua solicitação foi enviada ao administrador da empresa.\nAguarde a aprovação.");
                    delete[] item;

                    ui->line_procurarempresa->clear();
                    ui->table_listaempresas->clear();

                    ui->stacked_gerenciador->setCurrentIndex(0);
                    ui->button_meusprojetos->setVisible(true);
                    ui->button_solved->setVisible(true);
                    ui->button_tickets->setVisible(true);
                    ui->button_projetos->setVisible(true);
                }

            }else{
                return;
            }
        }
    }else
        return;
}

void Gerenciador::on_button_geraplic_clicked()
{
    this->resetar_variaveis();

    QSqlQuery procurar_aplicacoes;
    procurar_aplicacoes.prepare("SELECT * FROM registro_programadores WHERE status_aplicacao='Pendente' AND id_aplicado='"+usuario_conectado.getEmpresa()+"' ORDER BY nome_programador ASC");

    if(procurar_aplicacoes.exec()){
        int count = 0;

        //resetar nº de linhas e definir colunas para nao sobrecarregar o table com linhas extras
        ui->table_aplicacoes->setRowCount(0);
        ui->table_aplicacoes->setColumnCount(3);

        while(procurar_aplicacoes.next()){
            ui->table_aplicacoes->insertRow(count);
            ui->table_aplicacoes->setItem(count, 0, new QTableWidgetItem(procurar_aplicacoes.value(0).toString()));
            ui->table_aplicacoes->setItem(count, 1, new QTableWidgetItem(procurar_aplicacoes.value(2).toString()));
            ui->table_aplicacoes->setItem(count, 2, new QTableWidgetItem(procurar_aplicacoes.value(5).toString()));
            ui->table_aplicacoes->setRowHeight(count, 25);
            count++;
        }

        //layout da tableWidget
        ui->table_aplicacoes->setColumnWidth(0, 150);
        ui->table_aplicacoes->setColumnWidth(1, 299);
        ui->table_aplicacoes->setColumnWidth(2, 300);

        //formatação de nomes,triggers e css
        QStringList cabecalho = {"ID do Programador", "Nome do Programador", "E-mail"};
        ui->table_aplicacoes->setHorizontalHeaderLabels(cabecalho);
        ui->table_aplicacoes->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->table_aplicacoes->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->table_aplicacoes->verticalHeader()->setVisible(false);
        ui->table_aplicacoes->setStyleSheet("QTableView {selection-background-color: grey; background-color: white;}");
    }
    ui->stacked_gerenciador->setCurrentIndex(13);
}

void Gerenciador::on_table_aplicacoes_itemDoubleClicked(QTableWidgetItem *item)
{
    Usuario temporario;
    item = ui->table_aplicacoes->item(ui->table_aplicacoes->currentRow(), 0);
    temporario.setId(item->data(Qt::DisplayRole).toString());
    item = ui->table_aplicacoes->item(ui->table_aplicacoes->currentRow(), 1);
    temporario.setNome(item->data(Qt::DisplayRole).toString());
    item = ui->table_aplicacoes->item(ui->table_aplicacoes->currentRow(), 2);
    temporario.setEmail(item->data(Qt::DisplayRole).toString());

    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(this, "Confirmar escolha", "Você deseja aceitar o usuario na sua empresa?\nUsuário: '"+temporario.getNome()+"'"
                                "\nID: '"+temporario.getId()+"'\nE-mail: '"+temporario.getEmail()+"'", QMessageBox::Yes | QMessageBox::No);

    if(botao == QMessageBox::Yes){
        QSqlQuery aceitar_pedido;
        aceitar_pedido.prepare("UPDATE registro_programadores SET empresa='"+usuario_conectado.getEmpresa()+"',status_aplicacao='Aceito',id_aplicado=NULL WHERE id_programador='"+temporario.getId()+"'");

        if(aceitar_pedido.exec()){
            QMessageBox::information(this, "Novo funcionário cadastrado", "O usuário '"+temporario.getNome()+"' foi adicionado a sua equipe de funcionários.");

            QSqlQuery criar_evento;
            criar_evento.prepare("INSERT INTO registro_eventos (id_programador, titulo_evento, mensagem_evento) values(?, ?, ?)");
            criar_evento.addBindValue(temporario.getId());
            criar_evento.addBindValue("Pedido Aceito");
            criar_evento.addBindValue("Seu pedido para ingressar na empresa de ID:'"+usuario_conectado.getEmpresa()+"' foi aceito.");
            criar_evento.exec();

            this->on_button_geraplic_clicked();
            }
    }else
        return;
}

void Gerenciador::on_table_meusprojetos_itemDoubleClicked(QTableWidgetItem *item)
{
    QString id;
    item = ui->table_meusprojetos->item(ui->table_meusprojetos->currentRow(), 0);
    id = item->data(Qt::DisplayRole).toString();
    if(usuario_conectado.getCargo() == "Admin" || usuario_conectado.getCargo() == "Manager"){
        QSqlQuery encontra_projeto;
        encontra_projeto.prepare("SELECT * FROM registro_projetos WHERE id_projeto='"+id+"'");
        if(encontra_projeto.exec()){
            int count = 0;

            while(encontra_projeto.next())
                count++;

            if(count == 1){
                encontra_projeto.first();
                ui->label_projid->setText(encontra_projeto.value(0).toString());
                ui->label_projnome->setText(encontra_projeto.value(1).toString());
                ui->text_projdesc->setText(encontra_projeto.value(2).toString());
                ui->stacked_gerenciador->setCurrentIndex(14);

                QSqlQuery preenche_combodev;
                preenche_combodev.prepare("SELECT * FROM registro_programadores WHERE permissao='Manager' OR permissao='Admin' AND empresa='"+usuario_conectado.getEmpresa()+"'");
                if(preenche_combodev.exec()){
                    while(preenche_combodev.next())
                        ui->combo_projmanager->addItem(preenche_combodev.value(2).toString());
                }

                encontra_manager.prepare("SELECT * FROM registro_programadores WHERE id_programador='"+encontra_projeto.value(3).toString()+"'");
                if(encontra_manager.exec()){
                    int count2 = 0;

                    while(encontra_manager.next())
                        count2++;

                    if(count2 == 1){
                        encontra_manager.first();
                        ui->combo_projmanager->setCurrentIndex(ui->combo_projmanager->findText(encontra_manager.value(2).toString()));
                    }
                }
            }       
        }
    }
}

void Gerenciador::on_button_projdel_clicked()
{
    bool ok = true;
    QMessageBox::StandardButton botao;
    botao = QMessageBox::question(this, "Escolha uma opção", "Tem certeza que deseja deletar o projeto selecionado?", QMessageBox::Yes | QMessageBox::No);
    if(botao == QMessageBox::Yes){
        QSqlQuery deletar_projeto;
        deletar_projeto.exec("DELETE FROM registro_projetos WHERE id_projeto='"+ui->label_projid->text()+"'");
        deletar_projeto.exec("DELETE FROM programadores_projetos WHERE id_projeto='"+QString::number(ui->label_projid->text().toInt(&ok))+"'");
        deletar_projeto.exec("DELETE FROM registro_comentarios WHERE id_bug in(SELECT id_bug FROM registro_bugs WHERE nome_projeto='"+ui->label_projnome->text()+"' AND empresa='"+usuario_conectado.getEmpresa()+"')");
        deletar_projeto.exec("DELETE FROM registro_modificacoes_tickets WHERE id_bug in (SELECT id_bug FROM registro_bugs WHERE nome_projeto='"+ui->label_projnome->text()+"' AND empresa='"+usuario_conectado.getEmpresa()+"')");
        deletar_projeto.exec("DELETE FROM registro_bugs WHERE nome_projeto='"+ui->label_projnome->text()+"' AND empresa='"+usuario_conectado.getEmpresa()+"'");
//        deletar_projeto.exec("DELETE FROM bugs_projetos WHERE id_projeto='"+QString::number(ui->label_projid->text().toInt(&ok))+"'");
        QMessageBox::information(this, "Projeto deletado", "O projeto '"+ui->label_projnome->text()+"' foi removido com sucesso");
        this->on_button_meusprojetos_clicked();
    }else
        return;
}

void Gerenciador::on_button_config_clicked()
{
    if(usuario_conectado.getCargo() == "Admin"){
        ui->button_dadoscadastrar->setVisible(true);
        ui->button_dadosempresa->setVisible(false);
    }else{
        ui->button_dadoscadastrar->setVisible(false);
        ui->button_dadosempresa->setVisible(true);
    }

    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(15);

    ui->label_dadosnome->setText(usuario_conectado.getNome());
    ui->label_dadosemail->setText(usuario_conectado.getEmail());
    ui->label_dadoscargo->setText(usuario_conectado.getCargo());

    if(usuario_conectado.getEmpresa() != ""){
        QSqlQuery encontra_empresa;
        encontra_empresa.prepare("SELECT * FROM registro_empresas WHERE id='"+usuario_conectado.getEmpresa()+"'");
        if(encontra_empresa.exec()){
            int count = 0;

            while(encontra_empresa.next())
                count++;

            if(count == 1){
                encontra_empresa.first();
                ui->label_dadosempresa->setText(encontra_empresa.value(1).toString());
            }
        }
    }else{
        ui->label_dadosempresa->setText("Você ainda não possui uma empresa cadastrada");
        ui->label_dadosempresa->setStyleSheet("color: red;");
    }

}

void Gerenciador::on_button_dadosempresa_clicked()
{
    ui->stacked_gerenciador->setCurrentIndex(9);
}

void Gerenciador::on_button_dadosmod_clicked()
{
    configuracoes.setCargo(usuario_conectado.getCargo());
    configuracoes.setEmail(usuario_conectado.getEmail());
    configuracoes.checa_cargo(usuario_conectado.getCargo());
    configuracoes.adjustSize();
    configuracoes.show();
}

void Gerenciador::on_button_dadossenha_clicked()
{
    definir_senha.show();
}

void Gerenciador::on_button_dadoscadastrar_clicked()
{
    ui->stacked_gerenciador->setCurrentIndex(8);
}

void Gerenciador::on_combo_projmanager_currentTextChanged(const QString &arg1)
{
    QString variavel = arg1;

    if(variavel != encontra_manager.value(2).toString())
        ui->button_proj_modmanager->setEnabled(true);
    else
        ui->button_proj_modmanager->setEnabled(false);
}

void Gerenciador::on_button_tickets_equipe_clicked()
{
    //trocar index do stackedwidget e resetar variaveis
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(16);

    if(usuario_conectado.getCargo() == "Admin"){
        QSqlQuery admin_query;
        admin_query.prepare("SELECT * FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"' AND status='Aberto' ORDER BY id_bug DESC");
        if(admin_query.exec()){
            int count = 0;

            ui->lista_tickets_equipe->setRowCount(0);
            ui->lista_tickets_equipe->setColumnCount(3);

            while(admin_query.next()){
                ui->lista_tickets_equipe->insertRow(count);
                ui->lista_tickets_equipe->setItem(count, 0, new QTableWidgetItem(admin_query.value(8).toString()));
                ui->lista_tickets_equipe->setItem(count, 1, new QTableWidgetItem(admin_query.value(1).toString()));
                ui->lista_tickets_equipe->setItem(count, 2, new QTableWidgetItem(admin_query.value(9).toString()));
                ui->lista_tickets_equipe->setRowHeight(count, 25);
                count++;
            }

            ui->lista_tickets_equipe->setColumnWidth(0, 200);
            ui->lista_tickets_equipe->setColumnWidth(1, 300);
            ui->lista_tickets_equipe->setColumnWidth(2, 600);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket"};
            ui->lista_tickets_equipe->setHorizontalHeaderLabels(cabecalho);
            ui->lista_tickets_equipe->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->lista_tickets_equipe->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->lista_tickets_equipe->verticalHeader()->setVisible(false);
            ui->lista_tickets_equipe->setStyleSheet("QTableView {selection-background-color: blue;}");
        }
    }else if(usuario_conectado.getCargo() == "Manager"){
        QSqlQuery manager_query;
        manager_query.prepare("SELECT * FROM registro_bugs WHERE nome_projeto in (SELECT nome_projeto FROM registro_projetos WHERE id_manager='"+usuario_conectado.getId()+"') AND status='Aberto' ORDER BY id_bug DESC");
        if(manager_query.exec()){
            int count = 0;

            ui->lista_tickets_equipe->setRowCount(0);
            ui->lista_tickets_equipe->setColumnCount(3);

            while(manager_query.next()){
                ui->lista_tickets_equipe->insertRow(count);
                ui->lista_tickets_equipe->setItem(count, 0, new QTableWidgetItem(manager_query.value(8).toString()));
                ui->lista_tickets_equipe->setItem(count, 1, new QTableWidgetItem(manager_query.value(1).toString()));
                ui->lista_tickets_equipe->setItem(count, 2, new QTableWidgetItem(manager_query.value(9).toString()));
                ui->lista_tickets_equipe->setRowHeight(count, 25);
                count++;
            }

            ui->lista_tickets_equipe->setColumnWidth(0, 200);
            ui->lista_tickets_equipe->setColumnWidth(1, 300);
            ui->lista_tickets_equipe->setColumnWidth(2, 600);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket"};
            ui->lista_tickets_equipe->setHorizontalHeaderLabels(cabecalho);
            ui->lista_tickets_equipe->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->lista_tickets_equipe->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->lista_tickets_equipe->verticalHeader()->setVisible(false);
            ui->lista_tickets_equipe->setStyleSheet("QTableView {selection-background-color: blue;}");
        }
    }
}


void Gerenciador::on_button_solved_equipe_clicked()
{
    //resets de valores definidos durante uso hidden/add_item etc..
    this->resetar_variaveis();
    ui->stacked_gerenciador->setCurrentIndex(17);

    if(usuario_conectado.getCargo() == "Admin"){
        QSqlQuery query;
        query.prepare("SELECT * FROM registro_bugs WHERE empresa='"+usuario_conectado.getEmpresa()+"' "
                  "AND status='Resolvido' ORDER BY data_fechamento DESC");
        if(query.exec()){
            int count = 0;

            ui->lista_resolvidos_equipe->setRowCount(0);
            ui->lista_resolvidos_equipe->setColumnCount(4);

            //encontra os ultimos tickets abertos para o usuario logado
            while(query.next()){
                ui->lista_resolvidos_equipe->insertRow(count);
                ui->lista_resolvidos_equipe->setItem(count, 0, new QTableWidgetItem(query.value(8).toString()));
                ui->lista_resolvidos_equipe->setItem(count, 1, new QTableWidgetItem(query.value(1).toString()));
                ui->lista_resolvidos_equipe->setItem(count, 2, new QTableWidgetItem(query.value(9).toString()));
                ui->lista_resolvidos_equipe->setItem(count, 3, new QTableWidgetItem(query.value(12).toString()));
                ui->lista_resolvidos_equipe->setRowHeight(count, 25);
                count++;
            }

            //tamanho das colunas
            ui->lista_resolvidos_equipe->setColumnWidth(0, 200);
            ui->lista_resolvidos_equipe->setColumnWidth(1, 200);
            ui->lista_resolvidos_equipe->setColumnWidth(2, 500);
            ui->lista_resolvidos_equipe->setColumnWidth(3, 150);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket", "Data de Fechamento"};
            ui->lista_resolvidos_equipe->setHorizontalHeaderLabels(cabecalho);
            ui->lista_resolvidos_equipe->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->lista_resolvidos_equipe->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->lista_resolvidos_equipe->verticalHeader()->setVisible(false);
            ui->lista_resolvidos_equipe->setStyleSheet("QTableView {selection-background-color: blue;}");
        }
    }else if(usuario_conectado.getCargo() == "Manager"){
        QSqlQuery manager_query;
        manager_query.prepare("SELECT * FROM registro_bugs WHERE nome_projeto in (SELECT nome_projeto FROM registro_projetos WHERE id_manager='"+usuario_conectado.getId()+"') AND status='Resolvido' ORDER BY data_fechamento DESC");
        if(manager_query.exec()){
            int count = 0;

            ui->lista_resolvidos_equipe->setRowCount(0);
            ui->lista_resolvidos_equipe->setColumnCount(3);

            while(manager_query.next()){
                ui->lista_resolvidos_equipe->insertRow(count);
                ui->lista_resolvidos_equipe->setItem(count, 0, new QTableWidgetItem(manager_query.value(8).toString()));
                ui->lista_resolvidos_equipe->setItem(count, 1, new QTableWidgetItem(manager_query.value(1).toString()));
                ui->lista_resolvidos_equipe->setItem(count, 2, new QTableWidgetItem(manager_query.value(9).toString()));
                ui->lista_resolvidos_equipe->setRowHeight(count, 25);
                count++;
            }

            ui->lista_resolvidos_equipe->setColumnWidth(0, 200);
            ui->lista_resolvidos_equipe->setColumnWidth(1, 200);
            ui->lista_resolvidos_equipe->setColumnWidth(2, 500);

            //formatação de nomes,triggers e css
            QStringList cabecalho = {"Data de Abertura", "Nome do Projeto", "Título do ticket"};
            ui->lista_resolvidos_equipe->setHorizontalHeaderLabels(cabecalho);
            ui->lista_resolvidos_equipe->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->lista_resolvidos_equipe->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->lista_resolvidos_equipe->verticalHeader()->setVisible(false);
            ui->lista_resolvidos_equipe->setStyleSheet("QTableView {selection-background-color: blue;}");
        }
    }
}


void Gerenciador::on_lista_tickets_equipe_itemDoubleClicked(QTableWidgetItem *item)
{
    //limpa cache e adiciona os valores para _bug
    _bug.limpaCache();
    this->limpaCache_ticket();
    item = ui->lista_tickets_equipe->item(ui->lista_tickets_equipe->currentRow(), 0);
    _bug.setData_hora(item->data(Qt::DisplayRole).toString());
    item = ui->lista_tickets_equipe->item(ui->lista_tickets_equipe->currentRow(), 2);
    _bug.setTitulo(item->data(Qt::DisplayRole).toString());

    if(usuario_conectado.getCargo() == "Admin" || usuario_conectado.getCargo() == "Manager")
        ui->combo_prioridadeedit->setEnabled(true);
    else
        ui->combo_prioridadeedit->setEnabled(false);

    QSqlQuery abrir_ticket;
    abrir_ticket.prepare("select * from registro_bugs where titulo='"+_bug.getTitulo()+"' and data_hora='"+_bug.getData_hora()+"'");
    if(abrir_ticket.exec()){
        int count = 0;
        while(abrir_ticket.next())
            count++;

        if(count == 1){
            abrir_ticket.first();

            //inserir valores nas labels posicionadas
            ui->label_idticket->setText(abrir_ticket.value(0).toString());
            ui->label_autor->setText(abrir_ticket.value(2).toString());
            ui->label_nomeprojeto->setText(abrir_ticket.value(1).toString());
            ui->label_prioridade->setText(abrir_ticket.value(5).toString());
            ui->label_tituloticket_2->setText(abrir_ticket.value(9).toString());
            ui->text_descricao->setText(abrir_ticket.value(7).toString());
            ui->label_status->setText(abrir_ticket.value(6).toString());
            ui->label_nomedesignadoedit->setText(abrir_ticket.value(4).toString());
            ui->stacked_gerenciador->setCurrentIndex(3);

            //se não existirem arquivos anexados, não mostrar botão!
            if(abrir_ticket.value(11) == "")
                ui->button_download->setHidden(true);

            //procurar por comentários no ticket
            QSqlQuery busca_comentario;
            busca_comentario.prepare("SELECT * FROM registro_comentarios WHERE id_bug='"+ui->label_idticket->text()+"' ORDER BY id_comentario DESC");
            if(busca_comentario.exec()){
                int count = 0;
                ui->table_comentarios->setColumnCount(4);
                ui->table_comentarios->setRowCount(0);
                while(busca_comentario.next()){
                    ui->table_comentarios->insertRow(count);
                    ui->table_comentarios->setItem(count, 0, new QTableWidgetItem(busca_comentario.value(4).toString()));
                    ui->table_comentarios->setItem(count, 1, new QTableWidgetItem(busca_comentario.value(2).toString()));
                    ui->table_comentarios->setItem(count, 2, new QTableWidgetItem(busca_comentario.value(3).toString()));
                    ui->table_comentarios->setItem(count, 3, new QTableWidgetItem(busca_comentario.value(0).toString()));
                    ui->table_comentarios->setRowHeight(count, 25);
                    count++;
                }

                //layout da tableWidget
                item = ui->lista_tickets_equipe->item(ui->lista_tickets_equipe->currentRow(), 0);
                QString layout_teste = item->data(Qt::DisplayRole).toString();
                if(layout_teste == ""){
                    ui->table_comentarios->setColumnWidth(0, 300);
                    ui->table_comentarios->setColumnWidth(1, 300);
                    ui->table_comentarios->setColumnWidth(2, 300);
                }else{
                    ui->table_comentarios->resizeColumnToContents(2);
                    ui->table_comentarios->resizeColumnToContents(0);
                    ui->table_comentarios->resizeColumnToContents(1);
                }

                //formatação de nomes,triggers e css
                QStringList cabecalho = {"Data/Hora", "Autor", "Comentário"};
                ui->table_comentarios->setHorizontalHeaderLabels(cabecalho);
                ui->table_comentarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->table_comentarios->setSelectionBehavior(QAbstractItemView::SelectRows);
                ui->table_comentarios->verticalHeader()->setVisible(false);
                ui->table_comentarios->setColumnHidden(3, true);
                ui->table_comentarios->setStyleSheet("QTableView {selection-background-color: grey;}");

            }
            this->refresh_modificacoes();
        }
    }
}


void Gerenciador::on_lista_resolvidos_equipe_itemDoubleClicked(QTableWidgetItem *item)
{
    //limpa cache e adiciona os valores para _bug
    _bug.limpaCache();
    this->limpaCache_ticket();
    item = ui->lista_resolvidos_equipe->item(ui->lista_resolvidos_equipe->currentRow(), 0);
    _bug.setData_hora(item->data(Qt::DisplayRole).toString());
    item = ui->lista_resolvidos_equipe->item(ui->lista_resolvidos_equipe->currentRow(), 2);
    _bug.setTitulo(item->data(Qt::DisplayRole).toString());

    if(usuario_conectado.getCargo() == "Admin" || usuario_conectado.getCargo() == "Manager")
        ui->combo_prioridadeedit->setEnabled(true);
    else
        ui->combo_prioridadeedit->setEnabled(false);

    QSqlQuery abrir_ticket;
    abrir_ticket.prepare("select * from registro_bugs where titulo='"+_bug.getTitulo()+"' and data_hora='"+_bug.getData_hora()+"'");
    if(abrir_ticket.exec()){
        int count = 0;
        while(abrir_ticket.next())
            count++;

        if(count == 1){
            abrir_ticket.first();

            //inserir valores nas labels posicionadas
            ui->label_idticket->setText(abrir_ticket.value(0).toString());
            ui->label_autor->setText(abrir_ticket.value(2).toString());
            ui->label_nomeprojeto->setText(abrir_ticket.value(1).toString());
            ui->label_prioridade->setText(abrir_ticket.value(5).toString());
            ui->label_tituloticket_2->setText(abrir_ticket.value(9).toString());
            ui->text_descricao->setText(abrir_ticket.value(7).toString());
            ui->label_status->setText(abrir_ticket.value(6).toString());
            ui->label_nomedesignadoedit->setText(abrir_ticket.value(4).toString());
            ui->stacked_gerenciador->setCurrentIndex(3);

            //se não existirem arquivos anexados, não mostrar botão!
            if(abrir_ticket.value(11) == "")
                ui->button_download->setHidden(true);

            //procurar por comentários no ticket
            QSqlQuery busca_comentario;
            busca_comentario.prepare("SELECT * FROM registro_comentarios WHERE id_bug='"+ui->label_idticket->text()+"' ORDER BY id_comentario DESC");
            if(busca_comentario.exec()){
                int count = 0;
                ui->table_comentarios->setColumnCount(4);
                ui->table_comentarios->setRowCount(0);
                while(busca_comentario.next()){
                    ui->table_comentarios->insertRow(count);
                    ui->table_comentarios->setItem(count, 0, new QTableWidgetItem(busca_comentario.value(4).toString()));
                    ui->table_comentarios->setItem(count, 1, new QTableWidgetItem(busca_comentario.value(2).toString()));
                    ui->table_comentarios->setItem(count, 2, new QTableWidgetItem(busca_comentario.value(3).toString()));
                    ui->table_comentarios->setItem(count, 3, new QTableWidgetItem(busca_comentario.value(0).toString()));
                    ui->table_comentarios->setRowHeight(count, 25);
                    count++;
                }

                //layout da tableWidget
                item = ui->lista_resolvidos_equipe->item(ui->lista_resolvidos_equipe->currentRow(), 0);
                QString layout_teste = item->data(Qt::DisplayRole).toString();
                if(layout_teste == ""){
                    ui->table_comentarios->setColumnWidth(0, 300);
                    ui->table_comentarios->setColumnWidth(1, 300);
                    ui->table_comentarios->setColumnWidth(2, 300);
                }else{
                    ui->table_comentarios->resizeColumnToContents(2);
                    ui->table_comentarios->resizeColumnToContents(0);
                    ui->table_comentarios->resizeColumnToContents(1);
                }

                //formatação de nomes,triggers e css
                QStringList cabecalho = {"Data/Hora", "Autor", "Comentário"};
                ui->table_comentarios->setHorizontalHeaderLabels(cabecalho);
                ui->table_comentarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
                ui->table_comentarios->setSelectionBehavior(QAbstractItemView::SelectRows);
                ui->table_comentarios->verticalHeader()->setVisible(false);
                ui->table_comentarios->setColumnHidden(3, true);
                ui->table_comentarios->setStyleSheet("QTableView {selection-background-color: grey;}");

            }
            this->refresh_modificacoes();
        }
    }
}

