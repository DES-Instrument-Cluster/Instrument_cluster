import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: speedometer
    width: 300
    height: 300

    property real speed: 0
    property real animatedSpeed: 0

    // Numeric speed
    readonly property string speedFont: "bold 30px sans-serif"
    readonly property string unitFont: "13px sans-serif"

    // Speed gauge
    readonly property real gaugeStartAngle: 150
    property var gradient: null

    onSpeedChanged: {
        speedAnimation.to = speed;
        speedAnimation.start();
    }

    NumberAnimation {
        id: speedAnimation
        target: speedometer
        property: "animatedSpeed"
        duration: 100 // ms
        easing.type: Easing.InOutQuad
        onRunningChanged: {
            if (running) {
                updateTimer.start();
            } else {
                updateTimer.stop();
                foregroundCanvas.requestPaint();
            }
        }
    }

    Timer {
        id: updateTimer
        interval: 16  // 60fps
        repeat: true
        running: false
        onTriggered: foregroundCanvas.requestPaint()
    }

    Canvas {
        id: backgroundCanvas
        anchors.fill: parent
        onPaint: {
            let ctx = getContext("2d");
            ctx.reset();
            ctx.translate(width / 2, height / 2);
            ctx.clearRect(-width / 2, -height / 2, width, height);

            // drawBackground(ctx);
            drawScale(ctx);
        }
    }

    function drawBackground(ctx) {
        ctx.beginPath();
        ctx.arc(0, 0, 140, 0, 2 * Math.PI);
        ctx.fillStyle = "black";
        ctx.fill();
    }

    function drawScale(ctx) {
        ctx.strokeStyle = "white";
        for (let i = 0; i <= 240; i += 4) {
            ctx.save();
            ctx.rotate((240 + i) * Math.PI / 180);
            if (i % 40 === 0) {
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.moveTo(0, -125);
                ctx.lineTo(0, -140);
                ctx.stroke();
                ctx.translate(0, -107);
            } else if (i % 20 === 0) {
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(0, -130);
                ctx.lineTo(0, -140);
                ctx.stroke();
            } else {
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(0, -135);
                ctx.lineTo(0, -140);
                ctx.stroke();
            }
            ctx.restore();
        }
    }

    Canvas {
        id: foregroundCanvas
        anchors.fill: parent
        onAvailableChanged: {
            if (available) {
                let ctx = getContext("2d");
                gradient = ctx.createLinearGradient(-150, 0, 150, 0);
                gradient.addColorStop(0, "skyblue");  // Start color
                gradient.addColorStop(1, "purple");   // End color
                requestPaint();
            }
        }
        onPaint: {
            let ctx = getContext("2d");
            ctx.reset();
            ctx.translate(width / 2, height / 2);

            drawSpeedGauge(ctx);
            drawNumericText(ctx);
        }
    }

    function drawSpeedGauge(ctx) {
        if (animatedSpeed > 0) {
            let gaugeEndAngle = gaugeStartAngle + animatedSpeed * 0.8;
            ctx.strokeStyle = gradient;
            ctx.lineWidth = 10;
            ctx.beginPath();
            ctx.arc(0, 0, 110, gaugeStartAngle * Math.PI / 180, gaugeEndAngle * Math.PI / 180);
            ctx.stroke();
        }
    }

    function drawNumericText(ctx) {
        ctx.font = speedFont;
        ctx.fillStyle = "white";
        let speedText = Math.round(animatedSpeed).toString();
        let textMetrics = ctx.measureText(speedText);
        ctx.fillText(speedText, -textMetrics.width / 2, 0);

        ctx.font = unitFont;
        textMetrics = ctx.measureText("cm/s");
        ctx.fillText("cm/s", -textMetrics.width / 2, 20);
    }
}
