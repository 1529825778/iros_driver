#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
using namespace std;

struct Serial_port{
    string name;
    int baudRate;

};

 struct Eth{
    string name;
    string ip;
    int send_port;
    int receive_portl;

};

struct Usb{
    string name;
};

typedef union Port{
    Eth eth;
    Usb usb;
    Serial_port serial_port;

}Port;

struct Interface_Info{
    string interface_name;
    string interface_type;
};

class Interface{
    private:
        string interface_name;
        string interface_type;
        Port* port;
    public:
        Interface(){};
        Interface(Interface_Info interface_info);
        ~Interface(){};
        string getName();
        string get_type();
        void setPort(Port*);
        Port* getPort();

};
#endif