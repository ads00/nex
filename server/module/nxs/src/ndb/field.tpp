#include <ndb/type.hpp>
#include <ndb/expression.hpp>

namespace ndb
{
    template<class Engine> class expression;

    template<class T, size_t Size, class Engine, typename Field_Basic>
    field<T, Size, Engine, Field_Basic>::field(const ndb::table<Engine>& table) :
        field_base<Engine>(table, field_base<Engine>::auto_name(), Size, ndb::type<Engine>::template id<T>())
    {}

    template<class T, size_t Size, class Engine, typename Field_Basic>
    field<T, Size, Engine, Field_Basic>::field(const ndb::table<Engine>& table,
                                               const std::string& name,
                                               const T& default_value) :
        field_base<Engine>(table, name, Size, ndb::type<Engine>::template id<T>()),
        _default_value(default_value)
    {}

    template<class T, size_t Size, class Engine, typename Field_Basic>
    field<T, Size, Engine, Field_Basic>::field(const ndb::table<Engine>& table,
                                               const std::string& name,
                                               const field_option<Engine>& option) :
        field_base<Engine>(table, name, Size, ndb::type<Engine>::template id<T>(), option)
    {}

    template<class T, size_t Size, class Engine, typename Field_Basic>
    T field<T, Size, Engine, Field_Basic>::default_value() const
    {
        return _default_value;
    }

    template<class T, size_t Size, class Engine, typename Field_Basic>
    template<class V>
    expression<Engine> field<T, Size, Engine, Field_Basic>::operator,(const V& other) const
    {
        expression<Engine> test(*this, expression<sql>::get);
        return test , other;
    }

    template<class T, size_t Size, class Engine, typename Field_Basic>
    template<class V>
    expression<Engine> field<T, Size, Engine, Field_Basic>::operator=(const V& other) const
    {
        expression<Engine> test(*this, expression<sql>::set);
        return test = other;
    }

    template<class T, size_t Size, class Engine, typename Field_Basic>
    template<class V>
    expression<Engine> field<T, Size, Engine, Field_Basic>::operator==(const V& other) const
    {
        expression<Engine> test(*this);
        return test == other;
    }
} // ndb
