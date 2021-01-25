//
// Created by Lai on 2020/12/18.
//

#ifndef OPENGLDEMO_LOOPER_H
#define OPENGLDEMO_LOOPER_H

#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>
#include <functional>

struct LooperMessage {
    int what;
    int arg1;
    int arg2;
    void *obj;
    LooperMessage *next;
    bool quit;
    std::function<void()> funcType;
};


class Looper {

public:
    Looper();

    Looper &operator=(const Looper &) = delete;

    Looper(Looper &) = delete;

    virtual ~Looper();

    // 发送消息
    void postMessage(int what, bool flush = false);

    void postMessage(int what, std::function<void()> funcType,bool flush = false);

    void postMessage(int what, void *obj, bool flush = false);

    void postMessage(int what, int arg1, int arg2, bool flush = false);

    void postMessage(int what, int arg1, int arg2, void *obj,std::function<void()> funcType, bool flush = false);

    // 处理消息
    virtual void handleMessage(LooperMessage *msg);

// 退出Looper循环
    void quit();

    // 添加消息
    void addMessage(LooperMessage *msg, bool flush);

    // 消息线程句柄
    static void *trampoline(void *p);

    // 循环体
    void loop(void);

private:
    LooperMessage *head;
    pthread_t worker;
    sem_t headwriteprotect;
    sem_t headdataavailable;
    bool running;

};

#endif //OPENGLDEMO_LOOPER_H
