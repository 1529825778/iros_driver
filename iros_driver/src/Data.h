#ifndef DATA_H
#define DATA_H
#include <map>
#include "Interface.h"
#include <boost/serialization/map.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
using namespace std;

struct Data_Info{
    int data_length;
    string data_type;
    string data_handle_lib;
    string data_formate;
    string data_describe;
};

class Data{
    private:
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar,const unsigned int version){
            ar & data_length;
            ar & data_type;
            ar & data_describe;
            ar & data_handle_lib;
            ar & data_formate;
            ar & interface;
            ar & paramter_map;
        };
        int data_length;
        string data_type;
        string data_describe;
        string data_handle_lib;
        string data_formate;
        Interface* interface;
        map<string,string>* paramter_map;


    public:
        Data(){
            paramter_map=nullptr;
        };
        Data(Data_Info*);
        ~Data(){};

        int getLength();
        string getType();
        string getDescribe();
        string getLib();
        string getFormate();
        Interface* getInterface();
        void setInterface(Interface*);
        map<string,string>* getParamter();
        void setParamter(map<string,string>*);
        void setInfo(Data_Info*);
  
     



};


#endif