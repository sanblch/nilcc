import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.15
import QtQuick.VirtualKeyboard 2.15

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

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
         title: "NHL All-Star Team Players"
         anchors.left: matrix.right
         anchors.right: parent.right
         anchors.top: parent.top
         anchors.bottom: parent.bottom
         width: 320

         // Define x-axis to be used with the series instead of default one
         ValueAxis {
             id: valueAxis
             min: 2000
             max: 2011
             tickCount: 12
             labelFormat: "%.0f"
         }

         AreaSeries {
             name: "Russian"
             axisX: valueAxis
             upperSeries: LineSeries {
                 XYPoint { x: 2000; y: 1 }
                 XYPoint { x: 2001; y: 1 }
                 XYPoint { x: 2002; y: 1 }
                 XYPoint { x: 2003; y: 1 }
                 XYPoint { x: 2004; y: 1 }
                 XYPoint { x: 2005; y: 0 }
                 XYPoint { x: 2006; y: 1 }
                 XYPoint { x: 2007; y: 1 }
                 XYPoint { x: 2008; y: 4 }
                 XYPoint { x: 2009; y: 3 }
                 XYPoint { x: 2010; y: 2 }
                 XYPoint { x: 2011; y: 1 }
             }
         }
     }
}
