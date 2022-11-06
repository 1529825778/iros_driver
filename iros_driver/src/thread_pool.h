#ifndef THREAD_H
#define THREAD_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<glog/logging.h>
class Task{
    public:
        void (*function)(void* arg);
        void* arg;
};

class Threadpool{
    public:
        Threadpool(int,int,int);
        ~Threadpool();

        int threadPoolDestroy();
        /*给线程添加任务*/
        void threadPoolAdd(void(*func)(void*),void* arg);

        /*获取线程池中工作的线程个数*/
        int threadPoolBusyNums();

        /*获取线程池中活着的线程个数*/
        int threadPoolLiveNums();

        //工作的线程的任务函数
        static void* worker(void* arg);
        //管理者线程的任务函数
        static void* manager(void* arg);

        //单个线程推出
        void threadExit();




    private:
        Task* TaskQ;
        int queueCapacity; //容量
        int queueSize;     //当前任务个数
        int queueFront;    //队头，取数据
        int queueEnd;      //队尾, 放数据

        pthread_t managerID;  //管理者线程ID
        pthread_t* threadIDs; //工作线程ID

        int minNum;          //最小线程数量
        int maxNum;          //最大线程数量
        int busyNum;         //忙的线程数量
        int liveNum;         //存活的线程个数
        int exitNum;         //要销毁的线程个数

        pthread_mutex_t mutexPool;   //锁整个线程池
        pthread_mutex_t mutexBusy;   //锁busyNum变量
        pthread_cond_t notfull;      //任务队列是不是满了
        pthread_cond_t notEmpty;     //任务队列是不是空了

        int shutdown;                //是不是销毁线程池,销毁为1,不销毁为0


};
#endif