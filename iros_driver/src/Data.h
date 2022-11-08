#ifndef DATA_H
#define DATA_H
#include <map>
#include "Interface.h"
using namespace std;

struct Data_Info{
    int data_length;
    string data_type;
    string data_handle_lib;
    string data_describe;
};

class Data{
    private:
        string data_length;
        string data_type;
        string data_describe;
        string data_handle_lib;
        Interface interface;
        map<string,string> paramter_map;
    public:
        Data(){};
        Data(Data_Info);
        ~Data(){};

        int getLength();
        string getType();
        string getDescribe();
        string getLib();
        Interface getInterface();
        void setInterface(Interface);
        map<string,string> getParamter();
        void setParamter(map<string,string>);
  
     



};


#endif