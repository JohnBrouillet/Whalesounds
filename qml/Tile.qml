import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12

Item{
    id: tile
    width: parent.width
    height: parent.height
    property bool stateVisible: false
    visible: stateVisible
    property var source
    property var animal

    property var currentIndexPlaying: -1
    property var playingGlobal: false

    onVisibleChanged: { audioFile.stop(); playingGlobal = false; currentIndexPlaying = -1 }

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
        radius: 20
        color: "white"
        opacity: 0.9
    }

    MouseArea{
        anchors.fill: parent
        onClicked: tile.stateVisible = false
    }

    GridLayout{
        id: layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Layout.alignment: Qt.AlignCenter
        columns: 2
        Image {
            Layout.rowSpan: 4
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
            lineHeight: 3
            font.pixelSize: Qt.application.font.pixelSize * 2.0
            text: tile.animal
        }

        Label{
            id: description
            Layout.alignment: Qt.AlignHCenter
            Connections{
                target: jsoncare
                onSendDescription:{
                    description.text = _description
               }
            }
        }

        ButtonGroup{id: groupPlayButton }
        ColumnLayout{
            id: sounds
            spacing: 8
            Layout.fillWidth: true

            Connections{
                target: jsoncare
                onSoundsPath:{
                    repeaterSounds.model = _paths;
               }
            }
            Repeater{
                id: repeaterSounds



                RowLayout{
                    id: soundsControler
                    Layout.alignment: Qt.AlignHCenter

                    Label{
                        text: index + " : "
                    }
                    Slider{
                        id:slider
                        from: 0.0
                        stepSize: 0.1
                        to: audioFile.duration
                        value: currentIndexPlaying === index ? audioFile.position : value
                        onMoved: audioFile.seek(value)
                    }
                    Label{
                        id: time
                        text: currentIndexPlaying === index ? (audioFile.position*0.001).toFixed(0) + "/" + (audioFile.duration*0.001).toFixed(0) : "00/00"
                    }

                    RoundButton {
                        id: stop
                        icon.name: "stop"
                        icon.width: 64
                        icon.height: 64
                        enabled: currentIndexPlaying !== index ? false : true
                        onClicked: {
                            currentIndexPlaying = -1
                            audioFile.stop()
                            play.checked = false
                            playingGlobal = false
                        }
                    }

                    RoundButton {
                        ButtonGroup.group: groupPlayButton
                        checked: false
                        id: play
                        icon.name: checked && playingGlobal ? "pause" : "play1"
                        icon.width: 64
                        icon.height: 64
                        enabled: true

                        property bool playing: currentIndexPlaying !== index ? false : true
                        onClicked: {
                            audioFile.source = "file:///" + modelData
                            checked = true
                            if(!playingGlobal)
                            {
                                audioFile.play();
                                playingGlobal = true;
                                currentIndexPlaying = index
                            }
                            else
                            {
                                audioFile.pause()
                                playingGlobal = false
                            }

                        }
                    }
                }
            }
            Audio{
                id: audioFile
            }
        }

        Button{
            id: expertMode
            text: "Ouvrir dans le mode expert"
            Layout.fillWidth: true
            height: 20
            onClicked: shower.show()
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
