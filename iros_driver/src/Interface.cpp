#include "Interface.h"
Interface::Interface(Interface_Info interface_info){
    this->interface_name = interface_info.interface_name;
    this->interface_type = interface_info.interface_type;
}

string Interface::getName(){
    return this->interface_name;
}

string Interface::get_type(){
    return this->interface_type;
}



Port* Interface::getPort(){
    return this->port;
}

void Interface::setPort(Port* port){
    this->port = port;

}
