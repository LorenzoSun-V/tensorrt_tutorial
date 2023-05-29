#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include <memory>
#include <future>
#include <stdio.h>

using namespace std;

// 共享资源访问问题
// queue，stl对象不是 thread-safe
// 如果生产频率高于消费频率，则队列出现堆积现象
struct Job{
    shared_ptr<promise<string>> pro;  // 在深度学习中，应该是一个bbox或者其他输出的结果
    string input;
};
queue<Job> qjobs_;
mutex lock_;
condition_variable cv_;
const int limit_ = 5;

void video_capture(){
    
    int pic_id = 0;
    while(true){
        Job job;
        {
            unique_lock<mutex> l(lock_);
            char name[100];
            sprintf(name, "pic_%d.jpg", pic_id++);
            printf("生产了一个新图片：%s, qjobs_.size = %d\n", name, qjobs_.size());

            // 如果队列满了，我不生产，我去等待队列有空间再生产
            // 通知的问题，如何通知到wait，让他可以即时退出
            // wait的流程是：
            // 一旦进入wait，则解锁
            // 一旦退出wait，又加锁
            cv_.wait(l, [&](){
                // return false，表示继续等待
                // return true，表示不等待，跳出wait
                return qjobs_.size() < limit_;
            });

            job.pro.reset(new promise<string>());
            job.input = name;
            qjobs_.push(job);

            // 拿到推理结果，跟推理之前的图像一起进行画框，然后走下面流程
        }
        // 等待这个job处理完毕，拿结果
        // .get过后，实现等待，直到promise->set_value被执行了，这里的返回值就是result
        auto result = job.pro->get_future().get();
        printf("JOB %s -> %s\n", job.input.c_str(), result.c_str());

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void infer_worker(){

    while (true){
        
        if (!qjobs_.empty()){
            {
                lock_guard<mutex> l(lock_);
                auto pjob = qjobs_.front();
                qjobs_.pop();

                // 消费掉一个，就可以通知wait，去跳出等待
                cv_.notify_one();
                printf("消费掉一个图片：%s\n", pjob.input.c_str());

                auto result = pjob.input + "---infer";
                // new_pic 送回到生产者，怎么办
                pjob.pro->set_value(result);
            }
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
        this_thread::yield();
    }
}

int main(){
    
    thread t0(video_capture);
    thread t1(infer_worker);
    
    t0.join();
    t1.join();

    return 0;
}