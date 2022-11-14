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

class Asio_Serve
{
    private:
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint local_endpoint_;
        std::stringstream receive_stream;
        boost::asio::deadline_timer timer_;
        
        void do_receive(){
            boost::asio::ip::udp::endpoint sender_end_point;
                while(true){
                socket_.async_receive_from(boost::asio::buffer(receive_stream.str()),sender_end_point,boost::bind(&handler_receive,this,receive_stream.str(),boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
                }
        }
        void do_send(boost::asio::ip::udp::endpoint endpoint_){
            B b("b","b1");
            A a("a","A1",&b);
            stringstream ss("");
            boost::archive::binary_oarchive oa(ss);
            oa << a;

            socket_.send_to(boost::asio::buffer(ss.str(),ss.str().length()),endpoint_);
        }
           

    void handler_receive(string receive,const boost::system::error_code& error,std::size_t){
        if(!error || error==boost::asio::error::message_size){
            stringstream ss(receive);
            A a;
            boost::archive::binary_iarchive ia(ss);
            ia >> a;
            
            cout << a.getName()<<"\n";
            cout << a.getId()<<"\n";
            cout << a.getB()->getName()<<"\n";
            cout << a.getB()->getId()<<"\n";


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          


        }
    }
    public:
        Asio_Serve(boost::asio::io_service &io_service):socket_(io_service),timer_(io_service),local_endpoint_(boost::asio::ip::udp::v4(),c_multicast_port){
            socket_.open(local_endpoint_.protocol());
            socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
            socket_.bind(local_endpoint_);
            socket_.set_option(boost::asio::ip::multicast::join_group(boost::asio::ip::address_v4::from_string(c_multicast_addr),boost::asio::ip::address_v4::from_string(c_listen_addr)));
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
    