#include "Capbility.h"
#include "parse_owl.h"
#include "common.h"
#include <boost/archive/binary_iarchive.hpp>
#include <sstream>


Driver_mng* driver_mng;
Device_mng* device_mng;
Threadpool* pool;

Robot* global_robot;

int main(int argc, char* argv[]) {
    FLAGS_logtostderr = true;
    FLAGS_log_dir = "logs";
    google::InitGoogleLogging(argv[0]);
    driver_mng = new Driver_mng();
    device_mng = new Device_mng();
    Threadpool* pool = new Threadpool(5,10,15);
    parse_owl("/home/linman/iros_driver/config/Robot2.owl");
    printf("register finished!\n");

    int recv_socket = init_recv_socket(global_robot);


    while(1);
    

}

int init_recv_socket(Robot* robot){
    int recv_socket = -1;
    struct ip_mreqn mreqn;
    bzero(&mreqn,sizeof(mreqn));
    inet_pton(AF_INET,robot->getMulticast_ip().c_str(),&mreqn.imr_multiaddr.s_addr);
    mreqn.imr_address.s_addr = INADDR_ANY;
    mreqn.imr_ifindex = if_nametoindex(robot->getEth_name().c_str());
    struct sockaddr_in _m_local_addr;
    recv_socket = socket(AF_INET,SOCK_DGRAM,0);
    _m_local_addr={0};
    _m_local_addr.sin_family = AF_INET;//IPV4协议
    _m_local_addr.sin_port = htons(robot->getInterface()->getPort()->eth.receive_port);
    _m_local_addr.sin_addr.s_addr = INADDR_ANY;
     if (setsockopt(recv_socket,IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(struct ip_mreqn)) == -1) LOG(ERROR)<<"Setsockopt Multicast failed"<<std::endl;
    int ret = bind(recv_socket,(struct sockaddr*)&_m_local_addr,sizeof(_m_local_addr));
    if(ret==-1) LOG(ERROR)<<"bind faild"<<std::endl;
    return recv_socket;
    
}




