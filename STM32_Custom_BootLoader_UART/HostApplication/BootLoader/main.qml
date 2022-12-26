import QtQuick
import QtQuick.Controls
import com.milad.terminal

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Terminal {
        id: terminal
    }

    Row {
        id: row
        x: 150
        y: 62
        width: 340
        height: 40
        spacing: 15

        Label {
            id: lblPorts
            width: 80
            height: 30
            text: qsTr("Ports")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.underline: true
            font.pointSize: 15
        }

        ComboBox {
            id: cbPorts
            width: 100
            height: 30
            model: terminal.portList
        }

        Button {
            id: btnOpenPort
            width: 100
            height: 30
            text: qsTr("Open Port")
            onClicked: {
                if (btnOpenPort.text === "Open Port") {
                    var res = terminal.openPort(cbPorts.currentValue)
                    if (res) {
                        console.log("PORT " + cbPorts.currentValue + " Opend")
                        btnOpenPort.text = "Close Port"
                    } else {
                        console.log("Could't open " + cbPorts.currentValue)
                    }
                } else {
                    terminal.closePort()
                    btnOpenPort.text = "Open Port"
                    console.log("Port Closed!")
                }
            }
        }
    }

    Button {
        id: buttbtnSend
        x: 248
        y: 255
        width: 157
        height: 52
        text: qsTr("Button")
        onClicked: {
            terminal.sendData("10")
        }
    }
    Component.onCompleted: {
        console.log(terminal.portList)
    }
}
