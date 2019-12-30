import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Imagine 2.3
import QtQuick.Window 2.2
import "./" as T


Item {
    height: Screen.height
    width: Screen.width

    GridView{
       id: listview
       height: parent.height
       width: parent.width
       anchors.top:  parent.top
       anchors.bottom: parent.bottom
       anchors.horizontalCenter: parent.horizontalCenter
       cellWidth: {
            var w = Math.sqrt(Math.ceil(parent.width * parent.height / count));
            var y = parent.width / w;
            y = Math.ceil(y)
            if(y > 6)
                y = 6
            parent.width / y
       }
       cellHeight: cellWidth

       property var animals

       delegate: ItemDelegate{
           Rectangle{
               color: "white"
               width: listview.cellWidth
               height: listview.cellHeight
               Image {
                   id: image
                   width: listview.cellWidth - 3
                   height: listview.cellHeight - 3
                   fillMode: Image.PreserveAspectCrop
                   source: modelData

                   MouseArea{
                       anchors.fill: parent
                       z: 1
                       hoverEnabled: true

                       onClicked:{
                           tile.stateVisible = true;
                           tile.source = image.source
                           tile.animal = listview.animals[index]
                           jsoncare.sendPaths(tile.animal)
                       }

                   }
               }
           }
       }

       ScrollBar.vertical: ScrollBar {
           parent: listview
           policy: ScrollBar.AlwaysOn
           anchors.top: parent.top
           anchors.topMargin: listview.topPadding
           anchors.right: parent.right
           anchors.rightMargin: 1
           anchors.bottom: parent.bottom
           anchors.bottomMargin: listview.bottomPadding
       }
   }

   Component.onCompleted: {
       var images = [];
       var animals = []
       var paths = jsoncare.getAllImagesPaths();
       for(var i = 0; i < paths.length; i += 2)
       {
           animals.push(paths[i])
           images.push('file:///' + paths[i+1])
       }
       listview.model = images;
       listview.animals = animals

   }

   Rectangle{
       id: filter
       width: filterbuttons.width + 50
       height: 75
       color: "#dfe4ea"
       opacity: 0.7
       radius: 20
       anchors.bottom: parent.bottom
       anchors.bottomMargin: 10
       anchors.horizontalCenter: parent.horizontalCenter


       ButtonGroup { id: radioGroup }
       Row{
           id: filterbuttons
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.verticalCenter: parent.verticalCenter
           spacing: 20

           property variant famille

           Component.onCompleted: {
               filterbuttons.famille = jsoncare.getFamily();
           }

           Button{
               id: allButton
               checked: true
               text: "Tous"
               flat: true
               height: filter.height - 25
               ButtonGroup.group: radioGroup
               onClicked:{
                   checked = true
                   var images = [];
                   var animals = []
                   var paths = jsoncare.getAllImagesPaths();
                   for(var i = 0; i < paths.length; i += 2)
                   {
                       animals.push(paths[i])
                       images.push('file:///' + paths[i+1])
                   }
                   listview.model = images;
                   listview.animals = animals
               }
           }

           Repeater{
               model: filterbuttons.famille

               Button{
                   id: buttons
                   flat: true
                   text: modelData
                   ButtonGroup.group: radioGroup
                   height: filter.height - 25
                   onClicked:{
                       checked = true
                       var images = [];
                       var animals = []
                       var paths = jsoncare.getAllImagesPathsFamily(modelData);
                       for(var i = 0; i < paths.length; i += 2)
                       {
                           animals.push(paths[i])
                           images.push('file:///' + paths[i+1])
                       }
                       listview.model = images;
                       listview.animals = animals

                   }
               }
           }
       }

   }

   T.Tile{
        id:tile

   }

}
