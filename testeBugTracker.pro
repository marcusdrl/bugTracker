QT       += core gui sql network qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += SMTP_BUILD
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aba_de_comentario.cpp \
    busca_bug.cpp \
    emailaddress.cpp \
    formulario_de_cadastro.cpp \
    gerenciador.cpp \
    main.cpp \
    login.cpp \
    mimeattachment.cpp \
    mimecontentformatter.cpp \
    mimefile.cpp \
    mimehtml.cpp \
    mimeinlinefile.cpp \
    mimemessage.cpp \
    mimemultipart.cpp \
    mimepart.cpp \
    mimetext.cpp \
    modificar_dados.cpp \
    modificar_senha.cpp \
    quotedprintable.cpp \
    recuperar_senha.cpp \
    smtpclient.cpp \
    usuario.cpp

HEADERS += \
    SmtpMime \
    aba_de_comentario.h \
    busca_bug.h \
    emailaddress.h \
    formulario_de_cadastro.h \
    gerenciador.h \
    login.h \
    mimeattachment.h \
    mimecontentformatter.h \
    mimefile.h \
    mimehtml.h \
    mimeinlinefile.h \
    mimemessage.h \
    mimemultipart.h \
    mimepart.h \
    mimetext.h \
    modificar_dados.h \
    modificar_senha.h \
    quotedprintable.h \
    recuperar_senha.h \
    smtpclient.h \
    smtpexports.h \
    usuario.h

FORMS += \
    aba_de_comentario.ui \
    formulario_de_cadastro.ui \
    gerenciador.ui \
    login.ui \
    modificar_dados.ui \
    modificar_senha.ui \
    recuperar_senha.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ArquivosDeRecurso.qrc

DISTFILES += \
    C:/Users/Marcus Vinicius/Downloads/bug_img.png

