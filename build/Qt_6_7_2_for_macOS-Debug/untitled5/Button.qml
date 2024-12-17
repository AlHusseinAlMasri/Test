import QtQuick 2.15

Rectangle
{
    id: rectangel
    property string buttenImageSource: ""
    height: width
    radius: width/2
    color: "#e1ede4"

    Image {
        id: imageone
        width: rectangel.width * .8
        height: rectangel.height * .8
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: rectangel
        source: rectangel.buttenImageSource

    }



}
