import QtQuick 2.15

Item {
    id: batteryIndicator
    width: 120
    height: 40

    Rectangle {
        id: batteryBar
        width: batteryIndicator.width
        height: batteryIndicator.height
        color: "gray"

        Rectangle {
            id: batteryFill
            width: batteryChecker.batteryLevel / 100 * (parent.width - 6)
            height: parent.height - 6
            color: batteryChecker.batteryLevel > 20 ? "lime" : "orangered"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 3
        }

        Rectangle {
            width: 10
            height: 20
            color: "gray"
            anchors.left: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Text {
        id: batteryLevelText
        anchors.centerIn: parent
        text: batteryChecker.batteryLevel + '%'
        color: "white"
        font.bold: true
    }
}
