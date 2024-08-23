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
        if (!speedUpdateManager) {
            console.error("Failed to load SpeedUpdateManager");
        }

        if (!speedUpdateManager.getFilter()) {
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
            target: speedUpdateManager
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
                if (speedUpdateManager.getFilter().getIsFilterOn()) {
                    speedUpdateManager.getFilter().setIsFilterOn(false);
                    filterOnOffButton.text = "Filter OFF";
                    filterOnOffButton.background.color = "grey";
                    filterOnOffButton.font.color = "white";
                } else {
                    speedUpdateManager.getFilter().setIsFilterOn(true);
                    filterOnOffButton.text = "Filter ON";
                    filterOnOffButton.background.color = "white";
                    filterOnOffButton.font.color = "black";
                }
            }
        }
    }
}
