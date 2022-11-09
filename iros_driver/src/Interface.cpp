#include "Interface.h"

Interface::Interface(){
    this->port = (Port*)malloc(sizeof(Port));
}
Interface::Interface(string interface_type){
    this->interface_type = interface_type;
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

void Interface::setType(string interface_type){
    this->interface_type = interface_type;
}
