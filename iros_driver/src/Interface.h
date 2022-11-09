#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
using namespace std;


 struct Eth{
    string name;
    string ip;
    int send_port;
    int receive_port;

};

struct Usb{
    string name;
};

struct Serial_port{
    string name;
    int Baud_Rate;
};

typedef union Port{
    Eth eth;
    Usb usb;
    Serial_port serial_port;

}Port;

struct Interface_Info{
    string interface_type;
};

class Interface{
    private:
        string interface_type;
        Port* port;
    public:
        Interface();
        Interface(string interface_type);
        ~Interface(){};
        string getName();
        string get_type();
        void setPort(Port*);
        void setType(string interface_type);
        Port* getPort();

};
#endif