#pragma once

namespace Owl {
    /**
     * @brief Singleton pattern template
     *
     * @tparam T Singleton pattern typename
     *
     * Template usage
     * @code
     * class Example : public Singleton<Example>{
     *     friend Singleton<Example>;
     *
     * private:
     *     Example() = default;
     * };
     * @endcode
     */
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