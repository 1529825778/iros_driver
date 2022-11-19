#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/map.hpp>
#include "../Capbility.h"

#include "../test_struct.h"

#define ETH "Eth"
#define IP "ip地址"
#define SEND_PORT "发送端口"
#define RECV_PORT "接收端口"

#define USB "Usb"
#define USB_PATH "usb接口标识"

#define SERIALPORT "Serial"
#define SERIAL_PATH "接口标识"
#define BAUD_RATE "波特率"

using namespace std;

enum message_type
{
    register_type = 0,
    reply_t,
    heart_beat_t

};

class UDP_message
{
private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &type;
        ar &ca;
    };
    message_type type;
    Capbility *ca;

public:
    // UDP_message(){}
    // ~UDP_message(){}
    void setType(message_type type)
    {
        this->type = type;
    }
    void setCapbility(Capbility *ca)
    {
        this->ca = ca;
    }
    message_type getMessage_type()
    {
        return this->type;
    }
    Capbility *getCapbility()
    {
        return this->ca;
    }
};

const std::string c_multicast_addr = "239.255.0.1";
const std::string c_listen_addr = "192.168.1.22";
const int c_multicast_port = 30001;
class Asio_Serve
{
private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint local_endpoint_;
    boost::asio::ip::udp::endpoint sender_end_point;
    boost::asio::streambuf receive_stream;
    boost::asio::deadline_timer timer_;

