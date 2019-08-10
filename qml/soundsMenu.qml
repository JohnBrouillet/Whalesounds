import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Imagine 2.3
import "functions.js" as Func


Item {
    id: menuFiles
    height: 1080-50
    width: 1150


    RowLayout
    {
        id: rowE
        anchors.fill: parent

        anchors.margins: 10
        spacing: 10


        ColumnLayout{
            spacing: 16
            Layout.preferredWidth: 30

            Button{
                Layout.alignment: Qt.AlignHCenter
                width: parent.width
                enabled: false
                visible: false
                text:"Générateur de son"
                onClicked:{
                    animalItem.visible = false
                    generator.visible = true;
                    freqActivator.visible = true;
                    amplSlider.visible = true;
                    songNameLabel.text = "Générateur de son"
                    timelabel.text = "00:00/00:00"
                    infoLabel.text = ""
                    playercontrols.setGeneratorMode(true);
                }


            }

            ButtonGroup{
                buttons: rowbutton.children
            }

            GridLayout{
                id: rowbutton
                rowSpacing: 5
                columnSpacing: 5

                columns: 2

                Layout.alignment: Qt.AlignHCenter
                property variant famille

                Component.onCompleted: {
                    rowbutton.famille = jsoncare.getFamily();

                }

                Repeater {
                    model: rowbutton.famille
                    Button {
                        Layout.fillWidth: true
                        text: modelData
                        onClicked: {
                            filesFrame.famillyClicked(modelData);
                        }
                        Component.onCompleted: {
                            if(index == 0)
                            {
                                filesFrame.famillyClicked(modelData);
                                checked = true;
                            }
                        }

                        checkable: true
                    }
                }
            }


            Frame {
                id: filesFrame
                leftPadding: 1
                rightPadding: 1

                Layout.fillWidth: true
                Layout.fillHeight: true

                signal famillyClicked(var name)

                onFamillyClicked: listFiles.newFamilly(name)

                ListView {
                    clip: true
                    anchors.fill: parent
                    model: ListModel {
                        id: listFiles
                        signal newFamilly(var name)

                        onNewFamilly: {
                            listFiles.clear();
                            var listAnimals = jsoncare.getEspeces(name);
                            for (var i = 0; i < listAnimals.length; i += 2) {
                                append({
                                   speciesName: listAnimals[i],
                                   nbTracks: listAnimals[i+1]
                                });
                            }
                        }
                    }
                    delegate: ItemDelegate {
                        text: model.speciesName + " - " + model.nbTracks + " fichier(s)"
                        width: parent.width
                        onClicked: {
                            jsoncare.sendPaths(model.speciesName);
                            animalItem.visible = true;
                            generator.visible = false;
                            freqActivator.visible = false;
                            amplSlider.visible = false;
                            playercontrols.setGeneratorMode(false);
                        }

                    }

                    ScrollBar.vertical: ScrollBar {
                        parent: filesFrame
                        policy: ScrollBar.AlwaysOn
                        anchors.top: parent.top
                        anchors.topMargin: filesFrame.topPadding
                        anchors.right: parent.right
                        anchors.rightMargin: 1
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: filesFrame.bottomPadding
                    }
                }
            }



        }

        ColumnLayout {
            spacing: 26
            Layout.preferredWidth: 50
            Layout.alignment: Qt.AlignHCenter

            RowLayout{
                id: generator
                visible: false
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: false

                Layout.maximumHeight: 170

                Repeater {
                    id: rep
                    model: 5

                    Slider {
                        id: seekSlider
                        value: 440
                        orientation: Qt.Vertical
                        height:  120
                        from: 0
                        to: 1000
                        enabled: index == 0 ? true : false
                        stepSize: 1
                        Layout.fillHeight: false
                        Layout.preferredHeight: 140

                        ToolTip {
                            parent: seekSlider.handle
                            visible: seekSlider.pressed
                            text: value + " Hz"
                            y: parent.height
                            readonly property int value: seekSlider.valueAt(seekSlider.position)
                        }

                        onValueChanged: wavegenerator.generate(Func.generateWave());
                    }
                }
            }

            RowLayout{
                id: amplSlider
                visible: false
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: false
                Layout.preferredHeight: 50

                Repeater {
                    id: amplRep
                    model: 5

                    Slider {
                        id: ampli
                        value: index == 0 ? 10 : 1
                        orientation: Qt.Vertical
                        height: 10
                        from: 0
                        to: 10
                        enabled: index == 0 ? true : false
                        stepSize: 1
                        Layout.fillHeight: false
                        Layout.preferredHeight: 50

                        ToolTip {
                            parent: ampli.handle
                            visible: ampli.pressed
                            text: (value*0.1).toFixed(1)
                            y: parent.height
                            readonly property int value: ampli.valueAt(ampli.position)
                        }

                        onValueChanged: wavegenerator.generate(Func.generateWave());
                    }
                }
            }


            RowLayout{
                id: freqActivator
                visible: false
                spacing: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.maximumHeight: 170

                Repeater {
                    id: freqRepeat
                    model: 5

                    CheckBox {
                        id: freqCheck
                        checked: index == 0 ? true : false
                        onCheckedChanged: {
                            rep.itemAt(index).enabled = freqCheck.checkState;
                            amplRep.itemAt(index).enabled = freqCheck.checkState;
                            wavegenerator.generate(Func.generateWave());
                        }
                    }
                }
            }



            Item {
                id: animalItem
                Layout.fillWidth: true
                Layout.fillHeight: true

                Image {
                    id: animal
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    Connections{
                        target: jsoncare
                        onImagePath:{
                            animal.source = 'file:///' + _path;

                       }
                    }
                }
            }



            Label {
                id: songNameLabel
                Layout.alignment: Qt.AlignHCenter

                font.pixelSize: Qt.application.font.pixelSize * 1.4
                Connections{
                    target: jsoncare
                    onName: { songNameLabel.text = _name }
                }
            }


            Label
            {
                Layout.alignment: Qt.AlignHCenter
                id: timelabel
                text: "00/00"
                Connections{
                    target: playercontrols
                    onNewTime: { timelabel.text = time;}
                }
            }

            Label
            {
                Layout.alignment: Qt.AlignHCenter
                id: infoLabel
                Connections{
                    target: playercontrols
                    onNewText: { infoLabel.text = _text;}
                }
            }

            RowLayout {
                spacing: 8
                Layout.alignment: Qt.AlignHCenter


                RoundButton {
                    id: prevButton
                    icon.name: "previous"
                    icon.width: 32
                    icon.height: 32
                    enabled: false
                    onClicked: {
                        playercontrols.previous();
                    }

                    Connections{
                        target: playercontrols
                        onNewIndexFile:{
                            if(indexFile == 0 )
                                prevButton.enabled = false;
                            else
                                prevButton.enabled = true;
                        }
                    }
                }

                RoundButton {
                    id: stopButton
                    icon.name: "stop"
                    icon.width: 32
                    icon.height: 32
                    onClicked: playercontrols.reloadBuffer(false);
                }

                RoundButton {
                    // 1 : bouton play, 0 : button pause
                    property bool playing: true
                    id: playButton
                    icon.name: playing ? "play" : "pause"
                    icon.width: 32
                    icon.height: 32
                    enabled: true
                    onClicked: {
                        playercontrols.playClicked();
                    }

                    Connections{
                        target: playercontrols
                        onNewFiles: playButton.enabled = true;
                        onNewState: playButton.playing = state;
                    }
                }

                RoundButton {
                    id: nextButton
                    icon.name: "next"
                    icon.width: 32
                    icon.height: 32
                    enabled: false
                    onClicked: {
                        playercontrols.next();
                    }

                    Connections{
                        target: playercontrols
                        onNewIndexFile:{
                            if(indexFile == nbfiles - 1 )
                                nextButton.enabled = false;
                            else
                                nextButton.enabled = true;
                        }
                    }
                }
            }

            GridLayout{
                id: spectroControl
                Layout.alignment: Qt.AlignHCenter

                columns: 6

                Label{
                    text: "Zoom horizontal"
                }

                CheckBox{
                    id: checkX
                    visible: true
                    onCheckedChanged: spectrowidget.zoom(checkX.checkState, checkY.checkState)
                }

                Button{
                    icon.name: "zoomPx"
                    icon.width: 32
                    icon.height: 32

                    onClicked: spectrowidget.zoomSpectro(1)
                }

                Label{
                    text: "Echelle \n colorimétrique"
                    horizontalAlignment: Label.Center
                    Layout.rowSpan: 2
                }

                Button{
                    icon.name: "plus"
                    icon.width: 32
                    icon.height: 32

                    onClicked: spectrowidget.changeColorRange(1)
                }

                Label{
                    text: "Réinitialisation \n échelle"
                    horizontalAlignment: Label.Center
                }


                Label{
                    text: "Zoom vertical"
                }

                CheckBox{
                    id: checkY
                    onCheckedChanged: spectrowidget.zoom(checkX.checkState, checkY.checkState)
                }

                Button{
                    icon.name: "zoomPy"
                    icon.width: 32
                    icon.height: 32

                    onClicked: spectrowidget.zoomSpectro(0)

                }


                Button{
                    icon.name: "moins"
                    icon.width: 32
                    icon.height: 32

                    onClicked: spectrowidget.changeColorRange(0)
                }

                Button{
                    icon.name: "repeat"
                    icon.width: 32
                    icon.height: 32

                    Layout.alignment: Qt.AlignHCenter
                    onClicked: spectrowidget.reinit();

                }


            }


        }
    }
}



