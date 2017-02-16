#ifndef CORE_LOG_H_NXS
#define CORE_LOG_H_NXS

#include <nxs/share.hpp>
#include <nxs/preprocessor.hpp>

#include <string>
#include <vector>
#include <functional>

#define nxs_log nxs::log::add(NXS_LINE, NXS_FUNC_NAME)
#define nxs_warning nxs::log::add(NXS_LINE, NXS_FUNC_NAME) << nxs::log::warning

namespace nxs
{
    class NXS_SHARED log
    {
    public:
        enum type { system, network, database };
        enum level { note, warning, error };
        enum mod { none = 0, list = 1 };

    public:
        static log* instance_;
        static std::vector<std::string> type_str_;
        static std::vector<std::string> level_str_;

        std::string _output;
        level _level;
        std::string _line;
        std::string _func;
        std::function<void(const std::string&)> _redirect;
        bool _pushed;
        bool _mod;

        log();

        void push(const log::type& type);
        void clear();

    public:
        void redirect_output(std::function<void(const std::string&)> fn);

        log& operator<<(const std::string& data);
        log& operator<<(int data);
        log& operator<<(log::mod log_mod);
        log& operator<<(log::level log_level);
        log& operator<<(log::type log_type);

        static log& add(const std::string& line, const std::string& func);
    };
} // nxs

#endif // LOG_H_NXS
