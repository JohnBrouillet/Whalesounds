import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Imagine 2.3
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.2

Item {
    id: menuFiles
    Material.accent: Material.Blue
    height: Screen.height / 2
    width: Screen.width / 2

    ColumnLayout
    {
        id: rowE
        anchors.fill: parent

        anchors.margins: 10
        spacing: 10


        ColumnLayout{
            visible: false
            spacing: 16
            Layout.preferredWidth: 30

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
            id: playerLayout
            visible: true
            spacing: 26
            Layout.preferredWidth: 50
            Layout.alignment: Qt.AlignHCenter

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

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: zoomPix.visible = true;
                    }
                }
            }



            Label {
                id: songNameLabel
                Layout.alignment: Qt.AlignHCenter

                font.pixelSize: Qt.application.font.pixelSize * 1.4
                Connections{
                    target: jsoncare
                    onNewTrack: { songNameLabel.text = track.getName(); }
                }
            }


            RowLayout
            {
                spacing: 8
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: timeSlider
                    from: 0.0
                    stepSize: 0.1
                    Connections{
                        target: playercontrols
                        onNewDuration: timeSlider.to = value;
                        onNewPosition: timeSlider.value = value;
                    }

                    onMoved: playercontrols.seek(timeSlider.value);

                }

                Label
                {
                    id: timelabel
                    text: "00/00"
                    Connections{
                        target: playercontrols
                        onNewTime: { timelabel.text = time;}
                    }
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
                    icon.name: playing ? "play1" : "pause"
                    icon.width: 64
                    icon.height: 64
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
        }

        ColumnLayout{
            id: control
            visible: false
            Layout.alignment: Qt.AlignHCenter
    
            GridLayout{

                id: spectroControl
                Layout.alignment: Qt.AlignHCenter

                columns:  6

///////////////////
                Label{
                    text: "Vitesse \n de lecture"
                    horizontalAlignment: Label.Center
                }

                ComboBox{
                    id: rate
                    Layout.fillWidth: true
                    model : ["x0.25", "x0.5", "x0.75", "x1", "x1.25", "x1.5", "x1.75", "x2" ]
                    currentIndex: 3
                    onActivated: playercontrols.changeRate(rate.currentIndex);
                    Layout.columnSpan: 2
                }

                Label{
                    text: "Echelle \n colorimétrique"
                    horizontalAlignment: Label.Center
                }


                Button{
                    icon.name: "moins"
                    icon.width: 64
                    icon.height: 64
                    Layout.fillWidth: true

                    onClicked: spectrogram.changeColorRange(0)
                }

                Button{
                    icon.name: "plus"
                    icon.width: 64
                    icon.height: 64
                    Layout.fillWidth: true

                    onClicked: spectrogram.changeColorRange(1)
                }
///////////////////
                Label{
                    text: "Zoom \n horizontal"
                    horizontalAlignment: Label.Center
                }

                CheckBox{
                    id: checkX
                    visible: true
                    onCheckedChanged: {
                        spectrogram.zoom(checkX.checkState, checkY.checkState);
                        audiograph.zoom(checkX.checkState, checkY.checkState);
                    }
                }

                Button{
                    icon.name: "zoomPx1"
                    icon.width: 64
                    icon.height: 64

                    onClicked:{
                        spectrogram.zoomSpectro(1);
                        if(!checkY.checkState) audiograph.zoomSpectro(1);
                    }
                }

                Label{
                    text: "Réinitialisation \n échelle"
                    horizontalAlignment: Label.Center
                }

                Button{
                    icon.name: "repeat"
                    icon.width: 64
                    icon.height: 64
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: spectrogram.reinit();
                    Layout.columnSpan: 2
                }
////////////////////
                Label{
                    text: "Zoom \n vertical"
                    horizontalAlignment: Label.Center
                }

                CheckBox{
                    id: checkY
                    onCheckedChanged: {
                        spectrogram.zoom(checkX.checkState, checkY.checkState);
                        audiograph.zoom(checkX.checkState, checkY.checkState);
                    }
                }

                Button{
                    icon.name: "zoomPy1"
                    icon.width: 64
                    icon.height: 64

                    onClicked: {
                        spectrogram.zoomSpectro(0);
                        if(!checkY.checkState) audiograph.zoomSpectro(0);
                    }
                }


                Label{
                    text: "Couleur"
                    Layout.alignment: Qt.AlignCenter

                }

                ComboBox{
                    id: colorGradientChoices
                    model: ["Grayscale", "Hot", "Cold", "Night", "Candy", "Geography", "Ion", "Thermal", "Polar", "Spectrum", "Jet", "Hues"]
                    currentIndex: 8
                    onActivated: spectrogram.changeColor(colorGradientChoices.currentIndex);
                    Layout.columnSpan: 2
                    Layout.fillWidth: true

                }

 ////////////////////
                Button{
                    text: "Inverser les couleurs"
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: spectrogram.reverseColor();
                    Layout.columnSpan: 6
                }

////////////////////
                Button{
                    text: "Inverser les graphes"
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: layout.reverseGraph();
                    Layout.columnSpan: 6
                }

            }
        }

        Row{
            spacing: 5
            Layout.alignment: Qt.AlignHCenter

            Button{
                text: "Revenir aux photos"
                onClicked: shower.unshow()
            }

            Button{
                id: controlVisibility
                onClicked: {
                    control.visible = !control.visible;
                    playerLayout.visible = !playerLayout.visible;
                }
                text: "Configuration"
            }
        }
    }

    Item{
        id: zoomPix
        width: parent.width
        height: parent.height
        visible: false

        Rectangle{
            anchors.fill: parent
            color: "white"
            opacity: 0.9
        }

        MouseArea{
            anchors.fill: parent
            onClicked: zoomPix.visible = false
        }

        ColumnLayout{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Layout.alignment: Qt.AlignHCenter

            Image {
                id: animalZoom
                Layout.alignment: Qt.AlignHCenter
                sourceSize.width: menuFiles.width / 2
                sourceSize.height: menuFiles.height / 2
                source: animal.source
            }

            Label{
                id: description
                Layout.alignment: Qt.AlignHCenter
                wrapMode: Text.WordWrap
                lineHeight: 3
                font.pixelSize: Qt.application.font.pixelSize
                Connections{
                    target: jsoncare
                    onNewTrack: { description.text = track.getDescription(); }
                }
            }

            Label
            {
                Layout.alignment: Qt.AlignHCenter
                id: infoLabel
                Connections{
                    target: jsoncare
                    onNewTrack: { infoLabel.text = track.getCopyrights();}
                }
            }
        }
    }

}



