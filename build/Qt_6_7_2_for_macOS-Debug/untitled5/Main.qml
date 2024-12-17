import QtQuick 2.15
import QtQuick.Controls 2.15



ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Modern Buttons UI"
    Rectangle
    {
        id :barBottomTop
        anchors
        {
            top:parent.top

        }
        width: parent.width
        height: parent.height *.2
        Rectangle
        {
            id: rectangel
            //property string buttenImageSource: " "
            height: width
            radius: width/2
            color: "#e1ede4"

            Image {
                id: imageone
                width: parent.width * .6
                height: parent.height * .6
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
                source: "qrc:/image/Image/icons8-html-5-400.png"

            }
        }
        /*ButtenTest{
            id: startImage
            buttenImageSource: "qrc:/image/Image/icons8-html-5-400.png"
            width: parent.width *.6


        }*/
    }



}
