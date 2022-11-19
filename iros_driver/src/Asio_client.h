
#ifndef ASIO_CLIENT_H
#define ASIO_CLIENT_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/asio/steady_timer.hpp>
#include "Capbility.h"
#include<glog/logging.h>
using namespace std;
const std::string c_multicast_addr = "239.255.0.1";
const std::string c_listen_addr = "192.168.1.22";
const int c_multicast_port = 30001;
#include "UDP_message.h"

class Asio_Client
{
    private:
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint dest_endpoint_;
        boost::asio::streambuf recv_buff;
        boost::asio::steady_timer timer_;

        void handler_send(UDP_message* ,const boost::system::error_code& ,std::size_t);
        void handler_timeout(UDP_message*,const boost::system::error_code& ,std::size_t);
    public:
        Asio_Client(boost::asio::io_service &io_service,string ip,int port);
        ~Asio_Client();

        /*广播能力信息帧,对能力信息进行注册*/
        void do_send(UDP_message*);

        /*每秒钟发送一次心跳帧，表示该节点存活*/
        void do_send_heart_beat(UDP_message*);

};

#endif


    