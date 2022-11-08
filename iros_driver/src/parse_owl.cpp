#include "Capbility.h"
#include "common.h"
#include <iostream>
#include <glog/logging.h>
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/rdf/map_triple_crtpb.hpp"

string get_string(owlcpp::Node_id subj, owlcpp::Triple_store store)
{
    string temp = to_string(subj, store);
    int posId = temp.find("#");
    return temp.substr(posId + 1, temp.length() - posId - 1);
}

owlcpp::Node_id get_Type(owlcpp::Node_id subj, owlcpp::Triple_store store)
{
    owlcpp::Node_id id;
    owlcpp::Triple_store::query_b<1, 1, 0, 0>::range r = store.find_triple(subj, owlcpp::terms::rdf_type::id(), owlcpp::any, owlcpp::any);
    BOOST_FOREACH (owlcpp::Triple const &n, r)
    {
        return n.obj_;
    }
    return id;
}

void parse_owl(string owl_file)
{
    owlcpp::Triple_store store;
    Capbility_Info *capbility_info = new Capbility_Info;
    Device_Info *device_info = new Device_Info;
    Robot_Info *robot_info = new Robot_Info;
    Robot *robot;
    try
    {
        load_file(owl_file, store);
    }
    catch (...)
    {
        LOG(ERROR) << "error loading \"" << owl_file << "\"" << std::endl;
    }
    LOG(INFO)
        << store.map_triple().size() << " triples" << '\n'
        << store.map_node().size() << " nodes" << '\n'
        << store.map_ns().size() << " namespace IRIs" << '\n'
        << std::endl;

    owlcpp::Node_id Capbility_node, Device_node, Has_node, Equipe_node, Plateform_node;

    owlcpp::Triple_store::query_b<0, 1, 1, 0>::range r = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), owlcpp::terms::owl_Class::id(), owlcpp::any);
    owlcpp::Triple_store::query_b<0, 1, 1, 0>::range r2, r5;
    owlcpp::Triple_store::query_b<1, 0, 0, 0>::range r3;
    owlcpp::Triple_store::query_b<1, 1, 0, 0>::range r4;
    owlcpp::Triple_store::query_b<0, 0, 1, 0>::range r6;

    /*遍历所有类*/
    BOOST_FOREACH (owlcpp::Triple const &t, r)
    {
        cout << get_string(t.subj_, store) << std::endl;
        if (get_string(t.subj_, store).compare("Capability") == 0)
            Capbility_node = t.subj_;
        if (get_string(t.subj_, store).compare("Payload_device") == 0)
            Device_node = t.subj_;
        if (get_string(t.subj_, store).compare("Unmaned_plateform") == 0)
            Plateform_node = t.subj_;
    }
    /*遍历所有对象属性*/
    r = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), owlcpp::terms::owl_ObjectProperty::id(), owlcpp::any);
    BOOST_FOREACH (owlcpp::Triple const &t, r)
    {
        if (get_string(t.subj_, store).compare("has") == 0)
            Has_node = t.subj_;
        if (get_string(t.subj_, store).compare("equipe") == 0)
            Equipe_node = t.subj_;
    }

    /*遍历获取无人平台信息*/
    r = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), Plateform_node, owlcpp::any);

    BOOST_FOREACH (owlcpp::Triple const &m, r)
    {
        r3 = store.find_triple(m.subj_, owlcpp::any, owlcpp::any, owlcpp::any);
        BOOST_FOREACH (owlcpp::Triple const &o, r4)
        {
            if (get_string(o.pred_, store).compare(PLATEFORM_ID))
            {
                robot_info->id = to_string(o.obj_, store);
            }
            if (get_string(o.pred_, store).compare(PLATEFORM_MANFACTURE))
            {
                robot_info->manufacture = to_string(o.obj_, store);
            }
            if (get_string(o.pred_, store).compare(PLATEFORM_NAME))
            {
                robot_info->name = to_string(o.obj_, store);
            }
            if (get_string(o.pred_, store).compare(PLATEFORM_TYPE))
            {
                robot_info->type = to_string(o.obj_, store);
            }
        }
        robot = new Robot(robot_info);
    }

    /*解析载荷设备信息*/
    r2 = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), Device_node, owlcpp::any);
    BOOST_FOREACH (owlcpp::Triple const &t, r2)
    {
        r3 = store.find_triple(t.subj_, owlcpp::any, owlcpp::any, owlcpp::any);
        BOOST_FOREACH (owlcpp::Triple const &n, r3)
        {
            cout << (to_string(n.subj_, store)) << " , " << to_string(n.pred_, store) << " , " << to_string(n.obj_, store) << std::endl;
            if (get_string(n.pred_, store).compare(PAYLOAD_MANFACTURE) == 0)
            {

                cout << to_string(n.obj_, store) << "\n";
                device_info->manufacture = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(PAYLOAD_NAME) == 0)
            {
                cout << to_string(n.obj_, store) << "\n";
                device_info->name = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(PAYLOAD_ID) == 0)
            {
                device_info->id = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(PAYLOAD_TYPE) == 0)
            {
                device_info->type = to_string(n.obj_, store);
            }
        }
        PayLoad_Device *device = new PayLoad_Device(device_info);
        device->setRobot(robot);
        device_mng->getDeviceMap()->emplace(device_info->name + device_info->id, new PayLoad_Device(device_info));
    }

    /*解析能力信息*/
    r2 = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), Capbility_node, owlcpp::any);

    BOOST_FOREACH (owlcpp::Triple const &t, r2)
    {

        r3 = store.find_triple(t.subj_, owlcpp::any, owlcpp::any, owlcpp::any);
        BOOST_FOREACH (owlcpp::Triple const &n, r3)
        {
            if (get_string(n.pred_, store).compare(RESOULTION) == 0)
            {

                cout << to_string(n.obj_, store) << "\n";
                capbility_info->resoultion = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(RADIOUS) == 0)
            {
                cout << to_string(n.obj_, store) << "\n";
                capbility_info->radious = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(ANGLE) == 0)
            {
                capbility_info->angle = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(FREQUENCY) == 0)
            {
                capbility_info->frequency = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(CAPBILITY_NAME))
            {
                capbility_info->capbility_name = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(CAPBILITY_ID))
            {
                capbility_info->capbility_id = to_string(n.obj_, store);
            }
        }

        r5 = store.find_triple(owlcpp::any,Has_node,t.subj_,owlcpp::any);
        string name,id;
        BOOST_FOREACH (owlcpp::Triple const &n, r5){
            r3 = store.find_triple(n.subj_,owlcpp::any,owlcpp::any,owlcpp::any);
            BOOST_FOREACH(owlcpp::Triple const &o,r3){
               if(get_string(o.pred_,store).compare(PAYLOAD_NAME)) name = get_string(o.obj_,store);
               if(get_string(o.pred_,store).compare(PAYLOAD_ID)) id = get_string(o.obj_,store); 
            }

        }
        Capbility* ca = new Capbility(capbility_info);
        ca->setDevice((*(device_mng->getDeviceMap()))[name+id]);

        driver_mng->getCapbilityMap()->emplace(capbility_info->capbility_name + capbility_info->capbility_id, new Capbility(capbility_info));
    }
}
