#include <ui/tab.hpp>
#include <ui/main.hpp>
#include <ui/tree.hpp>
#include <ui/tree/item.hpp>
#include <ui/tabwidget.hpp>
#include <ui/tabbar.hpp>
#include <ui/render/engine.hpp>
#include <ui/render/web.hpp>
#include <ui/render/web_page.hpp>

#include <QLineEdit>
#include <QLabel>

#include <QDebug>

//! use events and queued connections for async network events

namespace ui
{
    tab::tab(ui::tabwidget* tabwidget) : QWidget(tabwidget),
        tabwidget_(tabwidget),
        id_((unsigned int)reinterpret_cast<void*>(this))
    {
        qRegisterMetaType<nxs::network::error_code>("error_code");

        // create new connexion
        using output_connexion = nxs::network::output_connexion;
        output_connexion& cnx = tabwidget_->main().client().connexion_add();
        connexion_id_ = cnx.id();

        // bind connexion events
        cnx.on_connect([this]() { emit event_connexion_connect(); });
        cnx.on_close([this](const nxs::network::error_code& status) { emit event_connexion_close(); });

        QObject::connect(this, &tab::event_connexion_connect, this, &tab::on_connexion_connect, Qt::QueuedConnection);
        QObject::connect(this, &tab::event_transfer_progress, this, &tab::on_transfer_progress, Qt::QueuedConnection);
        QObject::connect(this, &tab::event_connexion_close, this, &tab::on_connexion_close, Qt::QueuedConnection);

        // address
        address_bar_ = new QLineEdit(this);
        address_bar_->setObjectName("address");
        address_bar_->setFixedHeight(24);

        tabwidget->stack("address")->addWidget(address_bar_);
        QObject::connect(address_bar_, &QLineEdit::returnPressed, this, &tab::address_load);

        // status
        connexion_status_ = new QLabel(this);
        connexion_status_->setFixedSize(24, 24);
        connexion_status_->setPixmap(QIcon(":/image/connexion_status_idle").pixmap(20, 20));
        tabwidget->stack("status")->addWidget(connexion_status_);

        // tree
        tree_ = new ui::tree(this);
        tabwidget->stack("tree")->addWidget(tree_);

        // engine page
        page_ = new render::web_page(this);

        QObject::connect(this, &tab::page_load, page_, &render::page::load, Qt::QueuedConnection);

        static_cast<render::web_page*>(page_)->load_url("http://www.google.fr");

        address_bar_->setText("nex://127.0.0.1");
    }

    tab::~tab()
    {
        tabwidget_->main().client().connexion_delete(connexion_id_);
    }

    void tab::address_load()
    {
        try
        {
            auto address_text = address_bar_->text();
            url_ = nxs::network::url(address_text.toStdString());

            // connexion alive, send command
            if (connexion().is_alive())
            {
                connexion().protocol().send(url_.command(), [this](nxs::nex& nex)
                {
                    QString result = nex.input().data().get().c_str();
                    page_load(result);
                });
            }
            else
            {
                title_set("connecting...");
                connexion().connect(url_.host(), url_.port());
            }
        } catch (const std::exception& e)
        {
            //emit engine_load(e.what());
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////             EVENTS             ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    void tab::on_connexion_connect()
    {
        page_load("on_connexion_connect");
        title_set(url_.host().c_str());
        connexion_status_->setPixmap(QIcon(":/image/connexion_status_1").pixmap(20, 20));
    }

    void tab::on_transfer_progress(unsigned int n)
    {

    }


    void tab::on_connexion_close()
    {
        title_set("closed");
        connexion_status_->setPixmap(QIcon(":/image/connexion_status_0").pixmap(20, 20));
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////            GET/SET             ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    void tab::title_set(const QString& title)
    {
        tabwidget_->tabbar().title_set(this, title);
    }

    void tab::icon_set(const QIcon& icon)
    {
        tabwidget_->tabbar().icon_set(this, icon);
    }

    size_t tab::id() const
    {
        return id_;
    }

    const nxs::network::url& tab::url() const
    {
        return url_;
    }

    nxs::network::output_connexion& tab::connexion()
    {
        return tabwidget_->main().client().connexion(connexion_id_);
    }

    render::page& tab::page()
    {
        return *page_;
    }
} // ui
