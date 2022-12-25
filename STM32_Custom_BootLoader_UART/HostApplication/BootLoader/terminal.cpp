#include "terminal.h"

Terminal::Terminal(QObject *parent) : QObject{parent} {
  auto portList = QSerialPortInfo::availablePorts();

  for (auto &com : portList) {
    qInfo() << com.portName();
    _portList.append(com.portName());
  }
}

QList<QString> Terminal::portList() const { return _portList; }
