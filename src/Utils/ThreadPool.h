#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <iostream> // for std::cout, etc.

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    // Enqueue a task and get a future for its result
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type>;

private:
    // Worker threads
    std::vector<std::thread> workers;

    // Task queue
    std::queue<std::function<void()>> tasks;

    // Synchronization
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
};

// ==============================
// Constructor
// ==============================
inline ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    //std::cout << "[ThreadPool] Constructor -> Creating " 
    //          << numThreads << " worker threads.\n";

    for(size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this, i] {
            //std::cout << "[ThreadPool] Worker #" << i << " started.\n";
            while(true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    // Wait for either 'stop' or 'tasks' to become non-empty.
                    this->condition.wait(lock, [this]{ 
                        return this->stop.load() || !this->tasks.empty(); 
                    });

                    if(this->stop.load() && this->tasks.empty()) {
                    //    std::cout << "[ThreadPool] Worker thread exiting because stop=true and tasks.empty().\n";
                        return; 
                    }

                    // Pop the next task
                    task = std::move(this->tasks.front());
                    this->tasks.pop();

                    //std::cout << "[ThreadPool] Worker thread got a task from the queue. Remaining tasks="
                    //          << this->tasks.size() << "\n";
                }

                // Run the task
                //std::cout << "[ThreadPool] Worker thread executing a task...\n";
                task();
                //std::cout << "[ThreadPool] Worker thread finished a task.\n";
            }
        });
    }
}

// ==============================
// Destructor
// ==============================
inline ThreadPool::~ThreadPool() {
    //std::cout << "[ThreadPool] Destructor -> Stopping all threads.\n";

    // Signal to stop
    stop.store(true);
    condition.notify_all();

    // Join all threads
    for(size_t i = 0; i < workers.size(); ++i) {
        if (workers[i].joinable()) {
            //std::cout << "[ThreadPool] Joining worker #" << i << "\n";
            workers[i].join();
        }
    }
    //std::cout << "[ThreadPool] All threads joined. Destructor complete.\n";
}

// ==============================
// Enqueue method
// ==============================
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::invoke_result<F, Args...>::type>
{
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<return_type()> >(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                );
        
    std::future<return_type> res = task->get_future();
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        // Don't allow enqueueing after stopping the pool
        if(stop.load()) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks.emplace([task](){ (*task)(); });
        //std::cout << "[ThreadPool] Enqueue -> tasks.size() now " 
        //          << tasks.size() << "\n";
    }
    // Notify one waiting thread that there's a new task
    condition.notify_one();
    return res;
}

#endif // THREADPOOL_H
