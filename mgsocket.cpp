#include "mgsocket.h"
#include <QDebug>

MGSocket::MGSocket(quint8 type) :
    QObject(0), m_type(type)
{
}

void MGSocket::initSocket()
{
    m_socket = new QSslSocket(this);
    Q_CHECK_PTR(m_socket);
    connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslError(QList<QSslError>)));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(encrypted()), SLOT(encrypted()));
    connect(m_socket, &QSslSocket::encrypted, this, &MGSocket::sig_ready);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SIGNAL(sig_socketError(QAbstractSocket::SocketError)));
}

void MGSocket::setCertificate(QString file)
{
    Q_CHECK_PTR(m_socket);
    m_socket->addCaCertificates(file);
}

void MGSocket::createConnection(QString hostName, quint16 port)
{
    Q_CHECK_PTR(m_socket);
    m_socket->connectToHostEncrypted(hostName, port);
}

void MGSocket::sslError(QList<QSslError> list)
{
    foreach (QSslError err, list) {
       qDebug()<<"SSL Error:"<<err.errorString();
    }
}

void MGSocket::socketError(QAbstractSocket::SocketError err)
{
    qDebug()<<"Socket Error:"<<err;
}

void MGSocket::encrypted()
{
    qDebug()<<"Connection encrypted";
}

MGSocket::~MGSocket()
{
    delete m_socket;
}
