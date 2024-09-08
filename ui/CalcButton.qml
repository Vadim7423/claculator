import QtQuick
import QtQuick.Controls

Button {
    id: root
    property string title
    property color bgColor: "#e7e5f2"
    property var timer: Timer {
        interval: 500
        onTriggered: root.enabled = true
    }

    background: Rectangle {
        color: root.pressed ? "#5e5c68" : root.bgColor
        border {
            width: 2
            color: root.pressed ? "#5e5c68" : "#dedde4"
        }
        radius: 8
    }
    Text {
        anchors.centerIn: parent
        text: root.title
        color: root.pressed ? "white" : "black"
        font: Qt.font({
                          family: "Inter",
                          pixelSize: 16,
                          letterSpacing: 0,
                          weight: Font.Normal
                      })
    }

}
