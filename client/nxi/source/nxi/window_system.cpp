#include <nxi/system/window.hpp>

#include <nxi/core.hpp>
#include <nxi/database/ui.hpp>
#include <nxi/system/window.hpp>
#include <include/nxi/system/window.hpp>


namespace nxi
{
	window_system::window_system(nxi::core& nxi_core) :
		m_nxi_core{ nxi_core }
	{}

    void window_system::load()
    {
        // load stored windows
        auto res = ndb::query<dbs::ui>() << (
        ndb::get(nxi_model.window.id, nxi_model.window.x, nxi_model.window.y, nxi_model.window.w, nxi_model.window.h)
        << ndb::source(nxi_model.window));
        for (auto& line : res)
        {
            nxi::window w;
            w.id = line[nxi_model.window.id];
            w.x = line[nxi_model.window.x];
            w.y = line[nxi_model.window.y];
            w.w = line[nxi_model.window.w];
            w.h = line[nxi_model.window.h];
            m_windows.push_back(w);
        }
    }

    void window_system::add(nxi::window win)
    {
        win.id = 1;
        ndb::query<dbs::ui>() << ndb::add(
        nxi_model.window.x = win.x
        , nxi_model.window.y = win.y
        , nxi_model.window.w = win.w
        , nxi_model.window.h = win.h
        );
        emit event_add(std::move(win));
    }

    void window_system::del(int id)
    {
        ndb::query<dbs::ui>() << (ndb::del << ndb::source(nxi_model.window) << ndb::filter(nxi_model.window.id == id));
    }

    void window_system::move(unsigned int id, int x, int y)
    {
        ndb::query<dbs::ui>() << ndb::set(nxi_model.window.x = x, nxi_model.window.y = y);
        emit event_position_update(x, y);
    }

    void window_system::resize(unsigned int id, int w, int h)
    {
        ndb::query<dbs::ui>() << ndb::set(nxi_model.window.w = w, nxi_model.window.h = h);
    }

    std::vector<nxi::window>& window_system::get()
    {
        return m_windows;
    }
} // nxi