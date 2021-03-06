#ifndef NETWORK_CONNEXION_SETUP_H_NXS
#define NETWORK_CONNEXION_SETUP_H_NXS

#include <nxs/setup.hpp>
#include <cstddef>

namespace nxs
{
    namespace network { class connexion; template<size_t> class buffer; }

    template<>
    struct setup<network::connexion>
    {
        constexpr static std::size_t buffer_size = 4096;
        constexpr static std::size_t output_progress_size = 8192;
        using buffer_type = network::buffer<buffer_size>;
    };
} // nxs

#endif // NETWORK_CONNEXION_SETUP_H_NXS
