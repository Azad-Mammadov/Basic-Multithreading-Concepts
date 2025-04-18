#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

// Declaring global mutex and condition variables
std::mutex cout_mutex;
std::condition_variable cv1, cv2;
bool thread1_done = false; // Flag to indicate when Thread 1 is done
bool thread2_done = false; // Flag to indicate when Thread 2 is done

void timer1() {
    for (int i = 0; i < 10; ++i) {
        {
            // Lock the mutex to safely access std::cout 
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Thread 1: " << i + 1 << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Sleep for 500ms 
    }

    // Notify Thread 2 that Thread 1 is done
    {
        // Lock the mutex to safely update the shared flag `thread1_done`
        std::lock_guard<std::mutex> lock(cout_mutex);
        thread1_done = true;
    }
    cv1.notify_one(); // Notify waiting thread
}

void timer2() {
    // Wait for Thread 1 to finish
    {
        // Lock the mutex while waiting for the condition variable
        std::unique_lock<std::mutex> lock(cout_mutex);
        cv1.wait(lock, [] { return thread1_done; }); // Wait until thread1_done is true
    }

    // Thread 2 starts after Thread 1 is done
    for (int i = 0; i < 10; ++i) {
        {
            // Lock the mutex to safely access std::cout
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Thread 2: " << i + 1 << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Sleep for 500ms
    }

    // Notify Thread 3 that Thread 2 is done
    {
        // Lock the mutex to safely update the shared flag `thread2_done`
        std::lock_guard<std::mutex> lock(cout_mutex);
        thread2_done = true;
    }
    cv2.notify_one(); // Notify waiting thread
}

void timer3() {
    // Wait for Thread 2 to finish
    {
        // Lock the mutex while waiting for the condition variable
        std::unique_lock<std::mutex> lock(cout_mutex);
        cv2.wait(lock, [] { return thread2_done; }); // Wait until thread2_done is true
    }

    // Thread 3 starts after Thread 2 is done
    for (int i = 0; i < 10; ++i) {
        {
            // Lock the mutex to safely access std::cout
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Thread 3: " << i + 1 << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Sleep for 500ms
    }
}

int main() {
    std::cout << "Starting threads..." << std::endl;

    std::thread t1(timer1); // Thread 1
    std::thread t2(timer2); // Thread 2
    std::thread t3(timer3); // Thread 3

    t1.join(); // Wait for Thread 1 to finish
    t2.join(); // Wait for Thread 2 to finish
    t3.join(); // Wait for Thread 3 to finish

    std::cout << "All threads finished!" << std::endl;
    return 0;
}
