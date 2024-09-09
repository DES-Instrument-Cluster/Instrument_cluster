import QtQuick 2.15

Item {
    id: batteryIndicator
    width: 120
    height: 40

    property bool isCharging: false
    property real batteryPercentage: 0

    Rectangle {
        id: batteryBar
        width: batteryIndicator.width
        height: batteryIndicator.height
        color: "gray"

        Rectangle {
            id: batteryFill
            width: batteryPercentage / 100 * (parent.width - 6)
            height: parent.height - 6
            color: batteryPercentage > 20 ? "lime" : "orangered"
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
        id: batteryPercentText
        anchors.centerIn: parent
        text: isCharging ? "Charging..." : batteryPercentage + '%'
        color: "white"
        font.bold: true
    }
}
