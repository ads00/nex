#ifndef UI_RENDER_WEB_PAGE_H_NXI
#define UI_RENDER_WEB_PAGE_H_NXI

#include <ui/render/page.hpp>
#include <QWebEnginePage>

namespace ui{namespace render
{
    class web_page : public QWebEnginePage, public render::page
    {
    Q_OBJECT
    public:
        web_page(QWidget* parent);

        void load_url(const QString& url);
        void load(const QString& data) override;

    };
}} // ui::render

#endif // UI_RENDER_WEB_PAGE_H_NXI
