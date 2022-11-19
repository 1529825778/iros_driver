#ifndef UDP_MESSAGE_H
#define UDP_MESSAGE_H
#include "Capbility.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
enum message_type
{
    register_type = 0,
    reply_t,
    heart_beat_t

};

class UDP_message
{
private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &type;
        ar &ca;
    };
    message_type type;
    Capbility *ca;

public:
    // UDP_message(){}
    // ~UDP_message(){}
    void setType(message_type type)
    {
        this->type = type;
    }
    void setCapbility(Capbility *ca)
    {
        this->ca = ca;
    }
    message_type getMessage_type()
    {
        return this->type;
    }
    Capbility *getCapbility()
    {
        return this->ca;
    }
};
#endif