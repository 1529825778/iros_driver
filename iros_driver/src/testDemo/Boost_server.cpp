#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
using boost::asio::ip::udp;

using namespace std;

const std::string c_multicast_addr = "239.255.0.1";
const std::string c_listen_addr = "192.168.1.22";
const int c_multicast_port = 30001;
class B{
    private:

        friend class boost::serialization::access;
        template<class Archive>

        void serialize(Archive & ar,const unsigned int version){
            ar & name;
            ar & id;
        };
        string name;
        string id;

    public:
        B(){}
        B(string name,string id){
            this->name = name;
            this->id = id;
        }
        string getName(){
            return this->name;
        };
        string getId(){
            return this->id;
        };
};

class A{
    private:

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & name;
            ar & id;
            ar & b;
        };
       
        string name;
        string id;
        B* b;

        


    public:
        A(){
            b=new B();
        }
        A(string name,string id,B* b){
            this->name = name;
            this->id = id;
            this ->b = b;
        }
        string getName(){
            return this->name;
        }
        string getId(){
            return this->id;
        }
        B* getB(){
            return this->b;
        }

};

class Asio_Serve
{
    private:
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint local_endpoint_;
         boost::asio::ip::udp::endpoint sender_end_point;
        boost::asio::streambuf receive_stream;
        boost::asio::deadline_timer timer_;
        char data[1024];
        
        void do_receive(){
           
            socket_.async_receive_from(receive_stream.prepare(100),sender_end_point,boost::bind(&Asio_Serve::handler_receive,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
                
        }
        // void do_send(boost::asio::ip::udp::endpoint endpoint_){
        //     B b("b","b1");
        //     A a("a","A1",&b);
        //     stringstream ss("");
        //     boost::archive::binary_oarchive oa(ss);
        //     oa << a;

        //     socket_.send_to(boost::asio::buffer(ss.str(),ss.str().length()),endpoint_);
        // }
           

    void handler_receive(const boost::system::error_code& error,std::size_t size){
           
        if(!error && size>0){
            cout<<"size:"<<size<<std::endl;
            receive_stream.commit(size);
            boost::archive::binary_iarchive ia(receive_stream);
            cout<<"size:"<<size<<std::endl;
            A a;
            cout<<"size:"<<size<<std::endl;
            ia >> a;
            cout<<"size:"<<size<<std::endl;
            cout << a.getName()<<"\n";
            cout << a.getId()<<"\n";
            cout << a.getB()->getName()<<"\n";
            cout << a.getB()->getId()<<"\n";
            do_receive();
            
            
           


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          


        }
    }
    public:
        Asio_Serve(boost::asio::io_service &io_service):socket_(io_service),timer_(io_service),local_endpoint_(boost::asio::ip::udp::v4(),c_multicast_port){
            socket_.open(local_endpoint_.protocol());
            socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
            socket_.bind(local_endpoint_);
            socket_.set_option(boost::asio::ip::multicast::join_group(boost::asio::ip::address_v4::from_string(c_multicast_addr)));
            socket_.set_option(boost::asio::ip::multicast::enable_loopback(true));
            do_receive();
            
        }
        ~Asio_Serve(){
            socket_.close();
        }
};

int main(){
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
    