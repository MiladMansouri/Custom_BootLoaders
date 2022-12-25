import QtQuick
import com.milad.terminal

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Terminal {
        id: terminal
    }
    Component.onCompleted: {
        console.log(terminal.portList)
    }
}
