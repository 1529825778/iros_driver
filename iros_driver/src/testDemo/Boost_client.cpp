#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
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
        A(){}
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
class Asio_Client
{
    private:
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint dest_endpoint_;
        boost::asio::streambuf recv_buff;
        boost::asio::deadline_timer timer_;
        
        void do_receive(){
            boost::asio::ip::udp::endpoint sender_end_point;
                while(true){
                socket_.async_receive_from(recv_buff,sender_end_point,boost::bind(&Asio_Client::handler_receive,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
                }
        }
        void do_send(){
            // B b("b","b1");
            // A a("a","A1",&b);
            // boost::asio::streambuf buff;

            // boost::archive::binary_oarchive oa(buff);
            // oa << a;

            // socket_.send_to(buff,dest_endpoint_);
        }
           

    void handler_receive(const boost::system::error_code& error,std::size_t){
        if(!error || error==boost::asio::error::message_size){
            // A a;
            // boost::archive::binary_iarchive ia(recv_buff);
            // ia >> a;
            
            // cout << a.getName()<<"\n";
            // cout << a.getId()<<"\n";
            // cout << a.getB()->getName()<<"\n";
            // cout << a.getB()->getId()<<"\n";


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          


        }
    }
    public:
        Asio_Client(boost::asio::io_service &io_service):socket_(io_service),timer_(io_service),dest_endpoint_(boost::asio::ip::address::from_string("127.0.0.1"),c_multicast_port){
            do_send();
            
        }
        ~Asio_Client(){
            socket_.close();
        }
};

int main(){
    boost::asio::io_service io_service;
    Asio_Client client(io_service);
    io_service.run();
    

}
    