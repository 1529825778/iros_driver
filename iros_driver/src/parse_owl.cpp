
#include "parse_owl.h"

void dev_open(fuse_req_t req, struct fuse_file_info *fi)
{
    fuse_reply_open(req, fi);
}
void dev_read(fuse_req_t req, size_t size, off_t off, struct fuse_file_info *fi)
{
}

void dev_write(fuse_req_t req, const char *buf, size_t size, off_t off, struct fuse_file_info *fi)
{
}
void dev_ioctl(fuse_req_t req, int cmd, void *arg, struct fuse_file_info *fi, unsigned flags, const void *in_buf, size_t in_bufsz, size_t out_bufsz)
{
}

const struct cuse_lowlevel_ops dev_clop = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .ioctl = dev_ioctl,
};

/*生成在系统中未被注册的主设备编号*/
int get_Capbility_Major_id()
{
    srand(time(0));
    
    int id = rand() % 300;
    while (major_set->find(id) != major_set->end())
    {
        srand(time(0));
        id = rand() % 300;
    }
    return id;
}

void register_Capbility(void *capbility)
{
    
    struct cuse_info ci;
   
    memset(&ci, 0, sizeof(ci));
   
    Regester_parm* param = (Regester_parm*)capbility;
   
    string robot_name = param->capbility->getDevice()->getRobot()->getName();
    
    string robot_id = param->capbility->getDevice()->getRobot()->getId();
   
    string robot_key = robot_name+robot_id;
   
    robot_key.erase(remove(robot_key.begin(),robot_key.end(),'"'),robot_key.end());
   
    string capbility_key = param->capbility->getName()+param->capbility->getId();
    
   
    capbility_key.erase(remove(capbility_key.begin(),capbility_key.end(),'"'),capbility_key.end());
    cout<<"12345"<<endl;
    string filename = "DEVNAME=/"+robot_key + "/" + capbility_key;
    const char *devname[] = {filename.c_str()};
    //const char *devname[] = {"DEVNAME=/kv6001/Vlight_Perception1"};
    ci.dev_major = param->major;
    ci.dev_minor = param->minor;
    ci.dev_info_argc = 1;
    ci.dev_info_argv = devname;
    ci.flags = CUSE_UNRESTRICTED_IOCTL;
    std::string opt1 = "~/fuse";
    std::string opt2 = "-f";
    char *opts[] = {(char *)opt1.c_str(), (char *)opt2.c_str()};
    int ret = cuse_lowlevel_main(1, opts, &ci, &dev_clop, NULL);
    printf("%d\n", ret);
     cout<<"123456\n"<<endl;
    // return 0;
}





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
    Data_Info *data_info = new Data_Info;
    Interface_Info *interface_info = new Interface_Info;
    Robot *robot;

    Eth* eth = new Eth();

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

    owlcpp::Node_id Capbility_node, Device_node, Has_node, Equipe_node, Plateform_node, Transport_node;

    owlcpp::Triple_store::query_b<0, 1, 1, 0>::range r = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), owlcpp::terms::owl_Class::id(), owlcpp::any);
    owlcpp::Triple_store::query_b<0, 1, 1, 0>::range r2, r5;
    owlcpp::Triple_store::query_b<1, 0, 0, 0>::range r3, r7;
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
        if (get_string(t.subj_, store).compare("transport") == 0)
            Transport_node = t.subj_;
    }

    /*遍历获取无人平台信息*/
    r = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), Plateform_node, owlcpp::any);

    BOOST_FOREACH (owlcpp::Triple const &m, r)
    {
        robot = new Robot();
        cout << "机器人信息：" << to_string(m.subj_, store) << endl;
        r3 = store.find_triple(m.subj_, owlcpp::any, owlcpp::any, owlcpp::any);
        BOOST_FOREACH (owlcpp::Triple const &o, r3)
        {
            cout << "机器人属性" << to_string(o.pred_, store) << endl;
            if (get_string(o.pred_, store).compare(PLATEFORM_ID) == 0)
            {
                robot_info->id = to_string(o.obj_, store);
            }
            if (get_string(o.pred_, store).compare(PLATEFORM_MANFACTURE) == 0)
            {
                robot_info->manufacture = to_string(o.obj_, store);
            }
            if (get_string(o.pred_, store).compare(PLATEFORM_NAME) == 0)
            {
                robot_info->name = to_string(o.obj_, store);
            }
            if (get_string(o.pred_, store).compare(PLATEFORM_TYPE) == 0)
            {
                robot_info->type = to_string(o.obj_, store);
            }
            if(get_string(o.pred_,store).compare(MULTICAST_IP)==0){
                robot_info->multicast_ip = to_string(o.obj_,store);
            }
            if(get_string(o.pred_,store).compare(MULTICAST_PORT)==0){
                robot_info->multicast_port = atoi(to_string(o.obj_,store).c_str());
            }
            if(get_string(o.pred_,store).compare(ETH_NAME)==0){
                robot_info->eth_name = to_string(o.obj_,store);
            }
            if (get_string(o.pred_, store).compare(HAS) == 0)
            {

                r7 = store.find_triple(o.obj_, owlcpp::any, owlcpp::any, owlcpp::any);
                BOOST_FOREACH (owlcpp::Triple const &l, r7)
                {
                    eth->setType(ETH);
                    cout << "port: " << get_string(o.obj_, store) << "  " << get_string(l.pred_, store) << endl;
                     
                    if (get_string(l.pred_, store).compare(IP) == 0){
                       eth->setIp(get_string(l.obj_, store));
                    }

                    if (get_string(l.pred_, store).compare(SEND_PORT) == 0)
                        eth->setSendport(atoi(get_string(l.obj_, store).c_str()));
                    if (get_string(l.pred_, store).compare(RECV_PORT) == 0)
                         eth->setRecvport(atoi(get_string(l.obj_, store).c_str()));
                    if (get_string(l.pred_, store).compare(ETH_NAME) == 0)
                        eth->setName(get_string(l.obj_, store));
                }
              
            }
        }
        robot->setInterface(eth);
        robot->setInfo(robot_info);
        global_robot = robot;
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

                cout << to_string(n.obj_, store) << endl;
                device_info->manufacture = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(PAYLOAD_NAME) == 0)
            {
                cout << to_string(n.obj_, store) << endl;
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
        cout << device->getRobot()->getName() << endl;
        cout << device->getRobot()->getManufacture() << endl;
        cout << device->getRobot()->getId() << endl;

        string device_key=device_info->name + device_info->id;
        device_key.erase(remove(device_key.begin(),device_key.end(),'"'),device_key.end());
        device_mng->getDeviceMap()->emplace(device_key, device);
    }

    /*解析能力信息*/
    r2 = store.find_triple(owlcpp::any, owlcpp::terms::rdf_type::id(), Capbility_node, owlcpp::any);

    BOOST_FOREACH (owlcpp::Triple const &t, r2)
    {
        Data *data = new Data();

        r3 = store.find_triple(t.subj_, owlcpp::any, owlcpp::any, owlcpp::any);
        BOOST_FOREACH (owlcpp::Triple const &n, r3)
        {

            if (get_string(n.pred_, store).compare(RESOULTION) == 0)
            {

                cout << to_string(n.obj_, store) << endl;
                capbility_info->resoultion = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(RADIOUS) == 0)
            {
                cout << to_string(n.obj_, store) << endl;
                capbility_info->radious = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(ANGLE) == 0)
            {
                cout << to_string(n.obj_, store) << endl;
                capbility_info->angle = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(FREQUENCY) == 0)
            {
                cout << to_string(n.obj_, store) << endl;
                capbility_info->frequency = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(CAPBILITY_NAME) == 0)
            {
                cout << to_string(n.obj_, store) << endl;
                capbility_info->capbility_name = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(CAPBILITY_ID) == 0)
            {
                cout << to_string(n.obj_, store) << endl;
                capbility_info->capbility_id = to_string(n.obj_, store);
            }
            if (get_string(n.pred_, store).compare(IMPLEMENTBY) == 0)
            {

                r3 = store.find_triple(n.obj_, owlcpp::any, owlcpp::any, owlcpp::any);
                BOOST_FOREACH (owlcpp::Triple const &m, r3)
                {
                    cout << (to_string(m.subj_, store)) << " , " << to_string(m.pred_, store) << " , " << to_string(m.obj_, store) << std::endl;
                    if (get_string(m.pred_, store).compare(DATA_DESCRIBTION) == 0)
                    {
                        data_info->data_describe = get_string(m.obj_, store);
                    }
                    if (get_string(m.pred_, store).compare(DATA_FORMATE) == 0)
                    {
                        data_info->data_formate = get_string(m.obj_, store);
                    }
                    if (get_string(m.pred_, store).compare(DATA_LENGTH) == 0)
                    {
                        data_info->data_length = atoi(get_string(m.obj_, store).c_str());
                    }
                    if (get_string(m.pred_, store).compare(DATA_HANDLE_LIB) == 0)
                    {
                        data_info->data_handle_lib = get_string(m.obj_, store);
                    }
                    if (get_string(m.pred_, store).compare(DATA_TYPE) == 0)
                    {
                        data_info->data_type = get_string(m.obj_, store);
                    }
                    if (get_string(m.pred_, store).compare(TRANSPORTBY) == 0)
                    {
                        cout << "Interface!"<<endl;
                        Interface* interface = new Interface();
                        r4 = store.find_triple(m.obj_, owlcpp::terms::rdf_type::id(), owlcpp::any, owlcpp::any);
                        BOOST_FOREACH (owlcpp::Triple const &o, r4)
                        {
                            cout << "Interface: " << get_string(o.obj_, store) << endl;
                            if (get_string(o.obj_, store).compare(ETH) == 0)
                                interface->setType(ETH);
                            if (get_string(o.obj_, store).compare(USB) == 0)
                                interface->setType(USB);
                            if (get_string(o.obj_, store).compare(SERIALPORT) == 0)
                                interface->setType(SERIALPORT);
                        }
                        r7 = store.find_triple(m.obj_, owlcpp::any, owlcpp::any, owlcpp::any);
                        BOOST_FOREACH (owlcpp::Triple const &o, r7)
                        {
                            if (interface->get_type().compare(ETH) == 0)
                            {
                                Eth* eth = dynamic_cast<Eth*>(interface);
                                if (get_string(o.obj_, store).compare(IP) == 0)
                                   eth->setIp(get_string(o.obj_, store));
                                if (get_string(o.obj_, store).compare(SEND_PORT) == 0)
                                    eth->setSendport(atoi(get_string(o.obj_, store).c_str()));
                                if (get_string(o.obj_, store).compare(RECV_PORT) == 0)
                                    eth->setRecvport(atoi(get_string(o.obj_, store).c_str()));
                                if (get_string(o.pred_, store).compare(ETH_NAME) == 0)
                                    eth->setName(get_string(o.obj_, store));
                            }
                            if (interface->get_type().compare(USB) == 0)
                            {
                                 
                                cout << "usb:" << get_string(o.obj_, store) << " " << get_string(o.pred_, store) << endl;

                                if (get_string(o.pred_, store).compare(USB_PATH) == 0)
                                {
                                    cout << "USB:" << get_string(o.obj_, store)<<endl;
                                    
                                    interface->setName(get_string(o.obj_, store));
                                    cout<<"123123\n";
                                }
                            }
                            if (interface->get_type().compare(SERIALPORT) == 0)
                            {
                                cout<<"123123\n";
                                Serialport* serial_port = dynamic_cast<Serialport*>(interface);
                                if (get_string(o.obj_, store).compare(SERIAL_PATH) == 0)
                                    serial_port->setName(get_string(o.obj_, store));
                                if (get_string(o.obj_, store).compare(BAUD_RATE) == 0)
                                    serial_port->setBaud_rate(atoi(get_string(o.obj_, store).c_str()));
                                 cout<<"123123\n";
                            }
                        }

                        cout<<"set interface!!!!!!!!!!!!!!!!!"<<std::endl;

                        data->setInterface(interface);
                    }
                }
                data->setInfo(data_info);
            }
        }
         
        r5 = store.find_triple(owlcpp::any, Has_node, t.subj_, owlcpp::any);
        string name, id;
        BOOST_FOREACH (owlcpp::Triple const &n, r5)
        {
            r3 = store.find_triple(n.subj_, owlcpp::any, owlcpp::any, owlcpp::any);
            BOOST_FOREACH (owlcpp::Triple const &o, r3)
            {

                if (get_string(o.pred_, store).compare(PAYLOAD_NAME) == 0)
                    name = get_string(o.obj_, store);
                if (get_string(o.pred_, store).compare(PAYLOAD_ID) == 0)
                    id = get_string(o.obj_, store);
            }
        }

        
        Capbility *ca = new Capbility(capbility_info);
        
        string key = name+id;
         
        key.erase(remove(key.begin(),key.end(),'"'),key.end());
         
        ca->setDevice((*(device_mng->getDeviceMap()))[key]);
        

        ca->setData(data);
         
        string capbility_key = capbility_info->capbility_name + capbility_info->capbility_id;
         
        capbility_key.erase(remove(capbility_key.begin(),capbility_key.end(),'"'),capbility_key.end());
       
        

        driver_mng->getCapbilityMap()->emplace(key,ca);
        //cout<<"载荷标识:"<<ca->getDevice()->getManufacture()<<"\n";
        //cout << "载荷标识:" << (*(driver_mng->getCapbilityMap()))[capbility_info->capbility_name + capbility_info->capbility_id]->getDevice()->getRobot()->getManufacture() << "\n";
        //cout << "载荷标识:" << ca->getDevice()->getRobot()->getManufacture() << "\n";
    }
    cout << driver_mng->getCapbilityMap()->size() << endl;

    int major = get_Capbility_Major_id();
    int minor = 0;

    for (auto it : *(driver_mng->getCapbilityMap()))
    {
        // cout<<"机器人信息:\n"<<std::endl;
        cout << it.second->getDevice()->getManufacture()<<endl;
        cout << it.second->getDevice()->getRobot()->getManufacture()<<endl;
        cout << it.second->getDevice()->getRobot()->getId()<<endl;
        cout << it.second->getDevice()->getRobot()->getType()<<endl;
        cout << it.second->getDevice()->getRobot()->getName()<<endl;
        cout << dynamic_cast<Eth*>(it.second->getDevice()->getRobot()->getInterface())->getIp()<<endl;
        cout << dynamic_cast<Eth*>(it.second->getDevice()->getRobot()->getInterface())->getRecv_port()<<endl;
        cout << dynamic_cast<Eth*>(it.second->getDevice()->getRobot()->getInterface())->getSendport()<<endl;
        cout << "载荷信息:\n"
             << std::endl;

        cout << it.second->getDevice()->getManufacture()<<endl;
        cout << it.second->getDevice()->getName()<<endl;
        cout << it.second->getDevice()->getId()<<endl;
        cout << it.second->getDevice()->getType()<<endl;
        cout << "能力信息:\n"
             << std::endl;
        cout << it.second->getName()<<endl;
        cout << it.second->getId()<<endl;
        cout << it.second->getAngle()<<endl;
        cout << it.second->getFrequency()<<endl;
        cout << it.second->getRadious()<<endl;
        cout << it.second->getResoluion()<<endl;
        cout << "数据信息:\n"
             << std::endl;
        cout << it.second->getData()->getDescribe()<<endl;
        cout << it.second->getData()->getFormate()<<endl;
        cout << it.second->getData()->getLength()<<endl;
        cout << it.second->getData()->getLib()<<endl;
        cout << it.second->getData()->getType()<<endl;
        cout <<"is null?"<< (it.second->getData()->getParamter()==nullptr)<<endl;
        cout << "接口信息123213:\n"<< std::endl;
        cout<<123123<<std::endl;
        cout<<(it.second->getData()->getInterface()->get_type())<<std::endl;
        string type = (it.second->getData()->getInterface()->get_type());
        cout<<(it.second->getData()->getInterface()==NULL);
        cout<<type<<std::endl;
        if (type.compare(ETH) == 0)
        {
            cout << "ip:" << dynamic_cast<Eth*>(it.second->getDevice()->getRobot()->getInterface())->getIp()<<endl;
            cout << "recv_port:" << dynamic_cast<Eth*>(it.second->getDevice()->getRobot()->getInterface())->getRecv_port()<<endl;
            cout << "send_port:" << dynamic_cast<Eth*>(it.second->getDevice()->getRobot()->getInterface())->getSendport()<<endl;
        }
        if (type.compare(USB) == 0)
        {
            cout << "name:" << string(it.second->getData()->getInterface()->getName())<<endl;
        }
        if (type.compare(SERIALPORT) == 0)
        {
            cout << "name:" << string(it.second->getData()->getInterface()->getName())<<endl;
            cout << "Baud_Rate:" << dynamic_cast<Serialport*>(it.second->getDevice()->getRobot()->getInterface())->getBaud_rate()<<endl;
        }
       
        Regester_parm* parm = new Regester_parm;
      
        parm->capbility = it.second;
        
        parm->major=major;
       
        parm->minor=minor++;
        
        pool->threadPoolAdd(register_Capbility,(void*)parm);
    }
}



