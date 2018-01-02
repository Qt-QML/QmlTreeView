#include "TreeModel.hpp"


#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent) {
    mRoleNameMap[TreeModelRoleName] = "name";
    mRoleNameMap[TreeModelRoleBirthday] = "birthday";
    mRootNode = new TreeNode;
    QVariantList vlist;
    vlist.append("root");
    vlist.append("2018-01-01");
    mRootNode->setData(vlist);
}
TreeModel::~TreeModel() {
    delete mRootNode;
    mRootNode = nullptr;
}
void addArrayToTree (TreeNode *node, const QJsonArray &array) {
    TreeNode *newNode = nullptr;
    QJsonArray subArray;
    QVariantList vlist;
    for (const auto &i : array) {
        auto obj = i.toObject();
        newNode = new TreeNode(node);
        vlist.clear();
        vlist.append(obj["name"].toVariant());
        vlist.append(obj["birthday"].toVariant());
        newNode->setData(vlist);
        node->appendChild(newNode);
        subArray = obj["progeniture"].toArray();
        if (!subArray.isEmpty()) {
            addArrayToTree(newNode, subArray);
        }
    }
}
void addTreeToArray (const TreeNode *node, QJsonArray &array) {
    if (node) {
        auto data = node->data();
        QJsonObject obj;
        if (data.length() >= 2) {
            obj["name"] = data.at(0).toString();
            obj["birthday"] = data.at(1).toString();
        }
        int count = node->childCount();
        if (count > 0) {
            QJsonArray arr;
            for (int i = 0; i < count; ++i) {
                addTreeToArray(node->child(i), arr);
            }
            obj["progeniture"] = arr;
        }
        array.append(obj);
    }
}

QString TreeModel::loadDataFromJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "open file failed:" << file.fileName() << file.errorString();
        return QString();
    }
    QJsonParseError error;

    auto doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();
    if (doc.isNull()) {
        qWarning() << error.errorString();
    }

    auto rootObj = doc.object();
    const auto & array = rootObj.value("data").toArray();
    emit beginResetModel();
    addArrayToTree(mRootNode, array);
    emit endResetModel();
    return QString();
}

QString TreeModel::saveDataToJson(const QString &filePath) const {
    Q_UNUSED(filePath);
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly)) {
        qWarning() << "open file failed" << file.fileName() << file.errorString();
        return file.errorString();
    }

    QJsonArray array;
    //直接取子节点，忽略根节点的信息
    int count = mRootNode->childCount();
    for (int i = 0; i < count; ++i) {
        addTreeToArray(mRootNode->child(i), array);
    }

    QJsonObject rootObj;
    rootObj["data"] = array;
    QJsonDocument doc;
    doc.setObject(rootObj);
    file.write(doc.toJson());
    file.flush();
    file.close();
    return QString();
}

TreeNode *TreeModel::getNode(const QModelIndex &index) const {
    if (index.isValid()) {
        auto node = static_cast<TreeNode *>(index.internalPointer());
        if (node) {
            return node;
        }
    }
    return mRootNode;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QModelIndex();
    }
    if (role == TreeModelRoleName || role == TreeModelRoleBirthday) {
        auto node = getNode(index);
        return node->data().value(role - TreeModelRoleName);
    } else {
        return QVariant();
    }
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }
    auto parentNode = getNode(parent);
    auto childNode = parentNode->child(row);
    if (childNode) {
        return createIndex(row, column, childNode);
    } else {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }
    auto childNode = getNode(child);
    auto parentNode = childNode->parent();
    if (parentNode == mRootNode) {
        return QModelIndex();
    }
    return createIndex(parentNode->childCount(), 0, parentNode);
}

int TreeModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return mRootNode->data().length();
}
int TreeModel::rowCount(const QModelIndex &parent) const {
    return getNode(parent)->childCount();
}

bool TreeModel::hasChildren(const QModelIndex &parent) const {
    if (!parent.isValid()) return false;
    auto node = getNode(parent);
    if (node) {
        return node->childCount() > 0;
    } else {
        return false;
    }
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) return false;
    TreeNode *node = getNode(index);
    auto column = index.column();
    auto data = node->data();
    if (column < data.length()) {
        data[column] = value;
        node->setData(data);
        emit dataChanged(index, index);
        return true;
    } else {
        return false;
    }
}

QHash<int, QByteArray> TreeModel::roleNames() const {
    return mRoleNameMap;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return 0;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}



