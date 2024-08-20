import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import InstrumentCluster 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 400
    title: "Instrument Cluster"

    Component.onCompleted: {
        if (!canReceiver) {
            console.error("Failed to load CanReceiver");
        }

        if (!canReceiver.getFilter()) {
            console.error("Failed to load Filter");
        }
    }

    Rectangle {
        width: 1280
        height: 400
        color: "black"

        Speedometer {
            id: speedometer
            anchors.centerIn: parent
            speed: 0
        }

        Connections {
            target: canReceiver
            onSpeedUpdated: speed => {
                speedometer.speed = speed
            }
        }

        Text {
            id: batteryLevelText
            text: "Battery: " + batteryChecker.batteryLevel + "%"
            color: "white"
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.rightMargin: 10
            font.pixelSize: 20
        }

        Button {
            id: filterOnOffButton
            text: "Filter OFF"
            width: 120
            height: 40
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.top: parent.top
            anchors.topMargin: 25
            background: Rectangle {
                color: "grey"
                radius: 5
            }
            onClicked: {
                if (canReceiver.getFilter().getIsFilterOn()) {
                    canReceiver.getFilter().setIsFilterOn(false);
                    filterOnOffButton.text = "Filter OFF";
                    filterOnOffButton.background.color = "grey";
                    filterOnOffButton.font.color = "white";
                } else {
                    canReceiver.getFilter().setIsFilterOn(true);
                    filterOnOffButton.text = "Filter ON";
                    filterOnOffButton.background.color = "white";
                    filterOnOffButton.font.color = "black";
                }
            }
        }
    }
}
