#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <QWidget>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtSql>
#include <usuario.h>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QByteArray>
#include <QVariant>
#include <QCompleter>
#include <QFileSystemModel>
#include "busca_bug.h"
#include <QTableWidgetItem>
#include "aba_de_comentario.h"
#include "modificar_dados.h"
#include "modificar_senha.h"
#include <QNetworkAccessManager>
#include <QtNetwork/QtNetwork>
#include <QtNetwork>
#include <algorithm>
#include <iostream>

namespace Ui {
class Gerenciador;
}

class Gerenciador : public QWidget
{
    Q_OBJECT

public:
    explicit Gerenciador(QWidget *parent = nullptr);
    ~Gerenciador();

    void atualizaLogado(QSqlQuery query);
    void atualizaTemporario(QString nome);
    void atualizaContagem(); // atualiza a contagem de ocorrencias para cada tipo de ticket
    void atualizaTickets(); //atualiza os tickets na aba inicial do programa
    void iniciaTimer(); //inicia o temporizador para a query de busca de eventos
    bool checar_conexao(); //verifica se está conectado a internet

private:
    Ui::Gerenciador *ui;

    //login e informações
    Usuario usuario_conectado;
    Usuario temporario_designado;
    int count_baixo = 0;
    int count_medio = 0;
    int count_alto = 0;
    QString pepper_ger = ""; //inserir valor de pepper

    //Bugs
    Busca_Bug _bug;

    //Salvar arquivo
    QFileInfo info;
    QFile arquivo;
    QString nomeArquivo;
    QByteArray dados;
    QVector<QString> datas;

    //enviar comentarios
    Aba_De_Comentario aba_comentario;

    //editar tickets
    QSqlQuery abrir_ticket_edit;

    //QSqlQuery edit_desenvolvedores;
    QString var1;
    QString var2;
    QString var3;

    //dados para selecao de desenvolvedores em determinado projeto
    int projeto_selecionado;
    int id;
    QVector<int> ids_programadores;

    //Aba gerenciar projetos
    QVector<QString> nome_desenvolvedores;
    QVector<int> ids_projetos;

    //Editar dados de usuário
    Modificar_Dados configuracoes;
    Modificar_Senha definir_senha;

    //Query de eventos no database
    QTimer *tempo_sessao;
    QTime *tempo_query = new QTime(0,0,0);

    //Editar projetos
    QString manager;
    QSqlQuery encontra_manager;

    void resetaContagem();
    void limpaCache_ticket();
    void refresh_comentarios();
    void registros_ProgBug(); //registra na tabela programadores_bugs relacao N to N
    void registros_ProjBug(QString nome_do_projeto); //registra na table bugs_projetos relacao N to N
    void resetar_variaveis();
    void refresh_modificacoes();
    bool checa_emailExistente(QString email_ex);
    void busca_evento(); //procura novos registros no DB e envia mensagem para o usuario durante o uso ou ao logar no programa
    void esconde_buttons(); //esconde os botoes principais do menu

signals:
    void home_buttonClicked();
    void combo_changed();

private slots:
    void on_button_desconectar_clicked();
    void on_button_enviatkt_clicked();
    void on_button_addfile_clicked();
    void on_button_projetos_clicked();
    void on_button_novo_clicked();
    void on_combo_projeto_currentTextChanged(const QString &arg1);
    void on_combo_desenvolvedor_currentTextChanged(const QString &arg1);
    void on_lista_ultimasreq_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_download_clicked();
    void on_button_addcoment_clicked();

    void envio_de_comentario();
    void atualiza_temporizador();
    void envio_de_modificacoes();
    void envio_de_nova_senha();
    void on_button_tickets_clicked();
    void on_lista_tickets_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_solved_clicked();
    void on_combo_prioridadeedit_currentTextChanged(const QString &arg1);
    void on_combo_statusedit_currentTextChanged(const QString &arg1);
    void on_button_editarticket_clicked();
    void on_button_enviaredit_clicked();
    void on_combo_desenvolvedoredit_currentTextChanged(const QString &arg1);
    void on_button_regprojeto_clicked();
    void on_button_enviarreg_clicked();
    void on_button_gerenciador_projetos_clicked();
    void on_button_adddev_clicked();
    void on_button_gerenviar_clicked();
    void on_button_enviacademp_clicked();
    void on_line_procurarempresa_textChanged(const QString &arg1);
    void on_button_registrarempdev_clicked();
    void on_button_funcionarios_clicked();
    void on_table_funcionarios_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_desligarfunc_clicked();

    void on_button_promoverfunc_clicked();
    void on_button_meusprojetos_clicked();
    void on_table_resolvidos_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_enviarpedido_clicked();
    void on_button_geraplic_clicked();
    void on_table_aplicacoes_itemDoubleClicked(QTableWidgetItem *item);
    void on_table_meusprojetos_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_projdel_clicked();
    void on_button_config_clicked();
    void on_button_dadosempresa_clicked();
    void on_button_dadosmod_clicked();
    void on_button_dadossenha_clicked();
    void on_button_dadoscadastrar_clicked();
    void on_combo_projmanager_currentTextChanged(const QString &arg1);
    void on_button_tickets_equipe_clicked();
    void on_button_solved_equipe_clicked();
    void on_lista_tickets_equipe_itemDoubleClicked(QTableWidgetItem *item);
    void on_lista_resolvidos_equipe_itemDoubleClicked(QTableWidgetItem *item);
};

#endif // GERENCIADOR_H
