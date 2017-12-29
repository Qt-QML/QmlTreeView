#pragma once

#include <QObject>
#include <QAbstractItemModel>

#include "TreeNode.hpp"

class TreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    enum TreeModelRoles {
        TreeModelRoleName = Qt::UserRole + 1,
        TreeModelRoleBirthday
    };
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();
    Q_INVOKABLE QString loadDataFromJson(const QString &filePath);
    Q_INVOKABLE QString saveDataToJson(const QString &filePath) const;
    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    bool hasChildren(const QModelIndex &parent) const override;

//    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
//    Qt::ItemFlag flags(const QModelIndex &index) const override;
protected:
    TreeNode *getNode(const QModelIndex &index) const;
private:
    TreeNode *mRootNode = nullptr;
    QHash<int, QByteArray> mRoleNameMap;
};

