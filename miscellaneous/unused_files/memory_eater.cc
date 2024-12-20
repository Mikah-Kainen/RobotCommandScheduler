#pragma once
#include <iostream>
#include <vector>

//PLEASE NOTE, DO NOT USE THIS!!
//If think you have a good use case, contact the project maintainers to talk about it first
class MemoryEater
{
private:
    class TypeBase
    {
    public:
        virtual void MakePolymorphic()
        {
        }
    };

    template <typename T>
    class Type : public TypeBase
    {
    public:
        bool IsDead;
        T Value;

        Type(T& value)
            : IsDead{ false }, Value{ value } {}

        ~Type()
        {
            IsDead = true;
        }

        const T& GetRef()
        {
            if (IsDead)
            {
                std::cout << "Uh Oh this thing died" << std::endl;
            }
            return Value;
        }
    };

    std::vector<TypeBase*> objects; //maybe replace with std::vector<void*>

    MemoryEater()
    {
        objects = std::vector<TypeBase*>();
    }

public:

    static MemoryEater& GetInstance()
    {
        static MemoryEater eater = MemoryEater();
        return eater;
    }

    template <typename T>
    const T& CreateRef(T value)
    {
        objects.emplace_back(new Type<T>(value));
        return dynamic_cast<Type<T>*>(objects.back())->GetRef();
    }
};