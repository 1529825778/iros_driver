
#define FUSE_USE_VERSION 31
#include<cuse_lowlevel.h>
#include<fuse_opt.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<attr/xattr.h>
#include "thread_pool.h"
#include "ioctl.h"

typedef struct Device{
    char name[128];
    int dev_major;
    int dev_minor;
}Device;


static void dev_open(fuse_req_t req,struct fuse_file_info *fi){
    fuse_reply_open(req,fi);
}

static void dev_read(fuse_req_t req,size_t size,off_t off,struct fuse_file_info *fi){
    

} 

static void dev_write(fuse_req_t req,const char* buf,size_t size,off_t off,struct fuse_file_info *fi){

}


static void dev_ioctl(fuse_req_t req,int cmd,void *arg,struct fuse_file_info *fi,unsigned flags,const void *in_buf,size_t in_bufsz,size_t out_bufsz){

}

static const struct cuse_lowlevel_ops dev_clop={
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .ioctl = dev_ioctl,
};

void regester_Device(void* arg){

   
    Device* device = (Device*)arg;

    struct cuse_info ci;
    memset(&ci,0,sizeof(ci));
   

    const char* devname[] = {device->name};
    ci.dev_major = device->dev_major;
    ci.dev_minor = device->dev_minor;
    ci.dev_info_argc = 1;
    ci.dev_info_argv = devname;
    ci.flags = CUSE_UNRESTRICTED_IOCTL;
    std::string opt1 = "~/fuse";
    std::string opt2 = "-f";
    char* opts[] = {(char*)opt1.c_str(),(char*)opt2.c_str()}; 
    printf("12334\n");
    int ret = cuse_lowlevel_main(1,opts, &ci, &dev_clop, NULL);
    printf("%d\n",ret);
    //return 0;
}

int main(int argc,char **argv){
    FLAGS_logtostderr = true;
    FLAGS_log_dir="logs";
    google::InitGoogleLogging(argv[0]);
    // char dev[128] = "DEVNAME=/dev/fuse_dev";
    // struct cuse_info ci;
    // const char *dev_info_argv[]={dev};
    // printf("1243e23\n");
    // memset(&ci,0,sizeof(ci));
    // ci.dev_major = 101;
    // ci.dev_minor = 12;
    // ci.dev_info_argc = 1;
    // ci.dev_info_argv = dev_info_argv;
    // ci.flags = CUSE_UNRESTRICTED_IOCTL;
    // printf("1243e23\n");
    // cuse_lowlevel_main(argc, argv, &ci, &dev_clop, NULL);
    //  printf("1243e23\n");
    // char ip[15] = "192.168.10.10";
    // int port = 8001;
    // if(setxattr("/dev/dev/fuse_dev","user.ip", ip,strlen(ip),0)==-1){
    //     printf("%s\n",strerror(errno));
    // }
    // setxattr("/dev/dev/fuse_dev","user.port",&port,sizeof(port),0);
    // char t_ip[15];
    // int t_port=0;
    // getxattr("/dev/dev/fuse_dev","user.ip",&t_ip,strlen(t_ip));
    // getxattr("/dev/dev/fuse_dev","user.port",&port,sizeof(port));
    // printf("ip:%s\n",t_ip);
    // printf("port:%d\n",t_port);
    Device device={
        .name = "DEVNAME=/dev/my_dev",
        .dev_major =101,
        .dev_minor= 10,   
    };

    Threadpool* pool = new Threadpool(5,10,15);
     pool->threadPoolAdd(regester_Device,(void*)&device);
     while(1);
    //  pthread_t tid_fuse;
    //  int ret= pthread_create(&tid_fuse,NULL,regester_Device,(void*)&device);
    //  printf("%d\n",ret);
     //regester_Device((void*)&device);
     printf("123\n");
     //pthread_join(tid_fuse,NULL);
    // usleep(1000);
    // char ip[15] = "192.168.10.10";
    // int port = 8001;
    // if(setxattr("/dev/dev/my_dev","user.ip", ip,strlen(ip),0)==-1){
    //     printf("%s\n",strerror(errno));
    // }
    // setxattr("/dev/dev/my_dev","user.port",&port,sizeof(port),0);
    // char t_ip[15];
    // int t_port=0;
    // getxattr("/dev/dev/my_dev","user.ip",&t_ip,strlen(t_ip));
    // getxattr("/dev/dev/my_dev","user.port",&port,sizeof(port));
    // printf("ip:%s\n",ip);
    // printf("port:%d\n",port); 
    google::ShutdownGoogleLogging();

}

