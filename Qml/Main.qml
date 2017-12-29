import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as QC14
import QtQuick.Controls.Styles 1.4 as QCS14
QC14.TreeView {
    width: 640
    height: 480
    id: treeView
    alternatingRowColors: false
    QC14.TableViewColumn {
        role: "name"
        title: "Name"
    }
    QC14.TableViewColumn {
        role: "birthday"
        title: "Birthday"
    }
    model: tModel
    itemDelegate: Rectangle {
        width: 200
        implicitHeight: 20
        Text {
            text: styleData.value
            color: styleData.value
        }
    }
//    rowDelegate: Item {
//        width: treeView.width
//    }
}

