#include <ndb/engine/sql/expression.hpp>
#include <ndb/field.hpp>
#include <ndb/table.hpp>
#include <ndb/query.hpp>

namespace ndb
{
    std::vector<value<sql>> expression<sql>::value_list_;

    expression<sql>::expression(const field_base<sql>& f, typec type) :
        _type(type),
        _output(f.name())
    {
        _table = f.table().name();

        if (f.table().option().is_field())
        {
            const std::string& parent_table = f.table().option().parent().name();
            const std::string& join_table = f.table().name();
            const std::string& alias = f.table().field_name();
            // join by single value field
            if (f.table().option().is_field_single())
            {
                _output = alias + "." + f.name() + " AS " + f.real_name();
                _join = " INNER JOIN `" + join_table + "` " + alias + " ON `" + parent_table + "`.`" + alias + "` = " + alias + "." + join_table + "_id";
            }
            if (f.table().option().is_field_array())
            {
                _table = parent_table + "." + join_table;
                /*
                std::string link = parent_table + "." + join_table;
                _join = " LEFT JOIN `" + link + "` ON `" + link + "`" + ".id = `" + parent_table + "`.id"
                " INNER JOIN `" + join_table + "` ON `" + join_table + "`.id = `" + link + "`.`" + join_table + ".id`";
                */
            }
        }

        if (type == get || type == condition) _output = "`" + _table + "`." + f.name();

        if (type == set || type == add)
        {
            if (f.table().option().is_field_single()) _output = f.table().field_name();
            _output2 = "?" + std::to_string(value_index());
        }
    }

    void expression<sql>::clear()
    {
        value_list_.clear();
        *this = expression();
    }

    const std::string& expression<sql>::output() const { return _output; }
    const std::string& expression<sql>::output2() const { return _output2; }
    const std::string& expression<sql>::output_join() const { return _join; }

    void expression<sql>::append(const expression& expr)
    {
        _output += expr.native();
    }

    std::string expression<sql>::native() const
    {
        std::string str_native = "";

        if (_type == typec::get) str_native = "SELECT " + _output + " FROM `" + _table + "`" + _join;
        else if (_type == typec::add) str_native = "INSERT INTO `" + _table + "`(" + _output + ") VALUES(" + _output2 + ")";
        else if (_type == typec::set) str_native = "REPLACE INTO `" + _table + "`(" + _output + ") VALUES(" + _output2 + ")";
        else if (_type == typec::del) str_native = "DELETE FROM `" + _table + "` WHERE " + _output;
        else if (_type == typec::condition) str_native = " WHERE " + _output;
        else str_native = _output;
        return str_native;
    }

    size_t expression<sql>::value_index() { return value_list_.size() + 1; }

    expression<sql> expression<sql>::all(const table<sql>& t)
    {
        expression<sql> expr;
        expr._output = "*";
        expr._table = t.name();
        expr._type = expression<sql>::get;
        return expr;
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////           OPERATORS            ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    expression<sql>& expression<sql>::operator,(const expression<sql>& other)
    {
        if (_type == typec::join) _output += " " + other.output();
        else _output += "," + other.output();
        if (_type == typec::set || _type == typec::add) _output2 += "," + other.output2();
        return *this;
    }

    expression<sql>& expression<sql>::operator==(const field_base<sql>& f)
    {
        std::string table = f.table().name();
        std::string fname = f.name();
        if (f.table().option().is_field_array())
        {
            table = f.table().option().parent().name() + "." + f.table().name();
            fname = _output;
        }
        _output =  " INNER JOIN `" + _table + "` ON `" + _table + "`." + fname + " = `" + table + "`." + fname;
        return *this;
    }

    template<>
    expression<sql>& expression<sql>::operator==(const query<sql>& q)
    {
        _output += " = (" + q.native() + ")";
        return *this;
    }

    expression<sql>& expression<sql>::operator&&(const expression<sql>& other)
    {
        _output = "(" + _output  + " AND " + other.output() + ")";
        return *this;
    }
} // ndb
