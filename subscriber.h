#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <string>

struct Subscriber;

struct Subscribers_process {
    virtual void Process(std::vector<std::shared_ptr<Figure>> &buffer) = 0;
    virtual ~Subscribers_process() = default;
};

struct Console_process : Subscribers_process {
    void Process(std::vector<std::shared_ptr<Figure>> &buffer) override {
        for (const auto figure : buffer) {
            figure->Print(std::cout);
        }
    }
};

struct File_process : Subscribers_process {
    size_t name = 0;

    void Process(std::vector<std::shared_ptr<Figure>> &buffer) override {
        std::ofstream os(std::to_string(name));
        for (const auto figure : buffer) {
            figure->Print(os);
        }
        name++;
    }
};

struct Subscriber {
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::shared_ptr<Figure>> buffer;
    std::vector<std::shared_ptr<Subscribers_process>> processes;
    std::condition_variable sv;
    bool end;
    bool success;

    void operator()() { // именной функтор
        for(;;) {
            std::unique_lock<std::mutex> guard(mtx); // lock mutex
            cv.wait(guard, [&](){               // вошедший поток на этом месте ждет пока не выполнится условия
                return buffer.size() == buffer.capacity() || end;
            });
            if (end) {
                break;
            }
            for (size_t i = 0; i < processes.size(); i++) {
                processes[i]->Process(buffer);
            }
            buffer.clear();
            success = true;
            cv.notify_all();
        }
    }
};

#endif //SUBSCRIBER_H