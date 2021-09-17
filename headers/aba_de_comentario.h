#ifndef ABA_DE_COMENTARIO_H
#define ABA_DE_COMENTARIO_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class Aba_De_Comentario;
}

class Aba_De_Comentario : public QWidget
{
    Q_OBJECT

public:
    explicit Aba_De_Comentario(QWidget *parent = nullptr);
    ~Aba_De_Comentario();
    QString getTexto() {return texto;}
private slots:
    void on_button_cancelacoment_clicked();

    void on_button_enviacoment_clicked();

    void on_text_comentario_textChanged();

signals:
    void enviar_comentario();


private:
    Ui::Aba_De_Comentario *ui;
    QString texto;
    QString tracker;
    int contagem;
};

#endif // ABA_DE_COMENTARIO_H
