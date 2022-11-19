
#ifndef Robot_H
#define Robot_H
#include<string>
#include"Interface.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace std;

typedef struct Robot_Info {
    string name;
    string id;
    string manufacture;
    string type;
    string multicast_ip;
    int multicast_port;
    string eth_name;
}Robot_Info;

class Robot{
    private:

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & name;
            ar & id;
            ar & manufacture;
            ar & type;
            ar & multicast_ip;
            ar & multicast_port;
            ar & eth_name;
            ar & interface;
        };
        string name;
        string id;
        string manufacture;
        string type;
        string multicast_ip;
        int multicast_port;
        string eth_name;
        Interface* interface;
        
        
    public:
        Robot();
        ~Robot(){};
        Robot(Robot_Info*);
       
        string getName();
        string getId();
        string getManufacture();
        string getType();
        string getMulticast_ip();
        string getEth_name();
        int getMulticast_port();
        Interface* getInterface();
        void setInterface(Interface* interface);
        void setInfo(Robot_Info*);
                       

};
#endif