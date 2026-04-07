#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>
#include <unistd.h>
#include <string>
#include <cstdio>


class ThreadPool{
public:
    ThreadPool(size_t size);
    ~ThreadPool();

    template<typename FUN, typename... Args>
    // auto enqueue(FUN&& fun, Args&&... args) -> std::shared_ptr<decltype(fun(args...))>;
    auto enqueue(FUN&& fun, Args&&... args) -> std::future<decltype(fun(args...))>;
    // auto enqueue(FUN&& fun, Args&&... args) -> std::future<typename std::result_of<FUN(Args...)>::type>;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    bool running;
    std::mutex mtxThPool;
    std::condition_variable cvThPool;
};

ThreadPool::ThreadPool(size_t size):
    running(true)
{
    for(size_t i = 0; i < size; i++){
        workers.emplace_back(std::move([this](){
            for(;;){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtxThPool);
                    cvThPool.wait(lock, [this](){ return !running || !tasks.empty(); });
                    if(!running || tasks.empty()){
                        return;
                    }

                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtxThPool);
        running = false;
    }
    cvThPool.notify_all();
    for(auto& th: workers){
        if(th.joinable()){
            th.join();
        }
    }
}

// template <typename FUN, typename... Args>
// auto ThreadPool::enqueue(FUN &&fun, Args &&...args) -> std::shared_ptr<decltype(fun(args...))>
// {
//     using returnType = decltype(fun(args...));
//     auto retPtr = std::make_shared<returnType>();
    
//     auto task = [fun = std::forward<FUN>(fun),
//                 argsTuple = std::make_tuple(std::forward<Args>(args)...),
//                 retPtr]() mutable{
//         *retPtr = std::apply(fun, argsTuple);
//     };

//     {
//         std::unique_lock<std::mutex> lock(mtxThPool);
//         if(!running){
//             throw std::runtime_error("can not enqueue on stopped thread pool");
//         }
//         tasks.emplace(std::move(task));
//     }

//     cvThPool.notify_one();
//     return retPtr;
// }

template <typename FUN, typename... Args>
auto ThreadPool::enqueue(FUN &&fun, Args &&...args) -> std::future<decltype(fun(args...))>
{
    using returnType = decltype(fun(args...));

    auto task = std::make_shared<std::packaged_task<returnType()>>(
        std::bind(std::forward<FUN>(fun), std::forward<Args>(args)...)
    );

    std::future<returnType> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(mtxThPool);
        if(!running){
            throw std::runtime_error("can not enqueue on stopped thread pool");
        }
        tasks.emplace([task](){(*task)();});
    }

    cvThPool.notify_one();
    return res;
}


int main(int argc, char const *argv[])
{
    // ThreadPool pool(8);
    // auto ret1 = pool.enqueue([](int a, int b){
    //     sleep(1);
    //     return a + b;
    // }, 1, 2);

    // auto ret2 = pool.enqueue([](){
    //     sleep(2);
    //     return std::string("helloworld");
    // });

    // while(!ret1.unique()){
    //     usleep(200000);
    // }
    // printf("ret1: %d\n", *ret1);

    // while(!ret2.unique()){
    //     usleep(200000);
    // }
    // printf("ret2: %s\n", ret2->c_str());
    // return 0;


    ThreadPool pool(8);
    auto ret1 = pool.enqueue([](int a, int b){
        sleep(1);
        return a + b;
    }, 1, 2);

    auto ret2 = pool.enqueue([](){
        sleep(2);
        return std::string("helloworld");
    });

    ret1.wait();
    printf("ret1: %d\n", ret1.get());

    ret2.wait();
    printf("ret2: %s\n", ret2.get().c_str());
    return 0;
}
