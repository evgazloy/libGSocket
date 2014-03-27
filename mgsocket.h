#ifndef MGSOCKET_H
#define MGSOCKET_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QSslSocket>
#include <QSslError>
#include <QTimerEvent>

#if defined(LIBGSOCKET_LIBRARY)
#  define LIBGSOCKETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBGSOCKETSHARED_EXPORT Q_DECL_IMPORT
#endif

#define SOCKET_TIMEOUT 2000

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
    QAbstractSocket::SocketState getState();

private:
    QSslSocket *m_socket = 0;
    quint8 m_type;
    QString m_hostName;
    quint16 m_port;
    quint32 m_dataSize = 0;
    QByteArray m_inBuffer;

    void inData();
    void timerEvent(QTimerEvent *ev);

signals:
    void sig_ready();
    void sig_socketError(QAbstractSocket::SocketError error);
    void sig_inCmd(QByteArray data);

public slots:
    void sslError(QList<QSslError> list);
    void socketError(QAbstractSocket::SocketError err);
    void encrypted();
};
#endif // MGSOCKET_H
