import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    //    Terminal {
    //        id: terminal
    //    }
    Row {
        id: row
        x: 10
        y: 10
        width: 307
        height: 39
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
            //model: terminal.portList
        }

        Button {
            id: btnOpenPort
            width: 100
            height: 30
            text: qsTr("Open Port")
            //            onClicked: {
            //                if (btnOpenPort.text === "Open Port") {
            //                    var res = terminal.openPort(cbPorts.currentValue)
            //                    if (res) {
            //                        console.log("PORT " + cbPorts.currentValue + " Opend")
            //                        btnOpenPort.text = "Close Port"
            //                    } else {
            //                        console.log("Could't open " + cbPorts.currentValue)
            //                    }
            //                } else {
            //                    terminal.closePort()
            //                    btnOpenPort.text = "Open Port"
            //                    console.log("Port Closed!")
            //                }
            //            }
        }
    }

    Button {
        id: buttbtnSend
        x: 422
        y: 112
        width: 157
        height: 52
        text: qsTr("Button")
        onClicked: {

            bl.test("SSS")
        }

        //        onClicked: {
        //            //terminal.sendData("40\n")
        //            var data = terminal.getInfo()
        //            console.log("BL_Version :" + data["BL_Version"])
        //            console.log("MCU_CID :" + data["MCU_CID"])
        //            console.log("MCU_RDB : " + data["MCU_RDB"])
        //            console.log("MCU_FLASH_SIZE :" + data["MCU_FLASH_SIZE"])

        //            for (var prop in data) {
        //                console.log("Object item:", prop, "=", data[prop])
        //            }

        //            textFieldblVersion.text = data["BL_Version"]
        //            textFieldChipId.text = data["MCU_CID"]
        //            textFieldProtectionLevel.text = data["MCU_RDB"]
        //            textFieldFlashSize.text = data["MCU_FLASH_SIZE"]
        //        }
    }

    Grid {
        id: gridData
        x: 10
        y: 61
        width: 232
        height: 172
        spacing: 20
        rows: 4
        columns: 2

        Label {
            id: lblBootLoaderVersion
            width: 150
            height: 28
            text: qsTr("Boot loader Version")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 12
        }

        TextField {
            id: textFieldblVersion
            width: 50
            height: 28
            visible: true
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Bold
            font.pointSize: 12
            layer.enabled: false
            activeFocusOnTab: false
            placeholderText: qsTr("-")
            readOnly: true
        }

        Label {
            id: lblChipId
            width: 150
            height: 28
            text: qsTr("Chip ID")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 12
        }

        TextField {
            id: textFieldChipId
            width: 50
            height: 28
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Bold
            font.pointSize: 12
            placeholderText: qsTr("-")
            readOnly: true
        }

        Label {
            id: lblProtectionLevel
            width: 150
            height: 28
            text: qsTr("Protection Level")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 12
        }

        TextField {
            id: textFieldProtectionLevel
            width: 50
            height: 28
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Bold
            font.pointSize: 12
            placeholderText: qsTr("-")
            readOnly: true
        }

        Label {
            id: lblFlashSize
            width: 150
            height: 28
            text: qsTr("Flash Size(KB)")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 12
        }

        TextField {
            id: textFieldFlashSize
            width: 50
            height: 28
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Bold
            font.pointSize: 12
            placeholderText: qsTr("-")
            readOnly: true
        }
    }

    Component.onCompleted: {

        //console.log(terminal.portList)
    }
    Connections {
        target: bootloader

        id: bl
    }
}
