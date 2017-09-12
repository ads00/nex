#include <ui/tree/nex_item.hpp>
#include <ui/tree.hpp>
#include <widget/menu.hpp>

#include <nxs/network/client.hpp>
#include <nxs/resource.hpp>

#include <QDebug>
#include <QTreeWidgetItem>

namespace ui
{
    tree_nex_item::tree_nex_item(ui::tree* tree, size_t id, tree_item* parent) :
        tree_item(tree, parent),
        id_(id)
    {
    }

    tree_nex_item::tree_nex_item(tree_item* parent, size_t id) :
        tree_nex_item(&parent->tree(), id, parent)
    {}

    void tree_nex_item::list()
    {
        tree_item* parent_item = static_cast<tree_item*>(this);
        parent_item->takeChildren();

        session().send("nxs::resource_get;", [parent_item, this](nxs::nex& nex)
        {
            auto res_list = nex.input().data(0).get<std::vector<nxs::resource>>();

            for (auto& res : res_list)
            {
                tree_nex_item* item = new tree_nex_item(parent_item, res.id());
                item->setText(0, res.name().c_str());
                item->setIcon(0, QIcon(":/image/resource"));
                parent_item->addChild(item);
            }
        });
    }

    void tree_nex_item::option()
    {
        widget::menu add_menu;
        add_menu.add("void")->on_trigger([this]()
        {
           session().send("nxs::resource_add;name=new_resource;", [this](nxs::nex& nex)
           {
               auto new_item = new tree_nex_item(this, nex.input().data().get<int>());
               new_item->setText(0, "new_resource");
               new_item->setIcon(0, QIcon(":/image/resource"));
               addChild(new_item);
           });
        });

        widget::menu menu;
        menu.add("Add resource", add_menu);

        menu.add("Delete")->on_trigger([this]()
        {
            session().send("nxs::resource_del;id=" + std::to_string(id_) + ";", [this](nxs::nex& nex)
            {
                tree().item_del(this);
            });
        });

        void tree::item_del(tree_item* item) { emit event_item_del(item); }
        void tree::on_item_del(tree_item* item) { delete item; }

        menu.exec();
    }
} // ui