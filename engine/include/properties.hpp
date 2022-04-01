#pragma once

#include <functional>
// Define found on internet : https://stackoverflow.com/questions/58040401/is-there-an-alternative-for-visual-c-declspec-property-declaration-attribut

template<typename T>
class Property
{
public:
    Property() = default;

    operator const T&() const
    {
        // Call override getter if defined
        if (getter) return getter();
        return get();
    }

    void operator = (const T& value)
    {
        // Call override setter if defined
        if (setter) setter(value);
        set(value);
    }

    bool operator == (const T& value) const
    {
        return static_cast<const T&>(*this) == value;
    }

    const T* operator -> () const
    {
        return &t;
    }

    const T& get() const
    {
        return t;
    }

    void set(const T& value)
    {
        t = value;
    }

    std::function<const T&()> getter;
    std::function<void(const T&)> setter;

private:
    T t;
};
