#include <nxs/error.hpp>
#include <nxs/network/connexion/input.hpp>

namespace nxs{namespace network
{
    template<class Connexion>
    connexion_manager<Connexion>::connexion_manager() :
            m_work(m_ios)
    {}

    template<class Connexion>
    void connexion_manager<Connexion>::store(std::unique_ptr<Connexion> cnx)
    {

        _connexion_list.emplace(cnx.get()->id(), std::move(cnx));
    }

    template<class Connexion>
    void connexion_manager<Connexion>::run()
    {
         m_ios.run();
    }

    template<class Connexion>
    void connexion_manager<Connexion>::stop()
    {
        m_ios.stop();
    }

    template<class Connexion>
    boost::asio::io_service& connexion_manager<Connexion>::ios()
    {
        return m_ios;
    }

    template<class Connexion>
    bool connexion_manager<Connexion>::connexion_exist(size_t id) const
    {
        if (_connexion_list.find(id) != _connexion_list.end()) return true;
        return false;
    }

    template<class Connexion>
    Connexion& connexion_manager<Connexion>::connexion(size_t id) const
    {
        if (connexion_manager<Connexion>::connexion_exist(id)) return *_connexion_list.at(id);
        nxs_error << "connexion not found" << id << log::network;
    }

    template<class Connexion>
    void connexion_manager<Connexion>::connexion_delete(size_t id)
    {
        if (connexion_manager<Connexion>::connexion_exist(id))
        {
            ios().post([this, id]()
                       {
                           _connexion_list.erase(id);
                       });
        }
    }

    template<class Connexion>
    size_t connexion_manager<Connexion>::connexion_count() const
    {
        return _connexion_list.size();
    }
}} // nxs::network
