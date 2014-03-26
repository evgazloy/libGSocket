#ifndef MGSOCKET_H
#define MGSOCKET_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QSslSocket>
#include <QSslError>

#if defined(LIBGSOCKET_LIBRARY)
#  define LIBGSOCKETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBGSOCKETSHARED_EXPORT Q_DECL_IMPORT
#endif

class LIBGSOCKETSHARED_EXPORT MGSocket : public QObject
{
    Q_OBJECT
public:
    MGSocket(quint8 type);
    ~MGSocket();

    void initSocket();
    void setCertificate(QString file);
    void createConnection(QString hostName, quint16 port);
    void send(const QByteArray &data);

private:
    QSslSocket *m_socket = 0;
    quint8 m_type;

    void inData();

signals:
    void sig_ready();
    void sig_socketError(QAbstractSocket::SocketError error);

public slots:
    void sslError(QList<QSslError> list);
    void socketError(QAbstractSocket::SocketError err);
    void encrypted();
};
#endif // MGSOCKET_H
