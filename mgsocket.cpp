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
    connect(m_socket, &QSslSocket::readyRead, this, &MGSocket::inData);
}

void MGSocket::setCertificate(QString file)
{
    Q_CHECK_PTR(m_socket);
    m_socket->addCaCertificates(file);
}

void MGSocket::createConnection(QString hostName, quint16 port)
{
    Q_CHECK_PTR(m_socket);
    m_hostName = hostName;
    m_port = port;
    m_socket->connectToHostEncrypted(hostName, port);
}

void MGSocket::inData()
{
    Q_CHECK_PTR(m_socket);
    qDebug()<<"rcv"<<m_socket->bytesAvailable();
}

void MGSocket::timerEvent(QTimerEvent *ev)
{
    Q_CHECK_PTR(m_socket);
    if(m_socket->state()==QAbstractSocket::UnconnectedState)
        m_socket->connectToHostEncrypted(m_hostName, m_port);
    else
        this->killTimer(ev->timerId());
}

void MGSocket::sslError(QList<QSslError> list)
{
    foreach (QSslError err, list) {
       qDebug()<<"SSL Error:"<<err.errorString();
    }
}

void MGSocket::socketError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    Q_CHECK_PTR(m_socket);
    qDebug()<<"Socket Error:"<<m_socket->errorString();
    this->startTimer(SOCKET_TIMEOUT);
}

void MGSocket::encrypted()
{
    qDebug()<<"Connection encrypted";
}

void MGSocket::send(const QByteArray &data)
{
    Q_CHECK_PTR(m_socket);
    QByteArray buf;
    QDataStream out(&buf, QIODevice::ReadWrite);
    out<<data;
    m_socket->write(buf);
}

QAbstractSocket::SocketState MGSocket::getState()
{
    Q_CHECK_PTR(m_socket);
    return m_socket->state();
}

MGSocket::~MGSocket()
{
    delete m_socket;
}
