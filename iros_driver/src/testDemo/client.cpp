#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "Message.h"
 
void read_buf(boost::asio::ip::tcp::socket& s,
    boost::asio::streambuf& buf)
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