
#include "thread_pool.h"
/*线程池*/ 
 Threadpool::Threadpool(int min,int max,int queueSize){

        this->threadIDs = new pthread_t[max];

        for(int i=0;i<max;i++){
            this->threadIDs[i] = 0;
        }

        this->minNum = min;
        this->maxNum = max;
        this->busyNum = 0;
        this->liveNum = min;
        this->exitNum = 0;
        if(pthread_mutex_init(&this->mutexPool,NULL)!=0 || pthread_mutex_init(&this->mutexBusy,NULL)!=0||pthread_cond_init(&this->notEmpty,NULL)!=0||pthread_cond_init(&this->notfull,NULL)!=0){
            LOG(ERROR)<<"mutex or cond init failed\n"<<std::endl;
        }

        this->TaskQ =new Task[queueSize];
        this->queueCapacity = queueSize;
        this->queueFront = 0;
        this->queueEnd=0;
        this->shutdown = 0;

        //创建线程
        int errnum = pthread_create(&this->managerID,NULL,Threadpool::manager,this);
        if(errnum!=0){
            LOG(ERROR)<<"create manager_thread faild"<<strerror(errno)<<std::endl;
        }
        for(int i=0;i<min;i++){
            errnum = pthread_create(&this->threadIDs[i],NULL,Threadpool::worker,this);
            if(errnum!=0){
                LOG(ERROR)<<"create work thread faild"<<std::endl;
                break;
            }
        }
}

Threadpool::~Threadpool(){
    threadPoolDestroy();

}
/**
 * threadPoolDestroy：销毁线程池
 * threadPool* pool ：需要销毁的线程池
 * 
 **/
int Threadpool::threadPoolDestroy(){
    if(this ==nullptr){ return -1;}
    //关闭线程池
    this->shutdown = -1;
    //阻塞回收管理者线程,等待管理者线程结束
    pthread_join(this->managerID,NULL);
    //唤醒阻塞的工作线程
    for(int i=0;i<this->liveNum;i++){
        pthread_cond_signal(&this->notEmpty);
    }
    //释放堆内存
    if(this->TaskQ) delete[] this->TaskQ;
    if(this->threadIDs) free(this->threadIDs);

    //销毁互斥量，条件变量
    pthread_mutex_destroy(&this->mutexPool);
    pthread_mutex_destroy(&this->mutexBusy);
    pthread_cond_destroy(&this->notEmpty);
    pthread_cond_destroy(&this->notfull);
   
    return 0;
}

/**
 * threadPoolAdd:给线程池添加任务
 * ThreaPool *pool：线程池
 * void(*func)(void*):要加到任务中的函数指针
 * void* arg: 要加到任务中的函数指针的参数
 * */
void Threadpool::threadPoolAdd(void(*func)(void*),void* arg){
    pthread_mutex_lock(&this->mutexPool);
    while(this->queueSize == this->queueCapacity && !this->shutdown){
        //因为任务队列满了，或者线程池需要销毁，阻塞生产者线程
        pthread_cond_wait(&this->notfull,&this->mutexPool);
    }
    if(this->shutdown){
        pthread_mutex_unlock(&this->mutexPool);
        return;
    }
    //添加任务
    this->TaskQ[this->queueEnd].function = func;
    this->TaskQ[this->queueEnd].arg = arg;
    this->queueEnd = (this->queueEnd+1)%this->queueCapacity;
    this->queueSize++;

    pthread_cond_signal(&this->notEmpty);
    pthread_mutex_unlock(&this->mutexPool);
}

/**
 * threadPoolBusyNum:获取线程中的线程的个数
 * ThreadPool* pool: 线程池
 * 返回值：当前线程的个数
 * */
int Threadpool::threadPoolBusyNums(){
    pthread_mutex_lock(&this->mutexBusy);
    int busyNum = this->busyNum;
    pthread_mutex_unlock(&this->mutexBusy);
    return busyNum;
}

/**
 * threadPoolAliveNum:获取线程池中活着的线程的个数
 * ThreadPool* pool:线程池
 * */
int Threadpool::threadPoolLiveNums(){
    pthread_mutex_lock(&this->mutexPool);
    int aliveNum = this->liveNum;
    pthread_mutex_unlock(&this->mutexPool);
    return aliveNum;
}

//工作的线程
void* Threadpool::worker(void* arg){
    Threadpool *pool = (Threadpool*) arg;
    while(1){
        pthread_mutex_lock(&pool->mutexPool);
        while(pool->queueSize==0 && !pool->shutdown)
        {
            //阻塞工作线程
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);
            //判断是否有销毁线程
            if(pool->exitNum>0){
                pool->exitNum--;
                if(pool->liveNum>pool->minNum){
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    pool->threadExit();
                }
            }
        }
        //判断线程是否被关闭
        if(pool->shutdown){
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        }
        //从任务队列中取出一个任务
        Task* task=new Task();
        task->function = pool->TaskQ[pool->queueFront].function;
        task->arg = pool->TaskQ[pool->queueFront].arg;
        //移动头节点
        pool->queueFront =(pool->queueFront+1)%pool->queueCapacity;
        pool->queueSize--;
        //解锁
        pthread_cond_signal(&pool->notfull);
        pthread_mutex_unlock(&pool->mutexPool);
        LOG(INFO)<<"thread "<<pthread_self()<<"end working ...\n"<<std::endl;

        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        //执行
        task->function(task->arg);
        free(task->arg);
        task->arg=NULL;

        LOG(INFO)<<"thread "<<pthread_self()<< "end working ...\n"<<std::endl;
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

//管理者任务线程
void* Threadpool::manager(void* arg){
    Threadpool *pool = (Threadpool*) arg;
    while(!pool->shutdown){

        //3秒检查一次
        sleep(3);
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexPool);
        LOG(INFO)<<"LiveNum="<<liveNum<<","<<"BusyNum="<<busyNum<<std::endl;

        //添加线程
        //任务的个数>成活的线程个数&&存活的线程数<最大线程数
        if(queueSize>liveNum && liveNum<pool->maxNum){
            pthread_mutex_unlock(&pool->mutexPool);
            for(int i=0;i<pool->maxNum&&pool->liveNum<pool->maxNum;i++){
                if(pool->threadIDs[i]==0)
                {
                    pthread_create(&pool->threadIDs[i],NULL,worker,pool);
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }
        //销毁线程
        //条件是:忙的线程*2<存活的线程 &&存活的线程>最小线程数
        if(busyNum*2<liveNum&&liveNum>pool->minNum){
            pthread_mutex_lock(&pool->mutexPool);
            liveNum = pool->liveNum;
            busyNum = pool->busyNum;
            int NUMBER = liveNum - busyNum;
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            //发信号让线程自杀
            for(int i=0;i<NUMBER;i++){
                pthread_cond_signal(&pool->notEmpty);
            }
        }


    }
    return NULL;
}

void Threadpool::threadExit(){
    pthread_t tid = pthread_self();
    for(int i=0;i<this->maxNum;i++)
    {
        if(this->threadIDs[i]==tid){
            this->threadIDs[i]=0;
            LOG(INFO)<<tid<<"exiting...\n"<<std::endl;
            break;
        }
    }
    pthread_exit(NULL);
    return;
}
