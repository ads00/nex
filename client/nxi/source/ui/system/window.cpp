#include <ui/system/window.hpp>

#include <nxi/core.hpp>

#include <ui/core.hpp>

#include <ui/interface/main.hpp>
#include <include/ui/system/window.hpp>


namespace ui
{
	window_system::window_system(ui::core& ui_core) :
		m_ui_core{ ui_core }
	{
        QObject::connect(&ui_core.nxi_core().window_system(), &nxi::window_system::event_add, [this](const nxi::window& window)
        {
            add(window);
        });

        // load stored windows
        for (auto window : m_ui_core.nxi_core().window_system().get())
        {
            add(window);
        }

        // no windows, create one
        if (m_windows.size() == 0)
        {
            nxi::window default_window;
            default_window.x = 200;
            default_window.y = 200;
            default_window.w = 400;
            default_window.h = 400;
            m_ui_core.nxi_core().window_system().add(default_window);
        }
	}

    ui::window* window_system::add(nxi::window window)
    {
        auto ui_window = new ui::window(m_ui_core, window.id);
        // make defaut interface
        ui_window->move(window.x, window.y);
        ui_window->resize(window.w, window.h);
        ui_window->show();

        auto ui_interface = new ui::interfaces::main(m_ui_core);
        ui_window->interface_set(ui_interface);

        m_windows.push_back(ui_window);
        return ui_window;
    }

    void window_system::close(ui::window* window)
    {
	    if (count() == 1) m_ui_core.quit();
	    else
        {
            m_ui_core.nxi_core().window_system().del(window->id());
            window->deleteLater();
        }
        m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());
    }

    void window_system::move(ui::window* window, int x, int y)
    {
        m_ui_core.nxi_core().window_system().move(window->id(), x, y);
    }

    void window_system::resize(ui::window* window, int w, int h)
    {
        m_ui_core.nxi_core().window_system().resize(window->id(), w, h);
    }

    unsigned int window_system::count() const
    {
        return m_windows.size();
    }
} // ui