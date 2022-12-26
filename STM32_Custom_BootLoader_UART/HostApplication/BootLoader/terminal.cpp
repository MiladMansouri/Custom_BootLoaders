#include "terminal.h"

Terminal::Terminal(QObject *parent) : QObject{parent} {
  auto portList = QSerialPortInfo::availablePorts();

  for (auto &com : portList) {
    qInfo() << com.portName();
    _portList.append(com.portName());
  }
}

QList<QString> Terminal::portList() const { return _portList; }

bool Terminal::openPort(QString name) {
  this->_serial.setPortName(name);
  return this->_serial.open(QIODeviceBase::ReadWrite);
}

bool Terminal::sendData(QString data) {
  bool res = false;
  // this->_serial.write(data);
  return res;
}
