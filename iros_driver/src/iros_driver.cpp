#include "Capbility.h"
#include "parse_owl.h"

class Driver_mng{
    private:
        map<string,Capbility> capbilityMap;
    public:
        void setCapbilityMap(map<string,Capbility>);
        map<string,Capbility> getCapbilityMap(); 

};

extern Driver_mng* driver_mng;
int main(int argc, char* argv[]) {
    FLAGS_logtostderr = true;
    FLAGS_log_dir = "logs";
    google::InitGoogleLogging(argv[0]);
    driver_mng = new Driver_mng();
    parse_owl("../config/Robot4.owl");

}
