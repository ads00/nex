#include <ui/tree/item.hpp>
#include <ui/tree.hpp>
#include <ui/tab.hpp>

namespace ui
{
    tree_item::tree_item(ui::tree* tree, tree_item* parent) :
        QTreeWidgetItem(parent),
        tree_(tree)
    {
        this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled);
    }

    void tree_item::node(bool status)
    {
        QTreeWidgetItem* node = new QTreeWidgetItem(this);
        addChild(node);
    }

    ui::tree& tree_item::tree()
    {
        return *tree_;
    }

    nxs::network::protocol& tree_item::session()
    {
        return tree().tab().connexion().protocol();
    }
} // ui
