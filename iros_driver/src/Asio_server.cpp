#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Asio_server.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/map.hpp>

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

void Asio_Serve ::do_receive()
{

    socket_.async_receive_from(boost::asio::null_buffers(), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    // socket_.async_receive_from(receive_stream.prepare(600), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
// void do_send(boost::asio::ip::udp::endpoint endpoint_){
//     B b("b","b1");
//     A a("a","A1",&b);
//     stringstream ss("");
//     boost::archive::binary_oarchive oa(ss);
//     oa << a;

//     socket_.send_to(boost::asio::buffer(ss.str(),ss.str().length()),endpoint_);
// }

void Asio_Serve ::handler_receive(const boost::system::error_code &error, std::size_t size)
{
    int minor,major;
    Plateform_node* node;

    if (!error && size >= 0)
    {
        cout << "avaliable:" << socket_.available() << std::endl;
        if (socket_.available() == 0)
        {

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
            //接收到远程注册信息或者回复信息
            if (message.getMessage_type() == register_type ||message.getMessage_type()==reply_t)
            {
                 Capbility* cap = new Capbility(*(message.getCapbility()));

                if(plateform_map->find((sender_end_point.address()).to_string())==plateform_map->end()) {
                    node= new Plateform_node();
                    minor=0;
                    major = get_Capbility_Major_id();
                    node->setMajor(major);
                    major_set->insert(major);
                    node->getList()->emplace_back(cap);
                    plateform_map->emplace((sender_end_point.address()).to_string(),node);
                }
                else {
                    plateform_map->find((sender_end_point.address()).to_string())->second->emplace_back(cap);
                    minor++;
                }
               



                cout << "接收到远端能力注册信息" << std::endl;
                cout << "能力名称：" << message.getCapbility()->getName() << endl;
                cout << "能力ID:" << message.getCapbility()->getId() << endl;
                cout << "感知能力角度范围:" << message.getCapbility()->getAngle() << endl;
                cout << "感知能力感知频率:" << message.getCapbility()->getFrequency() << endl;
                cout << "感知能力半径：" << message.getCapbility()->getRadious() << endl;
                cout << "感知能力分辨率：" << message.getCapbility()->getResoluion() << endl;

                // cout << (message.getCapbility()->getDevice()==nullptr)<<std::endl;
                // cout << (message.getCapbility()->getData()==nullptr)<<std::endl;
                // cout << (message.getCapbility()->getData()->getInterface()==nullptr)<<std::endl;

                cout << "感知载荷厂商：" << message.getCapbility()->getDevice()->getManufacture() << endl;
                cout << "感知载荷名称：" << message.getCapbility()->getDevice()->getName() << endl;
                cout << "感知载荷类型：" << message.getCapbility()->getDevice()->getType() << endl;
                cout << "无人平台厂商:" << message.getCapbility()->getDevice()->getRobot()->getManufacture() << endl;
                cout << "无人平台ID:" << message.getCapbility()->getDevice()->getRobot()->getId() << endl;
                cout << "无人平台类型:" << message.getCapbility()->getDevice()->getRobot()->getType() << endl;
                cout << "无人平台名称：" << message.getCapbility()->getDevice()->getRobot()->getName() << endl;
                cout << "无人平台通信地址:" << dynamic_cast<Eth *>(message.getCapbility()->getDevice()->getRobot()->getInterface())->getIp() << endl;
                cout << "无人平台接收端口：" << dynamic_cast<Eth *>(message.getCapbility()->getDevice()->getRobot()->getInterface())->getRecv_port() << endl;
                cout << "无人平台发送端口" << dynamic_cast<Eth *>(message.getCapbility()->getDevice()->getRobot()->getInterface())->getSendport() << endl;

                cout << "感知数据描述：" << message.getCapbility()->getData()->getDescribe() << endl;
                cout << "感知数据格式：" << message.getCapbility()->getData()->getFormate() << endl;
                cout << "感知数据长度（0代表不定长数据）:" << message.getCapbility()->getData()->getLength() << endl;
                cout << "感知数据解析库:" << message.getCapbility()->getData()->getLib() << endl;
                cout << "感知数据类型:" << message.getCapbility()->getData()->getType() << endl;

                /*注册远端设备能力*/

                string robot_name = message.getCapbility()->getDevice()->getRobot()->getName();

                string robot_id = message.getCapbility()->getDevice()->getRobot()->getId();

                string robot_key = robot_name + robot_id;

                robot_key.erase(remove(robot_key.begin(), robot_key.end(), '"'), robot_key.end());

                string capbility_key = message.getCapbility()->getName() + message.getCapbility()->getId();

                capbility_key.erase(remove(capbility_key.begin(), capbility_key.end(), '"'), capbility_key.end());
                auto it = driver_mng->getCapbilityMap()->find(capbility_key);
                if(it!=driver_mng->getCapbilityMap()->end()){
                      Regester_parm* parm = new Regester_parm;
      
                      parm->capbility = it->second;
        
                      parm->major=node->getMajor();
       
                      parm->minor=minor++;
        
                      pool->threadPoolAdd(register_Capbility,(void*)parm);

                }
                    
            }
            if (message.getMessage_type() == heart_beat_t)
            {
                cout << "接收到心跳信息帧" << std::endl;
            }

            socket_.async_receive_from(boost::asio::null_buffers(), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            socket_.async_receive_from(receive_stream.prepare(socket_.available()), sender_end_point, boost::bind(&Asio_Serve::handler_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
    }
}

void do_reply(boost::asio::ip::udp::endpoint send_endpoint)
{
    UDP_message *udp_message = new UDP_message();
    for (auto it : *(driver_mng->getCapbilityMap()))
    {
        printf("register finished 12312321!\n");
        udp_message->setType(register_type);
        printf("register finished 12312321!\n");
        udp_message->setCapbility(it.second);
        printf("register finished 12312321!\n");

        global_client->do_send(udp_message);
        // global_client_service->run();
    }
}
Asio_Serve::Asio_Serve(boost::asio::io_service &io_service) : socket_(io_service), timer_(io_service), local_endpoint_(boost::asio::ip::udp::v4(), c_multicast_port)
{
    socket_.open(local_endpoint_.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(local_endpoint_);
    socket_.set_option(boost::asio::ip::multicast::join_group(boost::asio::ip::address_v4::from_string(c_multicast_addr)));
    socket_.set_option(boost::asio::ip::multicast::enable_loopback(true));
    do_receive();
}
Asio_Serve ::~Asio_Serve()
{
    socket_.close();
}
