#include "Capbility.h"
#include <iostream>
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"



void parse_owl(string owl_file){
    owlcpp::Triple_store store;
    try{
        load_file(owl_file, store);
        } catch(...) {
        LOG(ERROR) << "error loading \"" << owl_file << "\"" << std::endl;
    }
   LOG(INFO)
   << store.map_triple().size() << " triples" << '\n'
   << store.map_node().size() << " nodes" << '\n'
   << store.map_ns().size() << " namespace IRIs" << '\n'
   << std::endl;

   owlcpp::Node_id Capbility_node;

   owlcpp::Triple_store::query_b<0,1,1,0>::range r = store.find_triple(owlcpp::any,owlcpp::terms::rdf_type::id(),owlcpp::terms::owl_Class::id(),owlcpp::any);
   owlcpp::Triple_store::query_b<0,1,1,0>::range r2,r3 ;
   owlcpp::Triple_store::query_b<1,1,0,0>::range r4;

    BOOST_FOREACH(owlcpp::Triple const& t,r){
        string temp = to_string(t.subj_,store);
        int posId = temp.find("#");
        if((temp.substr(posId+1,temp.length()-posId-1)).compare("Capability")==0) Capbility_node = t.subj_;
        
    }
    r2 = store.find_triple(owlcpp::any,owlcpp::terms::rdf_type::id(),Capbility_node,owlcpp::any);
    r3=store.find_triple(owlcpp::any,owlcpp::terms::rdfs_domain::id(),Capbility_node,owlcpp::any);
    BOOST_FOREACH(owlcpp::Triple const& t,r2){
        BOOST_FOREACH(owlcpp::Triple const& m,r3){
            cout<<to_string(t.subj_)<<"    ,   "<<to_string(m.subj_)<<std::endl;
            r4=store.find_triple(t.subj_,m.subj_,owlcpp::any,owlcpp::any);

        }
    }

}
