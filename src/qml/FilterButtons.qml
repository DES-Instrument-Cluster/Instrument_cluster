import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: filterButtons
    width: 120
    height: 200

    function updateFilterState(button, filterType) {
        if (button.isOn) {
            filterManager.removeFilter(filterType);
            button.isOn = false;
            button.background.color = "grey";
            button.font.color = "white";
        } else {
            filterManager.addFilter(filterType);
            button.isOn = true;
            button.background.color = "white";
            button.font.color = "black";
        }
    }

    Component {
            id: filterButtonComponent
            Item {
                width: filterButtons.width
                height: 45

                Button {
                    anchors.fill: parent
                    text: modelData.text
                    property bool isOn: false
                    property int filterType: modelData.filterType
                    background: Rectangle {
                        color: "grey"
                        radius: 5
                    }
                    onClicked: updateFilterState(this, filterType)
                }
            }
        }

    Column {
        spacing: 10
        Repeater {
            model: [
                { text: "SMA", filterType: filterManager.SMA },
                { text: "EMA", filterType: filterManager.EMA },
                { text: "Kalman", filterType: filterManager.KALMAN }
            ]
            delegate: filterButtonComponent
        }
    }
}
