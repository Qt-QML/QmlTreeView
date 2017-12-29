import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as QC14
import QtQuick.Controls.Styles 1.4 as QCS14
QC14.TreeView {
    width: 640
    height: 480
    id: treeView
    alternatingRowColors: false
//    headerVisible: false
    QC14.TableViewColumn {
        role: "name"
        title: "Name"
        width: 200
    }
    QC14.TableViewColumn {
        role: "birthday"
        title: "Birthday"
        width: 200
    }
    model: tModel
    readonly property color cellBackgroundColor: "#EDEDF0"
    readonly property color cellCurrentRowColor: "#C4DEF4"
    readonly property color cellPressedColor: "#32A6FF"
    style: QCS14.TreeViewStyle {
        activateItemOnSingleClick: true
        backgroundColor: cellBackgroundColor
        itemDelegate: Item {
            width: 200
            height: 80
            Rectangle {
                anchors.fill: parent
                border.width: 1
                border.color: "#7f838c"
                color: isSelected ? cellPressedColor :cellBackgroundColor
                //                                       (styleData.pressed ? cellCurrentRowColor : )
                property bool isSelected: treeView.currentIndex === styleData.index
                onIsSelectedChanged: {
                    console.log(styleData.hasActiveFocus, styleData.index)
                    if (isSelected) {
                        forceActiveFocus()
                    }
                }
                Text {
                    text: styleData.value
                    anchors.centerIn: parent
                }
            }
        }

        rowDelegate: Item {
            width: 200
            height: 30
            Rectangle {
                color: cellBackgroundColor
                anchors.fill: parent
            }
        }
    }
}

