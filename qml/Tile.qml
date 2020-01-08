import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import QtQuick.Controls.Material 2.12

Item{
    id: tile
    Material.accent: Material.Blue
    width: parent.width
    height: parent.height
    property bool stateVisible: false
    visible: stateVisible
    property var source
    property var animal

    property var currentIndexPlaying: -1

    onVisibleChanged: { audioFile.stop(); currentIndexPlaying = -1 }

    states: [
        State { when: tile.stateVisible;
                PropertyChanges {   target: tile; opacity: 1.0    }},
        State { when: !tile.stateVisible;
                PropertyChanges {   target: tile; opacity: 0.0    }}
    ]
    transitions: [ Transition { NumberAnimation { property: "opacity"; duration: 500}} ]

    Rectangle{
        width: layout.width + 50
        height: layout.height + 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        radius: 10
        color: "#dfe4ea"
        opacity: 0.9

        MouseArea{
            anchors.fill: parent
            onClicked: tile.stateVisible = false
        }
    }


    GridLayout{
        id: layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Layout.alignment: Qt.AlignCenter
        columns: 2
        property var rowSpanValue
        Image {
            Layout.rowSpan: layout.rowSpanValue
            id: animalZoom
            Layout.alignment: Qt.AlignCenter
            sourceSize.width: tile.width / 3
            sourceSize.height: tile.height / 3
            source: tile.source
            fillMode: Image.PreserveAspectFit
        }


        Label{
            id: animalName
            Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.WordWrap
            lineHeight: 1
            font.pixelSize: Qt.application.font.pixelSize * 2.0
            text: tile.animal
        }

        Label{
            id: description
            Layout.alignment: Qt.AlignHCenter
            lineHeight: 2
            Connections{
                target: jsoncare
                onSendDescription:{
                    if(_description !== "")
                    {
                        description.text = _description
                        description.visible = true
                        layout.rowSpanValue = 4
                    }
                    else
                    {
                        description.visible = false
                        layout.rowSpanValue = 3
                    }
               }
            }
        }

        Frame{

            background: Rectangle {
                   color: "transparent"
                   radius: 5
               }

            Layout.fillWidth: true
            ColumnLayout{
                id: sounds
                spacing: 8
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                Connections{
                    target: jsoncare
                    onSoundsPath:{
                        audioFile.stop();
                        currentIndexPlaying = -1;
                        repeaterSounds.model = _paths;     
                   }

                }

                Repeater{
                    id: repeaterSounds

                    RowLayout{
                        id: soundsControler
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true

                        Slider{
                            id:slider
                            from: 0.0
                            stepSize: 0.1
                            to: audioFile.duration
                            value: currentIndexPlaying === index ? audioFile.position : 0.0
                            onMoved: audioFile.seek(value)
                            Layout.fillWidth: true
                        }
                        Label{
                            id: time
                            property var duree : currentIndexPlaying === index ? audioFile.duration : 0.0
                            text: currentIndexPlaying === index ? shower.durationText(audioFile.position, audioFile.duration) : shower.durationText(0.0, audioDummy.duration)
                        }

                        RoundButton {
                            id: stop
                            icon.name: "stop"
                            icon.width: 64
                            icon.height: 64
                            enabled: currentIndexPlaying !== index ? false : true
                            onClicked: {
                                currentIndexPlaying = -1;
                                audioFile.stop();
                            }
                        }

                        RoundButton {
                            checked: false
                            id: play
                            icon.name: "play1"
                            icon.width: 64
                            icon.height: 64
                            enabled: true

                            property bool playing: currentIndexPlaying !== index ? false : true
                            onClicked: {
                                audioFile.source = "file:///" + modelData

                                if(audioFile.playbackState === Audio.PlayingState)
                                    audioFile.pause();
                                else
                                {
                                    currentIndexPlaying = index
                                    audioFile.play();
                                }
                            }
                        }

                        Audio{
                            id: audioDummy
                            source: "file:///" + modelData
                        }
                    }
                }
                Audio{
                    id: audioFile
                    onPlaying: {
                        for(var i = 0; i < repeaterSounds.count; i++)
                        {
                            repeaterSounds.itemAt(i).children[3].icon.name = currentIndexPlaying === i ? "pause" : "play1";
                        }
                    }
                    onPaused: {
                        repeaterSounds.itemAt(currentIndexPlaying).children[3].icon.name = "play1";
                    }
                    onStopped: {
                        for(var i = 0; i < repeaterSounds.count; i++)
                        {
                            repeaterSounds.itemAt(i).children[3].icon.name = "play1";
                        }
                    }
                }
            }
        }
        Button{
            id: expertMode
            text: "Ouvrir dans le mode avancé"
            Layout.fillWidth: true
            height: 20
            onClicked: { shower.show(); audioFile.stop(); }
        }

        Label{
            id: credits
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter
            Connections{
                target: jsoncare
                onSendCopyrights:{
                    credits.text = _copyrights
               }
            }
        }



    }
}
