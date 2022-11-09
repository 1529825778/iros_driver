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
    Data_Info* data_info = new Data_Info;
    Interface_Info* interface_info = new Interface_Info;
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

    owlcpp::Node_id Capbility_node, Device_node, Has_node, Equipe_node, Plateform_node,Transport_node;

    owlcpp::Triple_store::query_b<0, 1, 1, 0>::range r = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), owlcpp::terms::owl_Class::id(),owlcpp::any);
    owlcpp::Triple_store::query_b<0, 1, 1, 0>::range r2, r5;
    owlcpp::Triple_store::query_b<1, 0, 0, 0>::range r3,r7;
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
        if(get_string(t.subj_,store).compare("transport")==0)
            Transport_node = t.subj_;
        
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
        Data* data = new Data();

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
            if (get_string(n.pred_,store).compare(IMPLEMENTBY)==0){

                
                r3 = store.find_triple(n.obj_,owlcpp::any,owlcpp::any,owlcpp::any);
                BOOST_FOREACH(owlcpp::Triple const &m,r3){
                    if(get_string(m.pred_,store).compare(DATA_DESCRIBTION)==0){
                        data_info->data_describe = get_string(m.obj_,store);
                    }
                    if(get_string(m.pred_,store).compare(DATA_FORMATE)==0){
                        data_info->data_formate = get_string(m.obj_,store);
                    }
                    if(get_string(m.pred_,store).compare(DATA_LENGTH)==0){
                        data_info->data_length = atoi(get_string(m.obj_,store).c_str());
                    }
                    if(get_string(m.pred_,store).compare(DATA_HANDLE_LIB)==0){
                        data_info->data_handle_lib = get_string(m.obj_,store);
                    }
                    if(get_string(m.pred_,store).compare(DATA_TYPE)==0){
                        data_info->data_type = get_string(m.obj_,store);
                    }
                    if(get_string(m.pred_,store).compare(TRANSPORTBY)==0){

                        Interface* interface = new Interface();
                        r4 = store.find_triple(m.obj_,owlcpp::terms::rdf_type::id(),owlcpp::any,owlcpp::any);
                        BOOST_FOREACH(owlcpp::Triple const&o,r4){
                            if(get_string(o.obj_,store).compare(ETH)) interface->setType(ETH);
                            if(get_string(o.obj_,store).compare(USB)) interface->setType(USB);
                            if(get_string(o.obj_,store).compare(SERIALPORT)) interface->setType(SERIALPORT);
                        }
                        r7 = store.find_triple(m.obj_,owlcpp::any,owlcpp::any,owlcpp::any);
                        BOOST_FOREACH(owlcpp::Triple const&o,r4){
                            if(interface->get_type().compare(ETH)){
                                if(get_string(o.obj_,store).compare(IP)==0) interface->getPort()->eth.ip=get_string(o.obj_,store);
                                if(get_string(o.obj_,store).compare(SEND_PORT)) interface->getPort()->eth.send_port = atoi(get_string(o.obj_,store).c_str());
                                if(get_string(o.obj_,store).compare(RECV_PORT)) interface->getPort()->eth.receive_port = atoi(get_string(o.obj_,store).c_str());
                            }
                            if(interface->get_type().compare(USB)){
                                if(get_string(o.obj_,store).compare(USB_PATH)==0) interface->getPort()->usb.name=get_string(o.obj_,store);
                            }
                            if(interface->get_type().compare(SERIALPORT)==0){
                                if(get_string(o.obj_,store).compare(SERIAL_PATH)==0) interface->getPort()->serial_port.name=get_string(o.obj_,store);
                                if(get_string(o.obj_,store).compare(BAUD_RATE)==0) interface->getPort()->serial_port.Baud_Rate = atoi(get_string(o.obj_,store).c_str());
                            }
                        }
                        data->setInterface(interface);
                        


                    }
                }
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
        ca->setData(data);

        driver_mng->getCapbilityMap()->emplace(capbility_info->capbility_name + capbility_info->capbility_id, new Capbility(capbility_info));
    }

        for(auto it : *(driver_mng->getCapbilityMap())){
            // cout<<"机器人信息:\n"<<std::endl;
            // cout<<it.second->getRobot()->getManufacture();
            // cout<<it.second->getRobot()->getName();
            // cout<<it.second->getRobot()->getId();
            // cout<<it.second->getRobot()->getType();
            // cout<<it.second->getRobot()->getName();
            // cout<<it.second->getRobot()->getInterface()->getPort()->eth.ip;
            // cout<<"载荷信息:\n"<<std::endl;
            // cout<<it.second->getDevice()->getManufacture();
            // cout<<it.second->getDevice()->getName();
            // cout<<it.second->getDevice()->getId();
            // cout<<it.second->getDevice()->getType();
            cout<<"能力信息:\n"<<std::endl;
            cout<<it.second->getName();
            cout<<it.second->getId();
            cout<<it.second->getAngle();
            cout<<it.second->getFrequency();
            cout<<it.second->getRadious();
            cout<<it.second->getResoluion();
            cout<<"数据信息:\n"<<std::endl;
            cout<<it.second->getData()->getDescribe();
            cout<<it.second->getData()->getFormate();
            cout<<it.second->getData()->getLength();
            cout<<it.second->getData()->getLib();
            cout<<it.second->getData()->getType();
            cout<<"接口信息:\n"<<std::endl;
            string type = it.second->getData()->getInterface()->get_type();
            
            if(type.compare(ETH)==0){
                cout<<"ip:"<<it.second->getData()->getInterface()->getPort()->eth.ip;
                cout<<"recv_port:"<<it.second->getData()->getInterface()->getPort()->eth.receive_port;
                cout<<"send_port:"<<it.second->getData()->getInterface()->getPort()->eth.send_port;
            }
            if(type.compare(USB)==0){
                cout<<"name:"<<it.second->getData()->getInterface()->getPort()->usb.name;
            }
            if(type.compare(SERIALPORT)==0){
                cout<<"name:"<<it.second->getData()->getInterface()->getPort()->serial_port.name;
                cout<<"Baud_Rate:"<<it.second->getData()->getInterface()->getPort()->serial_port.Baud_Rate;
            }

        }
}
