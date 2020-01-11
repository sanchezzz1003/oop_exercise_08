/*
Асинхронное программирование
Вариант 4: Пятиугольник, трапеция, ромб.
Комков Александр
М8О-201Б-18
*/

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <cstdlib>

#include "factory.h"
#include "figures.h"
#include "subscriber.h"

void menu() {
    std::cout << "\nMenu\n";
    std::cout << "Add figure\n";
    std::cout << "Exit\n\n";
}


int main(int argc, char *argv[]){
    if (argc != 2) {
        std::cout << "Enter size \n";
        return 1;
    }

    size_t vector_size = std::atoi(argv[1]);
    Factory factory;

    Subscriber subscriber;
    subscriber.buffer.reserve(vector_size);
    subscriber.processes.push_back(std::make_shared<Console_process>());
    subscriber.processes.push_back(std::make_shared<File_process>());

    std::thread subscriber_thread(std::ref(subscriber));

    menu();
    std::string cmd;
    while(std::cin >> cmd) {
        std::unique_lock<std::mutex> main_lock(subscriber.mtx);
        if (cmd == "Menu") {
            menu();
            continue;
        } else if (cmd == "Exit") {
            subscriber.end = true;
            subscriber.cv.notify_all();
            break;
        } else if (cmd == "Add"){
            std::string figure_type;

            for (size_t id = 0; id < vector_size; id++) {
                std::cout << "Enter name of figure: Pentagon[Pent], Trapezoid[Trapeze], Rhombus[Rhomb]\n";
                std::cin >> figure_type;
                if (figure_type == "Pent"){
                    Vertex *vertices = new Vertex[5];
                    std::cout << "Enter the coordinates separated by a space\n";
                    for (int i = 0; i < 5; i++) {
                        std::cin >> vertices[i];
                    }
                    try {
                        subscriber.buffer.push_back(factory.FigureCreate(Pent, vertices, id));
                    } catch (std::logic_error &e) {
                        std::cout << e.what() << "\n";
                        id--;
                    }
                } else if (figure_type == "Trapeze") {
                    Vertex *vertices = new Vertex[4];
                    std::cout << "Enter the coordinates separated by a space\n";
                    for (int i = 0; i < 4; i++) {
                        std::cin >> vertices[i];
                    }
                    try {
                        subscriber.buffer.push_back(factory.FigureCreate(Trapeze, vertices, id));
                    } catch (std::logic_error &e) {
                        std::cout << e.what() << "\n";
                        id--;
                    }
                } else if (figure_type == "Rhomb") {
                    Vertex *vertices = new Vertex[4];
                    std::cout << "Enter the coordinates separated by a space\n";
                    for (int i = 0; i < 4; i++) {
                        std::cin >> vertices[i];
                    }
                    try {
                        subscriber.buffer.push_back(factory.FigureCreate(Rhomb, vertices, id));
                    } catch (std::logic_error &e){
                        std::cout << e.what() << "\n";
                        id--;
                    }
                }
            }
            if (subscriber.buffer.size() == vector_size) {
                subscriber.cv.notify_all();
                subscriber.cv.wait(main_lock, [&subscriber]() {
                    return subscriber.success == true;
                });
                subscriber.success = false;
            }
        }
    }
    subscriber_thread.join();
    return 0;
}
