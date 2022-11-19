#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/map.hpp>
#include "Capbility.h"
#include "UDP_message.h"
#include "parse_owl.h"


#define ETH "Eth"
#define IP "ip地址"
#define SEND_PORT "发送端口"
#define RECV_PORT "接收端口"

#define USB "Usb"
#define USB_PATH "usb接口标识"

#define SERIALPORT "Serial"
#define SERIAL_PATH "接口标识"
#define BAUD_RATE "波特率"


const std::string c_multicast_addr = "239.255.0.1";
const std::string c_listen_addr = "192.168.1.22";
const int c_multicast_port = 30001;

using namespace std;
class Asio_Serve
{
private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint local_endpoint_;
    boost::asio::ip::udp::endpoint sender_end_point;
    boost::asio::streambuf receive_stream;
    boost::asio::deadline_timer timer_;

    void do_receive();

    void do_reply( boost::asio::ip::udp::endpoint);

    void handler_receive(const boost::system::error_code &error, std::size_t size);

public:
    Asio_Serve(boost::asio::io_service &io_service);
    ~Asio_Serve();
};

int main()
{
    boost::asio::io_service io_service;
    Asio_Serve server(io_service);
    io_service.run();
}
