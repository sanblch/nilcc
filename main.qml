import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtCharts 2.15
import QtQuick.VirtualKeyboard 2.15

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    signal paste()

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }

        Menu {
            id: contextMenu
            MenuItem {
                text: "Paste"
                onClicked: root.paste()
            }
        }
    }

    TableView {
        id: matrix
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 320
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: tablemodel

        delegate: Rectangle {
            implicitWidth: 50
            implicitHeight: 25
            Text {
                text: display
            }
        }

    }

    ChartView {
        id: chart
        objectName: "chart"
        title: "Concentration"
        anchors.left: matrix.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 320
    }
}
