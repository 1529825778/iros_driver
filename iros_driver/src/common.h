#ifndef Common_H
#define Common_H
#include<stdio.h>
#include<string>
#include<map>
#include<glog/logging.h>
#include"Capbility.h"
using namespace std;

#define RESOULTION                   "感知分辨率"
#define RADIOUS                      "感知范围半径"
#define ANGLE                        "感知角度范围"
#define FREQUENCY                    "感知频率"
#define CAPBILITY_NAME               "能力名称"
#define CAPBILITY_ID                 "能力标识"

#define PAYLOAD_MANFACTURE           "载荷厂家"
#define PAYLOAD_NAME                 "载荷名称"
#define PAYLOAD_ID                   "载荷标识"
#define PAYLOAD_TYPE                 "载荷类型" 

#define PLATEFORM_MANFACTURE         "无人平台厂商"
#define PLATEFORM_NAME               "无人平台名称"
#define PLATEFORM_ID                 "无人平台标识"
#define PLATEFORM_TYPE               "无人平台类型"

#define DATA_DESCRIBTION             "数据描述"
#define DATA_FORMATE                 "数据格式"
#define DATA_TYPE                    "数据类型"
#define DATA_LENGTH                  "数据长度"
#define DATA_HANDLE_LIB              "编解码库"

#define IMPLEMENTBY                  "implementBy"
#define TRANSPORTBY                  "transportBy"

#define ETH                          "Eth"
#define IP                           "ip地址"
#define SEND_PORT                    "发送端口"
#define RECV_PORT                    "接收端口"


#define USB                          "Usb"
#define USB_PATH                     "usb接口标识"


#define SERIALPORT                   "Serial"
#define SERIAL_PATH                  "接口标识"
#define BAUD_RATE                    "波特率"
 

class Driver_mng{
    private:
        map<string,Capbility*>* capbilityMap;
    public:
        Driver_mng(){
            capbilityMap = new map<string,Capbility*>();
        };
        void setCapbilityMap(map<string,Capbility*>* map){
            this->capbilityMap = map;
        };
        map<string,Capbility*>* getCapbilityMap(){
            return this->capbilityMap;
        }; 

};

class Device_mng{
    private:
        map<string,PayLoad_Device*>* deviceMap;
    public:
        Device_mng(){
            deviceMap = new map<string,PayLoad_Device*>();
        };
        void setDeviceMap(map<string,PayLoad_Device*>* map){
            this->deviceMap = map;
        }
        map<string,PayLoad_Device*>* getDeviceMap(){
            return this->deviceMap;
        }

};
extern Driver_mng* driver_mng;
extern Device_mng* device_mng;











#endif



