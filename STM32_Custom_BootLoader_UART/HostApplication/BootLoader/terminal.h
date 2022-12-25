#ifndef TERMINAL_H
#define TERMINAL_H

#include <QDebug>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Terminal : public QObject {
  Q_OBJECT
  Q_PROPERTY(QList<QString> portList READ portList NOTIFY portListChanged)
public:
  explicit Terminal(QObject *parent = nullptr);

  QList<QString> portList() const;

signals:

  void portListChanged();

private:
  QSerialPort _serial;
  QList<QString> _portList;
};

#endif // TERMINAL_H
