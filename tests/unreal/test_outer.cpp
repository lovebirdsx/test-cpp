#include <doctest.h>
#include <string>
#include <vector>

namespace
{
    class UObject
    {
    public:
        UObject(const std::string &name, UObject *outer = nullptr) : name(name), outer(outer)
        {
            if (outer)
            {
                outer->addInner(this);
            }
        }

        void addInner(UObject *inner)
        {
            inners.push_back(inner);
        }

        UObject *getOuter() const
        {
            return outer;
        }

        const std::vector<UObject *> &getInners() const
        {
            return inners;
        }

        std::string getName() const
        {
            return name;
        }

    private:
        std::string name;
        UObject *outer;
        std::vector<UObject *> inners;
    };

    class AActor : public UObject
    {
    public:
        AActor(const std::string &name, UObject *outer) : UObject(name, outer)
        {
        }
    };

    class Component : public UObject
    {
    public:
        Component(const std::string &name, UObject *outer) : UObject(name, outer)
        {
        }
    };
}

TEST_CASE("outer")
{
    UObject outer("Outer");
    {
        AActor actor("Actor", &outer);
        auto outer2 = actor.getOuter();
        CHECK(outer2->getName() == "Outer");
        auto inners2 = outer2->getInners();
        CHECK(inners2.size() == 1);
        CHECK(inners2[0]->getName() == "Actor");

        Component component("Component1", &actor);
        auto outer3 = component.getOuter();
        CHECK(outer3->getName() == "Actor");
        
        Component component2("Component2", &actor);
        CHECK_EQ(outer3->getInners().size(), 2);
    }
}