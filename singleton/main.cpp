#include <iostream>
#include <string>
#include <chrono>
#include <thread>

class Singleton
{

protected:
    explicit Singleton(const std::string& value): value_(value)
    {}

    std::string value_;

public:

    /**
     * Одиночки не должны быть клонируемыми.
     */
    Singleton(Singleton &other) = delete;
    Singleton& operator=(const Singleton &) = delete;

    static Singleton& GetInstance(const std::string& value);

    void SomeBusinessLogic()
    {
       // ...
    }

    std::string value() const{
       return value_;
    }
};

Singleton& Singleton::GetInstance(const std::string& value)
{

   static Singleton instance(value);

   return instance;

}

void ThreadFoo(){
   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   Singleton& singleton = Singleton::GetInstance("FOO");
   std::cout << singleton.value() << "\n";
}

void ThreadBar(){
   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   Singleton& singleton = Singleton::GetInstance("BAR");
   std::cout << singleton.value() << "\n";
}


int main()
{
   std::cout <<"If you see the same value, then singleton was reused (yay!\n" <<
             "If you see different values, then 2 singletons were created (booo!!)\n\n" <<
             "RESULT:\n";
   std::thread t1(ThreadFoo);
   std::thread t2(ThreadBar);
   t1.join();
   t2.join();

   return 0;
}
