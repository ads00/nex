#ifndef UI_TREE_H_NXI
#define UI_TREE_H_NXI

#include <QTreeWidget>

class QString;
class QIcon;

namespace ui
{
    class tab;
    class tree_item;

    class tree : public QTreeWidget
    {
        Q_OBJECT
    private:
        ui::tab* tab_;

    public:
        tree(tab*);
        ~tree();

        void on_item_expand(QTreeWidgetItem*);
        void on_item_option(const QPoint&);
        void on_connexion_connect();
        void on_item_add(tree_item*, tree_item* parent = nullptr);
        void on_item_del(tree_item*);

        void item_add(tree_item*, tree_item* parent = nullptr);
        void item_del(tree_item*);

        ui::tab& tab();

    signals:
        void event_item_add(tree_item*, tree_item* parent = nullptr);
        void event_item_del(tree_item*);
    };

} // ui

#endif // UI_TREE_H_NXI
