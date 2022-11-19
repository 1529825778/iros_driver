#include "Asio_client.h"
#include "test_struct.h"

void Asio_Client::handler_send(UDP_message* udp_message,const boost::system::error_code& error,std::size_t bytes_transferred){
               if(!error){
                timer_.expires_from_now(std::chrono::seconds(1));
                timer_.async_wait([this,udp_message](const boost::system::error_code &error){
                    if(error){
                    cout<<error.message()<<std::endl;
                    }
                     else{
                    this->do_send_heart_beat(udp_message);
                    }

                });
                    
               }
        }
Asio_Client::Asio_Client(boost::asio::io_service &io_service,string ip,int port):socket_(io_service),timer_(io_service),dest_endpoint_(boost::asio::ip::address::from_string(ip),port){
            socket_.open(dest_endpoint_.protocol());
        }
Asio_Client::~Asio_Client(){
            socket_.close();
        }
        /*广播能力信息帧,对能力信息进行注册*/
void Asio_Client::do_send(UDP_message* udp_message){        
            boost::asio::streambuf buff;
            boost::archive::binary_oarchive oa(buff);
            cout << (udp_message->getCapbility()->getDevice()==nullptr)<<std::endl;
            cout << (udp_message->getCapbility()->getData()==nullptr)<<std::endl;
            cout << (udp_message->getCapbility()->getData()->getInterface()==nullptr)<<std::endl;
            oa << *udp_message;
            int ret = socket_.send_to(buff.data(),dest_endpoint_);
        }

        /*每秒钟发送一次心跳帧，表示该节点存活*/
void Asio_Client::do_send_heart_beat(UDP_message* udp_message){
            cout<<udp_message->getMessage_type()<<std::endl;
            cout<<(udp_message->getCapbility()==nullptr)<<std::endl;
            boost::asio::streambuf buff;
          
            boost::archive::binary_oarchive oa(buff);
           
            oa << *udp_message;
            socket_.async_send_to(buff.data(),dest_endpoint_,boost::bind(&Asio_Client::handler_send,this,udp_message,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
        }





    
