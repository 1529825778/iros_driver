#include "Capbility.h"
#include "parse_owl.h"
#include "common.h"
#include <boost/archive/binary_iarchive.hpp>
#include "Asio_client.h"
#include <sstream>


Driver_mng* driver_mng;
Device_mng* device_mng;
Threadpool* pool;

Robot* global_robot;
Asio_Client* global_client;
boost::asio::io_service* global_client_service;
set<int>* major_set;
map<string,vector<Plateform_node*>*>* plateform_map;


void get_exit_id(){
  /*获取系统中已经存在的设备编号*/
    string command = "cat /proc/devices |awk '/[0-9]/ {print $1}'";
    char result[1024];
    FILE *fp;
    fp = popen(command.c_str(), "r");
    int nread = fread((void *)result, 1, 1024, fp);
    cout << "size:" << nread << std::endl;
    const char *d = "\n";
    char *p;
    p = strtok(result, d);
    while (p)
    {
        major_set->insert(atoi(p));
        p = strtok(NULL, d);
    }
    pclose(fp);
}

void broadcast_capbility(void* arg){
    UDP_message* udp_message = new UDP_message();
    for(auto it:*(driver_mng->getCapbilityMap())){
          printf("register finished 12312321!\n");
        udp_message->setType(register_type);
          printf("register finished 12312321!\n");
        udp_message->setCapbility(it.second);
          printf("register finished 12312321!\n");
          
        global_client->do_send(udp_message);
        //global_client_service->run();


    }
    delete udp_message;
    sleep(3);
    udp_message = new UDP_message();
    udp_message->setType(heart_beat_t);
    //udp_message->setCapbility(new Capbility());
     global_client->do_send_heart_beat(udp_message);
     global_client_service->run();

}


int main(int argc,const char* argv[]) {
    FLAGS_logtostderr = true;
    FLAGS_log_dir = "logs";
    google::InitGoogleLogging(argv[0]);
    driver_mng = new Driver_mng();
    device_mng = new Device_mng();
    pool = new Threadpool(5,10,15);
    plateform_map = new map<string,vector<Plateform_node*>*>();
    major_set = new set<int>();
    parse_owl("/home/linman/iros_driver/config/Robot2.owl");
    printf("register finished!\n");
    
    boost::asio::io_service client_service;
    global_client_service = &client_service;
    Asio_Client client(client_service,"127.0.0.1",30001);
    global_client = &client;
   

    //io_service.run();
    pool->threadPoolAdd(broadcast_capbility,NULL);
    cout<<"end!!!!!"<<std::endl;
    





    while(1);
    

}





