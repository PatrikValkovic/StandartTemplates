#ifndef TEMPLATES_UNIQUEPOINTER_H
#define TEMPLATES_UNIQUEPOINTER_H

#include "Basis.h"

namespace Templates
{
    template<typename _T, typename DELETER = __defaultDeleter<_T>>
    class UniquePointer
    {
        using T = typename Meta::remove_array<_T>::type;
    private:
        T* _raw;
    public:
        UniquePointer() noexcept : _raw(nullptr)
        {}

        explicit UniquePointer(T* ptr) noexcept: _raw(ptr)
        {}

        UniquePointer(const UniquePointer&) = delete;

        UniquePointer(UniquePointer&& second) noexcept : UniquePointer()
        {
            using Templates::swap;
            swap(this->_raw, second._raw);
        }

        ~UniquePointer()
        {
            this->Release();
        }

        UniquePointer& operator=(const UniquePointer&) = delete;

        UniquePointer& operator=(UniquePointer&& second) noexcept
        {
            using Templates::swap;
            if (this == &second)
                return *this;
            this->Release();
            swap(this->_raw, second._raw);
            return *this;
        }

        T& operator*()
        {
            return *this->Get();
        }

        const T& operator*() const
        {
            return *this->Get();
        }

        T& operator[](int i)
        {
            return this->Get()[i];
        }

        const T& operator[](int i) const
        {
            return this->Get()[i];
        }


        T* operator->() const noexcept
        {
            return Get();
        }

        explicit operator bool() const noexcept
        {
            return this->_raw != nullptr;
        }

        void Release()
        {
            DELETER::perform(this->_raw);
            this->_raw = nullptr;
        }

        T* Get() const noexcept
        {
            return this->_raw;
        }

        T* Raw() const noexcept
        {
            return this->Get();
        }

        void Swap(UniquePointer& second) noexcept
        {
            using Templates::swap;
            swap(this->_raw, second._raw);
        }
    };

    template<typename T, typename ...ARGS>
    UniquePointer<T> make_unique(ARGS... args)
    {
        return UniquePointer<T>(new T(args...));
    }

    template<typename T>
    void swap(UniquePointer<T>& first,UniquePointer<T>& second)
    {
        first.Swap(second);
    }

}


#endif //TEMPLATES_SMARTPOINTERS_H
