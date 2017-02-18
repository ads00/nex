#include <nxs/network/connexion/output.hpp>
#include <nxs/network/protocol.hpp>
#include <nxs/network/connexion.hpp>
#include <nxs/network/buffer.hpp>

namespace nxs{namespace network
{
    using boost::asio::ip::tcp;
    // static
    boost::asio::io_service output_connexion::ios;

    output_connexion::output_connexion(std::string ip, int port) :
        connexion(protocol::create(this, protocol::nex)),
        _socket(ios), _timer(output_connexion::ios)
    {
        init();
        _ip = ip;
        _port = port;
        connect(_ip, _port);
    }
    output_connexion::~output_connexion()
    {
        _socket.close();
    }

    output_connexion::output_connexion() :connexion(protocol::create(this, protocol::nex)),
      _socket(ios),
      _timer(output_connexion::ios)
    {
        init();
    }

    void output_connexion::init()
    {
        _ip = "127.0.0.1";
        _port = 50;
        _iotype = 0;
        _alive = 0;
        _socket.close();
    }

    output_connexion* output_connexion::create()
    {
        return new output_connexion;
    }

    // callback
    void output_connexion::connect_callback_set(std::function<void()> fn) { _callback_connect = fn; }
    void output_connexion::data_read_callback_set(std::function<void(connexion::buffer_type)> fn) { _callback_data_read = fn; }
    void output_connexion::data_send_callback_set(std::function<void(size_t)> fn) { _callback_data_send = fn; }
    void output_connexion::error_callback_set(std::function<void(const char*)> fn) { _callback_error = fn; }

    void output_connexion::connect(std::string ip, int port, int time_out)
    {
        init();

        _ip = ip;
        _port = port;

        tcp::endpoint endpoint(boost::asio::ip::address::from_string(_ip), _port);
        _socket.async_connect(endpoint, boost::bind(&output_connexion::socket_connect, this, boost::asio::placeholders::error));
    }

    void output_connexion::sync_connect(std::string ip, int port)
    {
        init();
        _ip = ip;
        _port = port;
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(_ip), _port);
        _socket.connect(endpoint);
        _alive = 1;
    }

    // socket connect
    void output_connexion::socket_connect(const boost::system::error_code& status)
    {
        if (!status)
        {
            _alive = 1;
            if (_callback_connect) _callback_connect();
            data_read();
        }
        else socket_error(status);
    }

    // socket read
    void output_connexion::socket_read(const boost::system::error_code& status, size_t bytes_transferred)
    {
        if (!status && _alive)
        {
            _buffer.size(bytes_transferred);
            if (_callback_data_read) _callback_data_read(_buffer);
            // read next data
            data_read();
        }
        else socket_error(status);
    }
    // socket send
    void output_connexion::socket_send(const boost::system::error_code& status, size_t bytes_transferred)
    {
        if (!status && _alive)
        {
            if (_callback_data_send) _callback_data_send(bytes_transferred);
        }
        else socket_error(status);
    }
    // socket error
    void output_connexion::socket_error(const boost::system::error_code& status)
    {
        _alive = 0;
        if (_callback_error) _callback_error(status.message().c_str());
    }

    // data_read
    void output_connexion::data_read()
    {
        if (!_alive) return;
        //clear buffer
        _socket.async_read_some(boost::asio::buffer(_buffer.address(), _buffer.capacity()),
                                boost::bind(&output_connexion::socket_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred)
        );
    }

    void output_connexion::sync_data_read()
    {
        if (!_alive) return;

        while (1)
        {
            boost::system::error_code error;

            int data_size = _socket.read_some(boost::asio::buffer(_buffer.address(), _buffer.capacity()), error);
            _buffer.size(data_size);
            std::cout << "\nGET " << data_size << " ";
            std::cout.write(_buffer.data(), _buffer.size());
            protocol().output_read(_buffer);
            if (protocol().input().data_complete() || error) break;
        }

    }

    void output_connexion::data_send(const char* data, int data_size)
    {
        if (!_alive) return;
        boost::asio::async_write(_socket,
                                 boost::asio::buffer(data, data_size),
                                 boost::bind(&output_connexion::socket_send, this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred)
        );
    }

    void output_connexion::run()
    {
        output_connexion::ios.run();
    }

    const connexion::buffer_type& output_connexion::buffer() const { return _buffer; }

    std::string output_connexion::ip() { return _ip; }
    int output_connexion::port() { return _port; }
}} // nxs::network

