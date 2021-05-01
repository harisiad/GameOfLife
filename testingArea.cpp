#include <iostream>
#include <thread>
#include <mutex>

class dummy
{
public:
    int pos;
    std::string* data;
    std::string name;
    std::thread thr;
    std::mutex glMutex;

    dummy(std::string name, std::string* str, int pos)
    : data(str), pos(pos), name(name)
    {
        thr = std::thread(&dummy::job,this);
    }

    ~dummy()
    {
        thr.join();
    }

    void job()
    {
        int count = 0;
        while(count < 100000000)
        {
            {
                std::lock_guard<std::mutex> lg(glMutex);

                (*data)[pos] = char((*data)[pos] + 1);
                count++;
            }
            std::cout << std::endl;
            std::cout << name << ": " << *data << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

int main()
{
    std::string* str = new std::string("WWWWW.X.FFFFFFFFFFF.................MMMMMMMMMMMMM");
    dummy dummy1("dummy1", str, 6);
    dummy dummy2("dummy2", str, 5);
    dummy dummy3("dummy3", str, 7);
    dummy dummy4("dummy4", str, 8);
    dummy dummy5("dummy5", str, 9);
    dummy dummy6("dummy6", str, 10);
    dummy dummy7("dummy7", str, 11);
}