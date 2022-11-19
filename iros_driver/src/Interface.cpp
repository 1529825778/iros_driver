#include "Interface.h"

Interface::Interface(){
}
Interface::Interface(string interface_type){
    this->interface_type = interface_type;
}

 string Interface::get_type(){
    return this->interface_type;
}

string Interface::getName(){
    return this->name;
}

void Interface::setName(string name){
    this->name = name;

}

void Interface::setType(string interface_type){
    this->interface_type = interface_type;
}

string Eth::getIp(){
            return this->ip;
        }
int Eth:: getSendport(){
            return this->send_port;
        }
int Eth:: getRecv_port(){
            return this->receive_port;
        }
void Eth:: setIp(string ip){
            this->ip = ip;
        }
void Eth:: setSendport(int port){
            this->send_port = port;
        }
void Eth:: setRecvport(int port){
            this->receive_port = port;
        }

BOOST_CLASS_EXPORT_GUID(Eth,"Eth")
 int Serialport::getBaud_rate(){
            return this->Baud_rate;    
        }
 void Serialport::setBaud_rate(int Baud_rate){
            this->Baud_rate = Baud_rate;
        }
BOOST_CLASS_EXPORT_GUID(Serialport,"Serialport")