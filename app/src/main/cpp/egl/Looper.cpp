//
// Created by Lai on 2020/12/18.
//

#include "Looper.h"



#include <jni.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <logUtil.h>

struct LooperMessage;

typedef struct LooperMessage LooperMessage;



/**
 * 线程执行句柄
 * @param p
 * @return
 */
void* Looper::trampoline(void* p) {
    ((Looper*)p)->loop();
    return NULL;
}


Looper::Looper() {
    head = NULL;

    sem_init(&headdataavailable, 0, 0);
    sem_init(&headwriteprotect, 0, 1);
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&worker, &attr, trampoline, this);
    running = true;
}

Looper::~Looper() {
    if (running) {
        LOGCATI("Looper deleted while still running. Some messages will not be processed");
        quit();
    }
}

/**
 * 入队消息
 * @param what
 * @param flush
 */
void Looper::postMessage(int what, bool flush) {
    postMessage(what, 0, 0, NULL, nullptr,flush);
}


/**
 * 入队消息
 * @param what
 * @param obj
 * @param flush
 */
void Looper::postMessage(int what, void *obj, bool flush) {
    postMessage(what, 0, 0, obj, nullptr,flush);
}

void Looper::postMessage(int what, std::function<void()> funcType, bool flush) {
    postMessage(what, 0, 0, NULL, funcType,flush);
}

/**
 * 入队消息
 * @param what
 * @param arg1
 * @param arg2
 * @param flush
 */
void Looper::postMessage(int what, int arg1, int arg2, bool flush) {
    postMessage(what, arg1, arg2, NULL, nullptr,flush);
}

/**
 *
 * @param what
 * @param arg1
 * @param arg2
 * @param obj
 * @param flush
 */
void Looper::postMessage(int what, int arg1, int arg2,void *obj, std::function<void()> funcType,bool flush) {
    LooperMessage *msg = new LooperMessage();
    msg->what = what;
    msg->obj = obj;
    msg->arg1 = arg1;
    msg->arg2 = arg2;
    msg->next = NULL;
    msg->quit = false;
    msg->funcType=funcType;
    addMessage(msg, flush);
}

/**
 * 添加消息
 * @param msg
 * @param flush
 */
void Looper::addMessage(LooperMessage *msg, bool flush) {
    sem_wait(&headwriteprotect);
    LooperMessage *h = head;

    if (flush) {
        while(h) {
            LooperMessage *next = h->next;
            delete h;
            h = next;
        }
        h = NULL;
    }
    if (h) {
        while (h->next) {
            h = h->next;
        }
        h->next = msg;
    } else {
        head = msg;
    }
    LOGCATI("post msg %d", msg->what);
    sem_post(&headwriteprotect);
    sem_post(&headdataavailable);
}

/**
 * 循环体
 */
void Looper::loop() {
    while(true) {
        // wait for available message
        sem_wait(&headdataavailable);

        // get next available message
        sem_wait(&headwriteprotect);
        LooperMessage *msg = head;
        if (msg == NULL) {
            LOGCATI("no msg");
            sem_post(&headwriteprotect);
            continue;
        }
        head = msg->next;
        sem_post(&headwriteprotect);

        if (msg->quit) {
            LOGCATI("quitting");
            delete msg;
            return;
        }
        LOGCATI("processing msg %d", msg->what);
        handleMessage(msg);
        delete msg;
    }
}

/**
 * 退出Looper循环
 */
void Looper::quit() {
    LOGCATI("quit");
    LooperMessage *msg = new LooperMessage();
    msg->what = 0;
    msg->obj = NULL;
    msg->next = NULL;
    msg->quit = true;
    addMessage(msg, false);
    void *retval;
    pthread_join(worker, &retval);
    sem_destroy(&headdataavailable);
    sem_destroy(&headwriteprotect);
    running = false;
}

/**
 * 处理消息
 * @param what
 * @param data
 */
void Looper::handleMessage(LooperMessage *msg) {
    LOGCATI("dropping msg %d %p", msg->what, msg->obj);
}