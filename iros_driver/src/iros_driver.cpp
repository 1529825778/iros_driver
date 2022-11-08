#include "Capbility.h"
#include "parse_owl.h"
#include "common.h"



Driver_mng* driver_mng;
Device_mng* device_mng;


int main(int argc, char* argv[]) {
    FLAGS_logtostderr = true;
    FLAGS_log_dir = "logs";
    google::InitGoogleLogging(argv[0]);
    driver_mng = new Driver_mng();
    device_mng = new Device_mng();
    parse_owl("/home/linman/iros_driver/config/Robot2.owl");

}
