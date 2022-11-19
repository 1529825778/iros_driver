
#ifndef CAPBILITY_H
#define CAPBILITY_H
#include<map>
#include"Data.h"
#include"Robot.h"
#include"Device.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


using namespace std;

struct Capbility_Info{
    string capbility_id;
    string capbility_name;
    string resoultion;
    string radious;
    string angle;
    string frequency;
};
class Capbility{
    private:
     
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & capbility_name;
            ar & capbility_id;
            ar & resolution;
            ar & radious;
            ar & angle;
            ar & frequency;
            ar & payload_device;
            ar & data;
                
        };
        string capbility_name;
        string capbility_id;
        string resolution;
        string radious;
        string angle;
        string frequency;
       
        PayLoad_Device* payload_device;
        Data* data;
       
        


        
    public:
        Capbility(){};
        Capbility(Capbility_Info* capbility_info);
        ~Capbility();
        string getName();
        string getId();
        string getResoluion();
        string getRadious();
        string getAngle();
        string getFrequency();
        PayLoad_Device* getDevice();
        void setDevice(PayLoad_Device* device);
        Data* getData();
        void setData(Data* data);
        
                   

};
#endif