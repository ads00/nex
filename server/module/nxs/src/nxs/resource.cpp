#include <nxs/resource.hpp>
#include <nxs/error.hpp>

namespace nxs
{
    const auto& r = db::nex.resource;

    resource::resource(int id) : ndb::entity<db::tables::resource>(id, db::nex.resource)
    {
        /*
        db::result data = db::query() << (r.name, r.owner.id, r.date_creation) << (r.id == id);
        if (data.size() == 0) nxs_error(errc::system, "unable to load resource with id : " + std::to_string(id));
        _id = id;
        _name = data[r.name];
        _date_creation = data[r.date_creation];*/
/*
        db::query q;
        q << (r.type.name);
        std::cout << q.native();
        data = db::query() << (r.type.name);*/

    }

    int resource::id() const { return _id; }
    const std::string& resource::name() const { return _name; }

    //const nxs::user& resource::owner() const { return _owner; }


    std::vector<resource> resource::list()
    {
        std::vector<resource> l;
        db::query q;
        q << (db::nex.resource.name, db::nex.resource.type.name);
        std::cout << "\n" << q.native();
        db::result r = q.exec();
        for (auto& item : r)
        {
            std::cout << "\n" << item[db::nex.resource.name] << " " << item[db::nex.resource.type.name];
        }

        return l;
    }

    db::result resource::add(const std::string& name)
    {
        return db::query() + (r.name = name, r.owner.id = 0);
    }

    db::result resource::get()
    {
        return db::query() << (db::nex.resource.name, db::nex.resource.name);
    }

    db::result resource::type_add(int id, int type_id)
    {
        return db::query() + (db::nex.resource.type.id = type_id, db::nex.resource.id = id);
    }
} // nxs
