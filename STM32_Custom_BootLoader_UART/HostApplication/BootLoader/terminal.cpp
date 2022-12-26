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
  this->_serial.setBaudRate(QSerialPort::Baud115200);
  this->_serial.setDataBits(QSerialPort::Data8);
  this->_serial.setParity(QSerialPort::NoParity);
  this->_serial.setFlowControl(QSerialPort::NoFlowControl);
  return this->_serial.open(QIODeviceBase::ReadWrite);
}

void Terminal::closePort(void) { this->_serial.close(); }

bool Terminal::sendData(QString data) {
  bool res = true;
  const QChar *ch = data.constData();
  while (!ch->isNull()) {
    if (this->_serial.write((char *)ch)) {
      qInfo() << ch->unicode();
      ch++;
    } else {
      res = false;
    }
  }
  return res;
}
