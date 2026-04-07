#include <atomic>
#include <vector>
#include <thread>
#include <cstdio>
#include <optional>

template <typename T>
class LockFreeStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T const& data_) : data(data_), next(nullptr) {}
    };
    std::atomic<Node*> head {nullptr};
public:
    void push(const T& data){
        Node* new_node = new Node(data);
        new_node->next = head.load(std::memory_order_relaxed);
        // CAS循环，尝试将head从old_head指向new_node
        while(!head.compare_exchange_weak(new_node->next, new_node,
            std::memory_order_release, std::memory_order_relaxed));
    }

    bool pop(T& data){
        Node* old_head = head.load(std::memory_order_relaxed);
        while(nullptr != old_head){
            // CAS循环，尝试将head从old_head指向old_head->next
            if(head.compare_exchange_weak(old_head, old_head->next,
                std::memory_order_acquire, std::memory_order_relaxed)){
                
                data = old_head->data;
                delete old_head;
                return true;
            }
        }
        return false;
    }

};

template <typename T>
class LockFreeQueue {
    struct Node {
        T data;
        std::atomic<Node*> next;
        Node(T const& data_) : data(data_), next(nullptr) {}
    };

    std::atomic<Node*> head {nullptr};
    std::atomic<Node*> tail {nullptr};
public:

    LockFreeQueue(){
        Node* dummy = new Node(T());
        head.store(dummy, std::memory_order_relaxed);
        tail.store(dummy, std::memory_order_relaxed);
    }

    ~LockFreeQueue(){
        while (Node* node = head.load(std::memory_order_relaxed)){
            head.store(node->next.load(std::memory_order_relaxed), std::memory_order_relaxed);
            delete node;
        }
    }
    void enqueue(const T& data){ 
        Node* new_node = new Node(data);
        Node* old_tail = tail.load(std::memory_order_relaxed);
        while (true){
            Node* next = old_tail->next.load(std::memory_order_relaxed);
            if(nullptr == next){
                // 表明old_tail是最尾节点
                // 尝试将old_tail的next指向new_node
                if(old_tail->next.compare_exchange_weak(next, new_node,
                                                        std::memory_order_release, 
                                                        std::memory_order_relaxed)){
                    break;
                }
            } else{
                // 表明其他线程插入了新节点
                // 尝试将tail指向新的节点next
                tail.compare_exchange_weak(old_tail, next,
                                            std::memory_order_release, 
                                            std::memory_order_relaxed);
            }
            // 更新old_tail
            old_tail = tail.load(std::memory_order_relaxed);
        }
    }

    bool dequeue(T& data){ 
        Node* old_head = head.load(std::memory_order_relaxed);
        while (true){
            // 使用memory_order_acquire，保证读取next时，old_head已经可见
             Node* next = old_head->next.load(std::memory_order_acquire);
            if(nullptr == next){
                // 队列空
                return false;
            }
            // 尝试将head指向next
            if(head.compare_exchange_weak(old_head, next,
                                          std::memory_order_release, 
                                          std::memory_order_relaxed)){
                data = next->data;
                delete old_head;
                return true;
            }
        }
    }
};

// template <typename T>
// class LockFreeQueue {
// private:
//     struct Node {
//         T data;
//         std::atomic<Node*> next;
        
//         template<typename... Args>
//         explicit Node(Args&&... args) 
//             : data(std::forward<Args>(args)...), next(nullptr) {}
//     };

//     std::atomic<Node*> head_{nullptr};
//     std::atomic<Node*> tail_{nullptr};
//     std::atomic<std::size_t> size_{0};

// public:
//     LockFreeQueue() {
//         Node* dummy = new Node();
//         head_.store(dummy, std::memory_order_relaxed);
//         tail_.store(dummy, std::memory_order_relaxed);
//     }

//     ~LockFreeQueue() {
//         Node* node = head_.load(std::memory_order_relaxed);
//         while (node != nullptr) {
//             Node* next = node->next.load(std::memory_order_relaxed);
//             delete node;
//             node = next;
//         }
//     }

//     LockFreeQueue(const LockFreeQueue&) = delete;
//     LockFreeQueue& operator=(const LockFreeQueue&) = delete;
//     LockFreeQueue(LockFreeQueue&&) = delete;
//     LockFreeQueue& operator=(LockFreeQueue&&) = delete;

//     template<typename U>
//     bool enqueue(U&& value) {
//         Node* new_node = nullptr;
//         try {
//             new_node = new Node(std::forward<U>(value));
//         } catch (...) {
//             return false;
//         }

//         Node* old_tail = tail_.load(std::memory_order_relaxed);
//         while (true) {
//             Node* next = old_tail->next.load(std::memory_order_acquire);
            
//             if (old_tail != tail_.load(std::memory_order_relaxed)) {
//                 old_tail = tail_.load(std::memory_order_relaxed);
//                 continue;
//             }
            
//             if (next == nullptr) {
//                 if (old_tail->next.compare_exchange_weak(
//                     next, new_node, 
//                     std::memory_order_release,
//                     std::memory_order_relaxed
//                 )) {
//                     tail_.compare_exchange_strong(
//                         old_tail, new_node,
//                         std::memory_order_release,
//                         std::memory_order_relaxed
//                     );
//                     ++size_;
//                     return true;
//                 }
//             } else {
//                 tail_.compare_exchange_strong(
//                     old_tail, next,
//                     std::memory_order_relaxed,
//                     std::memory_order_relaxed
//                 );
//                 old_tail = next;
//             }
//         }
//     }

//     std::optional<T> dequeue() {
//         Node* old_head = head_.load(std::memory_order_relaxed);
        
//         while (true) {
//             Node* next = old_head->next.load(std::memory_order_acquire);
            
//             if (old_head != head_.load(std::memory_order_relaxed)) {
//                 old_head = head_.load(std::memory_order_relaxed);
//                 continue;
//             }
            
//             if (next == nullptr) {
//                 return std::nullopt;
//             }
            
//             if (head_.compare_exchange_weak(
//                 old_head, next,
//                 std::memory_order_release,
//                 std::memory_order_relaxed
//             )) {
//                 T value = std::move(next->data);
//                 delete old_head; // 释放哑元节点
//                 --size_;
//                 return value;
//             }
//         }
//     }

//     [[nodiscard]] bool empty() const noexcept {
//         return head_.load(std::memory_order_relaxed) == 
//                tail_.load(std::memory_order_relaxed);
//     }

//     [[nodiscard]] std::size_t size() const noexcept {
//         return size_.load(std::memory_order_relaxed);
//     }
// };

int main(int argc, char const *argv[])
{
    LockFreeQueue<int> que_;
    std::vector<std::thread> threads;
    std::vector<std::thread> threads2;

    for(int i = 0; i < 10000; i++){
        threads.push_back(std::thread([&que_, i](){
            for(int j = 0; j < 10000; j++){
                que_.enqueue(j);
                printf("thread: %d, enqueue: %d\n", i, j);
            }
        }));
    }

    for(int i = 0; i < 10000; i++){
        threads2.push_back(std::thread([&que_, i](){
            for(int j = 0; j < 10000; j++){
                int ret = 0;
                if(que_.dequeue(ret))
                    printf("thread: %d, dequeue %d\n", i, ret);
            }
        }));

    }

    for(int i = 0; i < 10000; i++){
        if(threads.at(i).joinable()){
            threads.at(i).join();
        }
        if(threads2.at(i).joinable()){
            threads2.at(i).join();
        }
    }
    return 0;
}
