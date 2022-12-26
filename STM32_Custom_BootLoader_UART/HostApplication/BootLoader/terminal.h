#ifndef TERMINAL_H
#define TERMINAL_H

#include <QDebug>
#include <QIODevice>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Terminal : public QObject {
  Q_OBJECT
  Q_PROPERTY(QList<QString> portList READ portList NOTIFY portListChanged)
public:
  explicit Terminal(QObject *parent = nullptr);

  QList<QString> portList() const;
  Q_INVOKABLE bool openPort(QString name);
  Q_INVOKABLE void closePort(void);
  Q_INVOKABLE bool sendData(QString data);
signals:

  void portListChanged();

private:
  QSerialPort _serial;
  QList<QString> _portList;
};

#endif // TERMINAL_H
