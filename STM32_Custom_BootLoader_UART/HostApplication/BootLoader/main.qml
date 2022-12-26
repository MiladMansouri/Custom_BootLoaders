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
            text: qsTr("Button")
            onClicked: {
                console.log(cbPorts.currentValue)
                var res = terminal.openPort(cbPorts.currentValue)
                if (res)
                    console.log("PORT " + cbPorts.currentValue + " Opend")
                else
                    console.log("Could't open " + cbPorts.currentValue)
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

        }
    }
    Component.onCompleted: {
        console.log(terminal.portList)
    }
}
