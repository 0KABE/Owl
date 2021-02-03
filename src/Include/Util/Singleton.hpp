#pragma once

namespace Owl {
    template<typename T>
    class Singleton {
    public:
        static T &GetInstance() {
            static T INSTANCE;
            return INSTANCE;
        }

        Singleton(T &&) = delete;

        Singleton(const T &) = delete;

        void operator=(const T &) = delete;

    protected:
        Singleton() = default;

        virtual ~Singleton() = default;
    };
}