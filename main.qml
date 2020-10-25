import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtCharts 2.15

Rectangle {
    id: root
    visible: true
    width: 640
    height: 480

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

    function refresh() {
        console.log("Refresh")
        var data = backend.result()
        var dim = backend.dim()
        var x = backend.x()
        yAxis.max = Math.max(...data)
        for(var i = 0; i < dim[1]; i++) {
            var series = chart.createSeries(ChartView.SeriesTypeLine, "line"+ i, xAxis, yAxis);
            series.pointsVisible = true;
            series.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
            series.hovered.connect(function(point, state){ console.log(point); }); // connect onHovered signal to a function
            for(var j = 0; j < dim[0]; j++)
            {
                series.append(x[j], data[j + i * dim[0]]);
            }
        }
    }

    ChartView {
        id: chart
        objectName: "chart"
        title: "Concentrations"
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
    }
}
