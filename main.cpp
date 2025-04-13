#include <iostream>
#include <thread>
#include <chrono>

void timer (int x) {
    for (int i = 0; i < 10; ++i) {
        std::cout << i + 1 << " ";
        std::this_thread::sleep_for(std::chrono::milliseconds(x)); // Sleep for 1 second
        if (x == 1000) {
            std::cout << "Thread 1 is running." << std::endl;
        } else if (x == 1200) {
            std::cout << "Thread 2 is running." << std::endl;
        }
    }

}

int main() {

    std::cout << "You will make it!" << std::endl;
    
    std::thread t1 (timer, 1000);   // Your code here
    std::thread t2 (timer, 1200);   // Your code here

    if (t1.joinable()) {
        t1.join(); // Wait for the thread to finish
    } 
    
    if (t2.joinable()) {
        t2.detach(); // Wait for the thread to finish
    }



    return 0;
}
