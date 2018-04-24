#include <nxs/network/protocol/http.hpp>
#include <nxs/network/connexion/basic.hpp>
#include <nxs/network/buffer.hpp>
#include <nxs/error.hpp>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace nxs{namespace network
{
    // read from input
    template<>
    void http<io::input>::read()
    {
        try {
        std::string str_data = std::string(connexion().buffer().data(), connexion().buffer().size());
        if (str_data.find("/favicon.ico") != std::string::npos)
        {
            output().set("nxs::response;");
            output().file_add("nex.ico");
            send();
            return;
        }

        // get request from GET /request...
            size_t s = str_data.find("/", 0) + 1;
        size_t e = str_data.find("HTTP", 0) - 1;
        std::string str_request = str_data.substr(s, e - s) + ";";


        input().set(str_request);
        input().validate();
        output().set("nxs::response;");
        transfer_complete(true);

        process();

        } catch (const std::exception& e) { error(e.what()); }
    }

    // send to input
    template<>
    void http<io::input>::send()
    {
        std::string type = "text/html";
        std::string header_file = "";

        if (!output().data_count())
        {
            send_string("<i>no output</i>");
            return;
        }
        const network::data& output_data = output().data(0);

        if (output_data.target() == network::data::hdd)
        {
            fs::path p(output_data.get<std::string>());
            header_file = "Content-Disposition: attachment; filename=" + p.filename().generic_string() + "\r\n";
        }

        // make header
        std::string header = "HTTP/1.1 200 OK\r\n"
        "Content-Type:" + type + "\r\n" +
        header_file +
        "Content-Length:" + std::to_string(output_data.size()) +
        "\r\n\r\n";

        // send header
        connexion().send(std::move(header));

        // send data
        if (output_data.target() == network::data::memory)
        {
            connexion().send(output().data_ptr(0));
        }
        else
        {
            /*
            std::ifstream file(output_data.get<std::string>(), std::ios::binary | std::ios::in);
            if (!file.is_open()) nxs_error << "data_hdd_read";
            std::array<char, 1024> buffer;
            while (!file.eof())
            {
                file.read(buffer.data(), 1024);
                connexion().send(buffer.data(), file.gcount());
            }
            file.close();*/
        }
    }

    // send string to input
    template<>
    void http<io::input>::send_string(std::string&& data)
    {
        std::string header = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length:" + std::to_string(data.size()) +
        "\r\n\r\n";

        // send header
        connexion().send(std::move(header));
        connexion().send(std::move(data));
    }


    // read from output
    template<>
    void http<io::output>::read()
    {
        try {
        std::cout << "\nGET HTTP DATA FROM OUTPUT MODE : \n";
        std::cout.write(connexion().buffer().data(), connexion().buffer().size());


        } catch (const std::exception& e) { return error(e.what()); }
    }

    // send to output
    template<> void http<io::output>::send()
    {

    }
}} // nxs
