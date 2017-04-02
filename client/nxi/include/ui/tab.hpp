#ifndef UI_TAB_H_NXI
#define UI_TAB_H_NXI

#include <nxs/network/connexion/output.hpp>
#include <nxs/network/url.hpp>

#include <QWidget>
#include <QString>

class QTabBar;
class QLineEdit;
class QStackedWidget;

namespace ui
{
    class main;
    class tree;

    class tab : public QWidget
    {
        Q_OBJECT
    private:
        ui::main* _main;
        size_t _connexion_id;
        size_t _index;

        int _id;
        QString _name;
        nxs::network::url _url;

        QLineEdit* _address_bar;
        ui::tree* _tree;
        QStackedWidget* _engine_stack;

    public:
        tab(ui::main* main_window, size_t tab_index);
        ~tab();

        void load(const nxs::network::url&);
        void address_load();

        void on_connect();
        void on_transfer_progress(unsigned int);
        void on_transfer_error();

        nxs::network::output_connexion& connexion();

        void title_set(const std::string&);
        void icon_set(const QIcon&);
        QTabBar& tabbar() const;


    signals:
        void engine_load(const QString&);
        void event_connect();
        void event_transfer_complete();
        void event_transfer_progress(int);
        void event_transfer_error();
    };

} // ui

#endif // UI_TAB_H_NXI
