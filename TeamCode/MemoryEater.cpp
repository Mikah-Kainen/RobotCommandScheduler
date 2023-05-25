#include <iostream>
#include <vector>

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

        T& GetRef()
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
    T& CreateRef(T value)
    {
        objects.emplace_back(new Type<T>(value));
        return dynamic_cast<Type<T>*>(objects.back())->GetRef();
    }
};