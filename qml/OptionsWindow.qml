import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.3
import QtQuick.Layouts 1.12


Item {
    id: optionswindow
    width: 200

    Column{
        anchors.fill: parent
        anchors.margins: 20

        CheckBox{
            text: "Cepstrogramme"
            width: parent.width
        }

    }

}
