#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
using namespace std;

class Message{
    private:
        int num;
        string str;
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & num;
            ar & str;
        }
    public:
        void setNum(int num){
            this->num = num;
        }
        void setStr(string str){
            this->str = str;
        }
        int getNum(){
            return this->num;
        }
        string getStr(){
            return this->str;
        }


};
#endif