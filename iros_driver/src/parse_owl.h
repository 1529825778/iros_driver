#ifndef PARSE_H
#define PARSE_H
#include <string>
#include "common.h"
#include "Capbility.h"
#include "thread_pool.h"
#include <iostream>
#include <glog/logging.h>
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/rdf/map_triple_crtpb.hpp"
typedef struct Regester_parm{
    Capbility* capbility;
    int major;
    int minor;

}Regester_parm;
void parse_owl(std::string file);
void get_Capbility_id();
#endif