    void do_receive()
    {

        //socket_.async_receive_from(boost::asio::null_buffers(), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        socket_.async_receive_from(receive_stream.prepare(600), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    // void do_send(boost::asio::ip::udp::endpoint endpoint_){
    //     B b("b","b1");
    //     A a("a","A1",&b);
    //     stringstream ss("");
    //     boost::archive::binary_oarchive oa(ss);
    //     oa << a;

    //     socket_.send_to(boost::asio::buffer(ss.str(),ss.str().length()),endpoint_);
    // }

    void handler_receive(const boost::system::error_code &error, std::size_t size)
    {

        if (!error && size >= 0)
        {
            sleep(5);
            // cout<<"avaliable:"<<socket_.available()<<std::endl;
            // if (socket_.available() == 0)
            // {
                

                receive_stream.commit(size);
                
                boost::archive::binary_iarchive ia(receive_stream);
               
                UDP_message message;

                ia >> message;
                
                // cout<<a.getId()<<std::endl;
                // cout<<a.getName()<<std::endl;
                // cout<<a.getB()->getName()<<std::endl;
                // cout<<a.getB()->getId()<<std::endl;
                // cout<<a.getB()->getC()->getName()<<std::endl;
                // cout<<a.getB()->getC()->getId()<<std::endl;

                // cout<<a.getB()->getC()->getPort()->eth.ip<<std::endl;
                //    cout<<a.getB()->getC()->getPort()->eth.receive_port<<std::endl;
                //       cout<<a.getB()->getC()->getPort()->eth.send_port<<std::endl;

                if (message.getMessage_type() == register_type)
                {
                    cout<<"接收到远端能力注册信息"<<std::endl;
                    cout<<"能力名称：" << message.getCapbility()->getName() << endl;
                    cout<<"能力ID:" << message.getCapbility()->getId() << endl;
                    cout<<"感知能力角度范围:" << message.getCapbility()->getAngle() << endl;
                    cout<<"感知能力感知频率:" << message.getCapbility()->getFrequency() << endl;
                    cout<<"感知能力半径：" << message.getCapbility()->getRadious() << endl;
                    cout <<"感知能力分辨率："<< message.getCapbility()->getResoluion() << endl;

                    // cout << (message.getCapbility()->getDevice()==nullptr)<<std::endl;
                    // cout << (message.getCapbility()->getData()==nullptr)<<std::endl;
                    // cout << (message.getCapbility()->getData()->getInterface()==nullptr)<<std::endl;

                    cout <<"感知载荷厂商："<< message.getCapbility()->getDevice()->getManufacture() << endl;
                    cout <<"感知载荷名称："<< message.getCapbility()->getDevice()->getName()<<endl;
                    cout <<"感知载荷类型："<< message.getCapbility()->getDevice()->getType()<<endl;
                    cout << "无人平台厂商:"<<message.getCapbility()->getDevice()->getRobot()->getManufacture() << endl;
                    cout <<"无人平台ID:" <<message.getCapbility()->getDevice()->getRobot()->getId() << endl;
                    cout <<"无人平台类型:"<< message.getCapbility()->getDevice()->getRobot()->getType() << endl;
                    cout <<"无人平台名称："<< message.getCapbility()->getDevice()->getRobot()->getName() << endl;
                    cout <<"无人平台通信地址:"<< dynamic_cast<Eth *>(message.getCapbility()->getDevice()->getRobot()->getInterface())->getIp() << endl;
                    cout <<"无人平台接收端口："<< dynamic_cast<Eth *>(message.getCapbility()->getDevice()->getRobot()->getInterface())->getRecv_port() << endl;
                    cout <<"无人平台发送端口"<<dynamic_cast<Eth *>(message.getCapbility()->getDevice()->getRobot()->getInterface())->getSendport() << endl;

                    cout <<"感知数据描述：" <<message.getCapbility()->getData()->getDescribe() << endl;
                    cout << "感知数据格式："<<message.getCapbility()->getData()->getFormate() << endl;
                    cout << "感知数据长度（0代表不定长数据）:"<<message.getCapbility()->getData()->getLength() << endl;
                    cout << "感知数据解析库:"<<message.getCapbility()->getData()->getLib() << endl;
                    cout <<"感知数据类型:"<< message.getCapbility()->getData()->getType() << endl;

                    
                }
                  if (message.getMessage_type() == heart_beat_t){
                    cout<<"接收到心跳信息帧"<<std::endl;
                  }

                
                socket_.async_receive_from(receive_stream.prepare(600), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            //}
            // else{
            //    socket_.async_receive_from(receive_stream.prepare(socket_.available()), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
               
            // }

            //     string type = (message.getCapbility()->getData()->getInterface()->get_type());

            // cout<<type<<std::endl;
            // if (type.compare(ETH) == 0)
            // {
            //     cout << "ip:" << string(message.getCapbility()->getData()->getInterface()->getPort()->eth.ip)<<endl;
            //     cout << "recv_port:" << message.getCapbility()->getData()->getInterface()->getPort()->eth.receive_port<<endl;
            //     cout << "send_port:" << message.getCapbility()->getData()->getInterface()->getPort()->eth.send_port<<endl;
            // }
            // if (type.compare(USB) == 0)
            // {
            //     cout << "name:" << string(message.getCapbility()->getData()->getInterface()->getPort()->usb.name)<<endl;
            // }
            // if (type.compare(SERIALPORT) == 0)
            // {
            //     cout << "name:" << string(message.getCapbility()->getData()->getInterface()->getPort()->serial_port.name)<<endl;
            //     cout << "Baud_Rate:" << message.getCapbility()->getData()->getInterface()->getPort()->serial_port.Baud_Rate<<endl;
            // }

        }
    }

public:
    Asio_Serve(boost::asio::io_service &io_service) : socket_(io_service), timer_(io_service), local_endpoint_(boost::asio::ip::udp::v4(), c_multicast_port)
    {
        socket_.open(local_endpoint_.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(local_endpoint_);
        socket_.set_option(boost::asio::ip::multicast::join_group(boost::asio::ip::address_v4::from_string(c_multicast_addr)));
        socket_.set_option(boost::asio::ip::multicast::enable_loopback(true));
        do_receive();
    }
    ~Asio_Serve()
    {
        socket_.close();
    }
};

int main()
{
    boost::asio::io_service io_service;
    Asio_Serve server(io_service);
    io_service.run();
}

// class server
// {
// public:
//   server(boost::asio::io_service& io_service, short port)
//     : socket_(io_service, udp::endpoint(udp::v4(), port))
//   {
//     do_receive();
//   }

//   void do_receive()
//   {
//     socket_.async_receive_from(
//         boost::asio::buffer(data_, max_length), sender_endpoint_,
//         [this](boost::system::error_code ec, std::size_t bytes_recvd)
//         {
//           if (!ec && bytes_recvd > 0)
//           {
//             cout<<"123\n"<<std::endl;
//             do_send(bytes_recvd);
//           }
//           else
//           {
//             do_receive();
//           }
//         });
//   }

//   void do_send(std::size_t length)
//   {
//     socket_.async_send_to(
//         boost::asio::buffer(data_, length), sender_endpoint_,
//         [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
//         {
//           do_receive();
//         });
//   }

// private:
//   udp::socket socket_;
//   udp::endpoint sender_endpoint_;
//   enum { max_length = 1024 };
//   char data_[max_length];
// };

// int main(int argc, char* argv[])
// {
//   try
//   {
//     if (argc != 2)
//     {
//       std::cerr << "Usage: async_udp_echo_server <port>\n";
//       return 1;
//     }

//     boost::asio::io_service io_service;

//     server s(io_service, std::atoi(argv[1]));

//     io_service.run();
//   }
//   catch (std::exception& e)
//   {
//     std::cerr << "Exception: " << e.what() << "\n";
//   }

//   return 0;
// }
