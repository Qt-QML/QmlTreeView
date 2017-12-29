#pragma once
#include <QList>
#include <QObject>
#include <QVariant>
class TreeNode {
public:
    explicit TreeNode(TreeNode *parent = nullptr) : mParent(parent) {}
    ~TreeNode() {
        qDeleteAll(mChildrenList);
    }

    void appendChild(TreeNode *child) {
        mChildrenList.append(child);
    }
    void removeChild(TreeNode *child) {
        if(mChildrenList.removeOne(child)) {
            //删除成功时释放
            delete child;
        }
    }
    void setData (const QVariantList &data) { mData = data;}
    TreeNode *child(int index) const { return mChildrenList.value(index);}
    TreeNode *parent() const { return mParent;}
    int childCount() const { return mChildrenList.count();}
    QVariantList data() const { return mData;}
private:
    QVariantList mData;
    TreeNode *mParent = nullptr;
    QList<TreeNode *> mChildrenList;
};

