import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtCharts 2.15

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480

    signal paste()

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Edit")
            Action {
                text: qsTr("&Paste")
                shortcut: StandardKey.Paste
                onTriggered: root.paste()
            }
        }
    }

    TableView {
        id: matrix
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 15
        height: 370
        width: 320
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: tablemodel

        delegate: Rectangle {
            implicitWidth: 30
            implicitHeight: 15
            TextInput {
                text: display
                onAccepted: {
                    tablemodel.setData(row, column, displayText)
                }
            }
        }

    }

    TableView {
        id: conc
        y: 100
        anchors.left: matrix.left
        anchors.top: matrix.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        height: 30
        width: 320
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: concmodel

        delegate: Rectangle {
            implicitWidth: 30
            implicitHeight: 15
            TextInput {
                text: display
                onAccepted: {
                    concmodel.setData(row, column, displayText)
                }
            }
        }

    }

    function refresh() {
        console.log("Refresh")
        var data = backend.result()
        var dim = backend.dim()
        var x = backend.x()
        yAxis.max = Math.max(...data)
        chart.removeAllSeries()
        for(var i = 0; i < dim[1]; i++) {
            var series = chart.createSeries(ChartView.SeriesTypeLine, "s"+ i, xAxis, yAxis);
            series.pointsVisible = true;
            series.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
            series.hovered.connect(function(point, state) {
                var p = chart.mapToPosition(point);
                tooltip.x = p.x
                tooltip.y = p.y - tooltip.height
                tooltip.text = qsTr("%1 { pH: %2, c: %3 }").arg(series.name).arg(point.x).arg(point.y)
                tooltip.visible = true
            })
            for(var j = 0; j < dim[0]; j++)
            {
                series.append(x[j], data[j + i * dim[0]]);
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
        axes: [
            ValueAxis{
                id: xAxis
                min: 1.0
                max: 14.0
            },
            ValueAxis{
                id: yAxis
                min: 0.0
                max: 1.0
            }]

        ToolTip {
            id: tooltip
            contentItem: Text{
                color: "#21be2b"
                text: tooltip.text
            }
            background: Rectangle {
                border.color: "#21be2b"
            }
        }
    }
}
