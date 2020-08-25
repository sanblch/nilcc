import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.VirtualKeyboard 2.15

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TextEdit {
        id: edit
        anchors.left: window.left
        anchors.top: window.bottom
        anchors.topMargin: 24
        width: 520
        height: 30
        font.family: "Roboto"
        font.pixelSize: 20
    }
}
