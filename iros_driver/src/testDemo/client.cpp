#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "Message.h"


// class Asio_Client
// {
//     private:
//         boost::asio::ip::udp::socket socket_;
//         boost::asio::ip::udp::endpoint dest_endpoint_;
//         boost::asio::streambuf recv_buff;
//         boost::asio::deadline_timer timer_;
        
//         void do_receive(){
//             boost::asio::ip::udp::endpoint sender_end_point;
//                 while(true){
//                 socket_.async_receive_from(recv_buff,sender_end_point,boost::bind(&Asio_Client::handler_receive,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
//                 }
//         }
//         void do_send(){
//             // B b("b","b1");
//             // A a("a","A1",&b);
//             // boost::asio::streambuf buff;

//             // boost::archive::binary_oarchive oa(buff);
//             // oa << a;

//             // socket_.send_to(buff,dest_endpoint_);
//         }
           

//     void handler_receive(const boost::system::error_code& error,std::size_t){
//         if(!error || error==boost::asio::error::message_size){
//             // A a;
//             // boost::archive::binary_iarchive ia(recv_buff);
//             // ia >> a;
            
//             // cout << a.getName()<<"\n";
//             // cout << a.getId()<<"\n";
//             // cout << a.getB()->getName()<<"\n";
//             // cout << a.getB()->getId()<<"\n";


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          


//         }
//     }
//     public:
//         Asio_Client(boost::asio::io_service &io_service):socket_(io_service),timer_(io_service),dest_endpoint_(boost::asio::ip::address::from_string("127.0.0.1"),c_multicast_port){
//             do_send();
            
//         }
//         ~Asio_Client(){
//             socket_.close();
//         }
// };


 
void read_buf(boost::asio::ip::tcp::socket& s,boost::asio::streambuf& buf)
{
    int size = 0;
    boost::asio::read(s, boost::asio::buffer(&size, sizeof(int)));
    if (size == 0)
    {
        std::cout << "No data...." << std::endl;
        return;
    }
    boost::asio::read(s, buf, boost::asio::transfer_exactly(size));
}
 
void write_buf(boost::asio::ip::tcp::socket& s, 
    boost::asio::streambuf& buf)
{
    int size = buf.size();
    cout<<"size:"<<size<<"\n";
    boost::asio::write(s, boost::asio::buffer(&size, sizeof(int)));
    boost::asio::write(s, buf, boost::asio::transfer_exactly(size));
}
 
int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }
 
    boost::asio::io_service io_service;
 
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), argv[1], argv[2]);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
 
    boost::asio::ip::tcp::socket s(io_service);
    boost::asio::connect(s, iterator);
 
    Message* message=new Message();
    int num;
    string str;
    std::cout << "Enter a number:";
    std::cin >> num;
    std::cout << "Enter a message:";
    std::cin >> str;
    message->setNum(num);
    message->setStr(str);
 
    boost::asio::streambuf buf1;
    boost::archive::binary_oarchive oa(buf1);
    oa << message;
    write_buf(s, buf1);
 
    boost::asio::streambuf buf2;
    read_buf(s, buf2);
 
    boost::archive::binary_iarchive ia(buf2);
    int rep_num = 0;
    ia >> rep_num;
    for (int i = 0; i < rep_num; ++i)
    {
        Message* recv_msg=new Message();
        ia >> recv_msg;
        std::cout << "Number:" << recv_msg->getNum();
        std::cout << "Message:" << recv_msg->getStr() << std::endl;
    }
 
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
 
  return 0;
}