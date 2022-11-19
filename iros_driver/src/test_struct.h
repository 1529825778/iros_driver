#ifndef TEST_STRUCT_H
#define TEST_STRUCT_H
#include<string>
#include "Interface.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
using namespace std; 
class C{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & name;
            ar & id;
            //ar & port;
        };
        string name;
        string id;
        
    public:
        C(){
            
        }
        C(string name,string id){
            this->name = name;
            this->id = id;
           
        }
        string getName(){
            return this->name;
        }
        string getId(){
            return this->id;
        }
        
 
};

class B{
    private:

        friend class boost::serialization::access;
        template<class Archive>

        void serialize(Archive & ar,const unsigned int version){
            ar & name;
            ar & id;
            ar & c;
        };
        string name;
        string id;
        C* c;

    public:
        B(){}
        B(string name,string id){
            this->name = name;
            this->id = id;
        }
        void setC(C* c){
            this->c =c;
        }

        string getName(){
            return this->name;
        }
        string getId(){
            return this->id;
        }
        C* getC(){
            return this->c;
        }
};

class A{
    private:

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar,const unsigned int version){
            ar & name;
            ar & id;
            ar & b;
        };
       
        string name;
        string id;
        B* b;

        


    public:
        A(){}
        A(string name,string id){
            this->name = name;
            this->id = id;
        }
        void setB(B* b){
            this->b = b;
        }
        string getName(){
            return this->name;
        }
        string getId(){
            return this->id;
        }
        B* getB(){
            return this->b;
        }

};
#endif
