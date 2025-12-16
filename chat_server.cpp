#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <chrono>

//sdilena promenna - vektor na ukladani chatujicich uzivatelu
std::vector<int> chat_users;
std::mutex chat_mutex;

void worker(int id) {
    // Worker joins, locks shared vector, appends its id to a shared vector
    {
        std::lock_guard<std::mutex> lock(chat_mutex);
        chat_users.push_back(id);
    }
    printf("User %d joined the chat.\n", id);

    //sleep for random time to simulate user activity
    std::this_thread::sleep_for(std::chrono::seconds(1 + rand() % 10));

    // Worker leaves, locks shared vector, removes its id from the shared vector
    {
        std::lock_guard<std::mutex> lock(chat_mutex);
        chat_users.erase(std::remove(chat_users.begin(), chat_users.end(), id), chat_users.end());
    }
    printf("User %d left the chat.\n", id);
}

int main() {
    int user_id = 0;

    //infinite loop to simulate server running
    while (true) {
        //sleep 5s to simulate waiting for new user connections
        std::this_thread::sleep_for(std::chrono::seconds(5));

        //add new user to the chat:
        std::thread(worker, user_id++).detach();
    }

}