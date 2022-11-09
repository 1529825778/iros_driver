#ifndef DATA_H
#define DATA_H
#include <map>
#include "Interface.h"
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
        int data_length;
        string data_type;
        string data_describe;
        string data_handle_lib;
        string data_formate;
        Interface* interface;
        map<string,string> paramter_map;
    public:
        Data(){};
        Data(Data_Info*);
        ~Data(){};

        int getLength();
        string getType();
        string getDescribe();
        string getLib();
        string getFormate();
        Interface* getInterface();
        void setInterface(Interface*);
        map<string,string> getParamter();
        void setParamter(map<string,string>);
        void setInfo(Data_Info*);
  
     



};


#endif