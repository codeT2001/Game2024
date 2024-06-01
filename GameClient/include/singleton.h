#ifndef SINGLETON_H
#define SINGLETON_H
#include "global.h"
/**************************************************
*
*@file      singleton.h
*@brief     单例模板
*
*@author    tandingfu
*@date      2024-05-27
*history
****************************************************/
template <typename T>
class Singleton{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>&) = delete;

    static std::shared_ptr<T> _instance;
public:

    static std::shared_ptr<T> getInstance(){
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    ~Singleton()
    {
        std::cout<<"this is singleton destruct"<<std::endl;
    }

    void pirntAddress()
    {
        std::cout<<_instance.get()<<std::endl;
    }

};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;
#endif // SINGLETON_H
