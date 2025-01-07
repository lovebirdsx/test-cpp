#include <doctest.h>
#include <string>
#include <memory>

namespace
{
    class Component
    {
    public:
        explicit Component(const std::string &name) : name(name) {}

        std::string getName() const
        {
            return name;
        }

        virtual ~Component() = default;

    protected:
        std::string name;
    };

    class RenderComponent : public Component
    {
    public:
        explicit RenderComponent(const std::string &name) : Component(name) {}
    };

    class ObjectInitializer
    {
    public:
        template <typename T, typename... TArgs>
        std::shared_ptr<T> createComponent(const std::string name, TArgs &&... args) const
        {
            auto component = std::make_shared<T>(name, std::forward<TArgs>(args)...);
            return component;
        }
    };

    class Actor
    {
    public:
        explicit Actor(const ObjectInitializer &initializer) : initializer(initializer) {
            baseComponent = initializer.createComponent<Component>("BaseComponent");
        }

        std::shared_ptr<Component> getBaseComponent() const
        {
            return baseComponent;
        }

    private:
        const ObjectInitializer& initializer;

    protected:
        std::shared_ptr<Component> baseComponent;
    };

    class MyActor : public Actor
    {
    public:
        explicit MyActor(const ObjectInitializer &initializer) : Actor(initializer)
        {
            renderComponent = initializer.createComponent<RenderComponent>("RenderComponent");
            baseComponent = renderComponent;
        }

    private:
        std::shared_ptr<RenderComponent> renderComponent;
    };
};

TEST_CASE("object initializer")
{
    ObjectInitializer initializer;
    {
        Actor actor(initializer);
        auto baseComponent2 = actor.getBaseComponent();
        CHECK(baseComponent2->getName() == "BaseComponent");
    }

    {
        MyActor myActor(initializer);
        auto baseComponent = myActor.getBaseComponent();
        CHECK(baseComponent->getName() == "RenderComponent");
    }
}
