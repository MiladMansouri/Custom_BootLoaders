#include "terminal.h"

Terminal::Terminal(QObject *parent) : QObject{parent} {
  auto portList = QSerialPortInfo::availablePorts();

  for (auto &com : portList) {
    qInfo() << com.portName();
    _portList.append(com.portName());
  }
  QObject::connect(&_serial, &QSerialPort::readyRead, this,
                   &Terminal::readTerminalData);
}

QList<QString> Terminal::portList() const { return _portList; }

bool Terminal::openPort(QString name) {
  this->_serial.setPortName(name);
  this->_serial.setBaudRate(QSerialPort::Baud115200);
  this->_serial.setDataBits(QSerialPort::Data8);
  this->_serial.setParity(QSerialPort::NoParity);
  this->_serial.setFlowControl(QSerialPort::NoFlowControl);
  this->_serial.open(QIODeviceBase::ReadWrite);

  return true;
}

void Terminal::closePort(void) { this->_serial.close(); }

bool Terminal::sendData(QString data) {
  bool res = true;
  //  const QChar *ch = data.constData();
  //  while (!ch->isNull()) {
  //    if (this->_serial.write((char *)ch)) {
  //      qInfo() << ch->unicode();
  //      ch++;
  //    } else {
  //      res = false;
  //    }
  //  }

  return res;
}

QByteArray Terminal::readTerminalData() {
  qDebug() << "New data available: " << _serial.bytesAvailable();
  recivebuffer = _serial.readAll();
  qDebug() << recivebuffer;
  return recivebuffer;
}

QVariantMap Terminal::getInfo() {
  QVariantMap data;
  QByteArray buffer;
  // bl version
  buffer.clear();
  buffer.append((char)0x20);
  buffer.append((char)0x0a);
  _serial.write(buffer);
  _serial.waitForBytesWritten();
  _serial.waitForReadyRead(100);
  qInfo() << recivebuffer[0];
  if (recivebuffer[0] == 0x60) {
    data.insert("BL_Version", abs(recivebuffer[1]));
  } else {
    data.insert("BL_Version", -1);
  }
  /*********************************************/
  // mcu cid
  buffer[0] = (char)0x22;
  buffer[1] = (char)0x0a;
  _serial.write(buffer);
  _serial.waitForBytesWritten();
  _serial.waitForReadyRead(100);
  if (recivebuffer[0] == 0x62) {
    qInfo() << recivebuffer[1] << recivebuffer[2];
    data.insert("MCU_CID", abs((recivebuffer[1] << 8) | (recivebuffer[2])));
  } else {
    data.insert("MCU_CID", -1);
  }
  /*********************************************/
  // mcu Write Protection Level
  buffer[0] = (char)0x23;
  buffer[1] = (char)0x0a;
  _serial.write(buffer);
  _serial.waitForBytesWritten();
  _serial.waitForReadyRead(100);
  if (recivebuffer[0] == 0x63) {
    qInfo() << recivebuffer[1] << recivebuffer[2];
    data.insert("MCU_RDB", abs(recivebuffer[1]));
  } else {
    data.insert("MCU_RDB", -1);
  }
  /*********************************************/
  // mcu Write Protection Level
  buffer[0] = (char)0x24;
  buffer[1] = (char)0x0a;
  _serial.write(buffer);
  _serial.waitForBytesWritten();
  _serial.waitForReadyRead(100);
  if (recivebuffer[0] == 0x64) {
    qInfo() << recivebuffer[1] << recivebuffer[2];
    data.insert("MCU_FLASH_SIZE",
                abs((recivebuffer[1] << 8) | (recivebuffer[2])));
  } else {
    data.insert("MCU_FLASH_SIZE", -1);
  }
  return data;
}
