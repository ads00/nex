#ifndef RESOURCE_H_NXS
#define RESOURCE_H_NXS

#include <nxs/database.hpp>
#include <nxs/resource/type.hpp>

namespace nxs
{
    class resource : public db::entity<nxs::resource>
    {
        using Type_List = std::map<std::string, nxs::type>;
    private:
        // nxs::user& _user;
        std::string _name;
        std::chrono::time_point<std::chrono::system_clock> _date_creation;
        Type_List _type;

        void hydrate(const db::line&);

    public:
        constexpr static const auto& db_ref = db::nex.resource;
        resource(int id);
        resource(const db::line&);

        int id() const;
        const std::string& name() const;

        // std::vector<resource> list() const;
        const Type_List& type_get() const;
        static db::result type_add(int id, int type_id);
    };
} // nxs

#endif // RESOURCE_H_NXS
