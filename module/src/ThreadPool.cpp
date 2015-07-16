#include "ThreadPool.h"
#include "log.h"

//初始化线程池
void pool_init (int max_thread_num)  
{  
     pool = (CThread_pool *) malloc (sizeof (CThread_pool));  
  
     pthread_mutex_init (&(pool->queue_lock), NULL);  
     pthread_cond_init (&(pool->queue_ready), NULL);//？  
  
     pool->queue_head = NULL;  
  
     pool->max_thread_num = max_thread_num;  
     pool->cur_queue_size = 0;  
  
     pool->shutdown = 0;  
  
     pool->threadid =(pthread_t *) malloc (max_thread_num * sizeof (pthread_t));  
     int i = 0;  
     for (i = 0; i < max_thread_num; i++)  
     {  
         pthread_create (&(pool->threadid[i]), NULL, thread_routine,NULL);  
     }  
}  

//向线程池中加入任务 
int pool_add_worker(void * (*process) (const void *arg), void *arg)  
{
	if(pool->cur_queue_size==pool->max_thread_num){
		printfs(2,"线程池满！\n");
		return 0;
	}
    //构造一个新任务
     CThread_worker *newworker =(CThread_worker *) malloc (sizeof (CThread_worker));  
     newworker->process = process;  
     newworker->arg = arg;  
     newworker->next = NULL;//别忘置空  
     pthread_mutex_lock (&(pool->queue_lock));  
    //将任务加入到等待队列中
     CThread_worker *member = pool->queue_head;  
    if (member != NULL)  
     {  
        while (member->next != NULL)  
             member = member->next;  
         member->next = newworker;  
     }else{  
         pool->queue_head = newworker;  
     }  
  
     assert(pool->queue_head != NULL);  
  
     pool->cur_queue_size++;  
     pthread_mutex_unlock (&(pool->queue_lock));  
    //好了，等待队列中有任务了，唤醒一个等待线程； 
     //注意如果所有线程都在忙碌，这句没有任何作用
     pthread_cond_signal (&(pool->queue_ready));  

    return 1;  
}

//销毁线程池，等待队列中的任务不会再被执行，但是正在运行的线程会一直 
//把任务运行完后再退出
int pool_destroy()
{  
    if (pool->shutdown)  
        return -1;/*防止两次调用*/  
     pool->shutdown = 1;  
  
  
    /*唤醒所有等待线程，线程池要销毁了*/  
     pthread_cond_broadcast (&(pool->queue_ready));  
  
  
    /*阻塞等待线程退出，否则就成僵尸了*/  
    int i;  
    for (i = 0; i < pool->max_thread_num; i++)  
         pthread_join (pool->threadid[i], NULL);  
     free (pool->threadid);  
  
  
    /*销毁等待队列*/  
     CThread_worker *head = NULL;  
     while (pool->queue_head != NULL)  
     {  
         head = pool->queue_head;  
         pool->queue_head = pool->queue_head->next;  
         free (head);  
     }  
    /*条件变量和互斥量也别忘了销毁*/  
     pthread_mutex_destroy(&(pool->queue_lock));  
     pthread_cond_destroy(&(pool->queue_ready));  
       
     free (pool);  
    /*销毁后指针置空是个好习惯*/  
     pool=NULL;  
    return 0;  
}

void *thread_routine(void *arg)
{  
     printfs(2,"starting thread 0x%x\n", pthread_self());
     while (1)  
     {  
         pthread_mutex_lock (&(pool->queue_lock));  
        /*如果等待队列为0并且不销毁线程池，则处于阻塞状态; 注意pthread_cond_wait是一个原子操作，等待前会解锁，唤醒后会加锁*/  
        while (pool->cur_queue_size == 0 && !pool->shutdown)  
         {  
             printfs(2,"thread 0x%x is waiting\n", pthread_self ());  
             pthread_cond_wait (&(pool->queue_ready), &(pool->queue_lock));  
         }  
  
  
        /*线程池要销毁了*/  
        if (pool->shutdown)  
         {  
            /*遇到break,continue,return等跳转语句，千万不要忘记先解锁*/  
             pthread_mutex_unlock (&(pool->queue_lock));  
             printfs(2,"thread 0x%x will exit\n", pthread_self ());  
             pthread_exit (NULL);  
         }  
  
  
         printfs(2,"thread 0x%x is starting to work\n", pthread_self ());  
  
        /*assert是调试的好帮手*/  
         assert(pool->cur_queue_size != 0);  
         assert(pool->queue_head != NULL);  
           
        /*等待队列长度减去1，并取出链表中的头元素*/  
         pool->cur_queue_size--;  
         CThread_worker *worker = pool->queue_head;  
         pool->queue_head = worker->next;  
         pthread_mutex_unlock (&(pool->queue_lock));  
  
  
        /*调用回调函数，执行任务*/  
         (*(worker->process)) (worker->arg);  
         free (worker);  
         worker = NULL;  
     }  
    /*这一句应该是不可达的*/  
     pthread_exit(NULL);  
}

/*
//下面是测试代码  
void *myprocess(const void *argc)
{
	int i = *(int *)argc;
	printfs(2,"threadid is 0x%x, working on task %d\n", pthread_self(),i);
	usleep(1000);//休息一秒，延长任务的执行时间
	return NULL;  
}

int funcA(int a,int b)  
{  
    return a + b;  
} 

int main (int argc, char **argv)  
{
	int (*func)(int,int);
	func = funcA;  
	//两种赋值给函数指针的方法都可以
	int i = func(12,10);
	printfs(2,"%d\n",i);
	void * (*f1)(const void *argv);
	f1 = myprocess;
	printfs(2,"0x%04x\n",f1(&i));

	pool_init(25);//线程池中最多三个活动线程  

	int i=0;
	while(1){
		i += pool_add_worker(myprocess, &i);
		usleep(500);
		if(i>100000000) break;
	}

	//等待所有任务完成
	//sleep (5);  
	//销毁线程池
	pool_destroy();  
	//free (workingnum);
	
	return 0;
}
*/
