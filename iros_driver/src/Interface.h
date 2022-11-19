#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include"boost/serialization/split_member.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>


using namespace std;


struct Interface_Info{
    string interface_type;
};


class Interface{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & interface_type;
            ar & name;
        };
        string interface_type;
        string name;
        
        
    public:
        Interface();
        Interface(string interface_type);
        ~Interface(){};
        virtual string getName();
        virtual string get_type();
        virtual void setName(string name);
        virtual void setType(string interface_type);

};

class Eth : public Interface{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & boost::serialization::base_object<Interface>(*this);
            ar & ip;
            ar & send_port;
            ar & receive_port;
        };
        string ip;
        int send_port;
        int receive_port;
    public:
        string getIp();
        int getSendport();
        int getRecv_port();
        void setIp(string ip);
        void setSendport(int port);
        void setRecvport(int port);
};



// class Usb : public Interface{
//     private:
//         friend class boost::serialization::access;
//             template<class Archive>
//             void serialize(Archive & ar,const unsigned int version){
//             ar & boost::serialization::base_object<Interface>(*this);
//         };
//     public:
//         Usb(){};
// };
class Serialport : public Interface{
    private:
    friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & boost::serialization::base_object<Interface>(*this);
            ar & Baud_rate;
        };
        int Baud_rate;
    public:
        int getBaud_rate();
        void setBaud_rate(int Baud_rate);

};



#endif