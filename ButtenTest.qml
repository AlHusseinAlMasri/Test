import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id: rectangel
    property string buttenImageSource: " "
    height: width
    radius: width/2
    //color: "#e1ede4"

    Image {
        id: imageone
        width: parent.width * .6
        height: parent.height * .6
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        source: rectangel.buttenImageSource

    }
}


