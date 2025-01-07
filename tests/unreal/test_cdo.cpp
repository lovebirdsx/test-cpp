#include <doctest.h>
#include <memory>
#include <map>
#include <mutex>
#include <typeindex>
#include <unordered_map>

namespace
{
    class CDOBase
    {
    public:
        virtual ~CDOBase() = default;

        virtual std::unique_ptr<CDOBase> clone() const = 0;
    };

    class CDOManager
    {
    public:
        static CDOManager &getInstance()
        {
            static CDOManager instance;
            return instance;
        }

        void registerCDO(const std::type_index &name, std::unique_ptr<CDOBase> cdo)
        {
            _cdos[name] = std::move(cdo);
        }

        const CDOBase *getCDO(const std::type_index &name) const
        {
            std::lock_guard<std::mutex> lock(_mutex);
            auto it = _cdos.find(name);
            if (it != _cdos.end())
            {
                return it->second.get();
            }
            return nullptr;
        }

    private:
        CDOManager() = default;
        ~CDOManager() = default;
        CDOManager(const CDOManager &) = delete;
        CDOManager &operator=(const CDOManager &) = delete;

        mutable std::mutex _mutex;
        std::unordered_map<std::type_index, std::unique_ptr<CDOBase>> _cdos;
    };

// 宏定义，用于在类中声明和定义CDO相关的静态方法
#define DECLARE_CDO(ClassType)        \
public:                               \
    static const ClassType &getCDO(); \
private:                              \
    static bool bCDOInitialized;      \
    static std::unique_ptr<ClassType> CDOInstance;

// 宏定义，用于在类的实现文件中定义CDO
#define DEFINE_CDO(ClassType)                                                                             \
    std::unique_ptr<ClassType> ClassType::CDOInstance = nullptr;                                          \
    bool ClassType::bCDOInitialized = []() -> bool {                                                      \
        ClassType *defaultObj = new ClassType(true);                                                      \
        CDOManager::getInstance().registerCDO(typeid(ClassType), std::unique_ptr<ClassType>(defaultObj)); \
        return true;                                                                                      \
    }();                                                                                                  \
    const ClassType &ClassType::getCDO()                                                                  \
    {                                                                                                     \
        const CDOBase *baseCDO = CDOManager::getInstance().getCDO(typeid(ClassType));                     \
        return *static_cast<const ClassType *>(baseCDO);                                                  \
    }

    class MyActor: public CDOBase
    {
    public:
        MyActor(bool bCDO = false) : health(100), name("MyActor")
        {
            if (bCDO)
            {
                
            }
        }

        std::unique_ptr<CDOBase> clone() const override
        {
            return std::make_unique<MyActor>(*this);
        }

        int health;
        std::string name;

        DECLARE_CDO(MyActor)
    };

    DEFINE_CDO(MyActor)

    class ActorFactory
    {
    public:
        template <typename T, typename... TArgs>
        static std::unique_ptr<T> Create(TArgs &&...args)
        {
            const CDOBase *baseCDO = CDOManager::getInstance().getCDO(typeid(T));
            if (!baseCDO)
            {
                return nullptr;
            }

            std::unique_ptr<CDOBase> newObjBase = baseCDO->clone();
            std::unique_ptr<T> newObj = std::unique_ptr<T>(static_cast<T*>(newObjBase.release()));

            return newObj;
        }
    };
}

TEST_CASE("cdo")
{
    const MyActor& cdo = MyActor::getCDO();
    CHECK_EQ(cdo.health, 100);

    std::unique_ptr<MyActor> actor = ActorFactory::Create<MyActor>();
    CHECK_EQ(actor->health, 100);

    MyActor& mutableCdo = const_cast<MyActor&>(cdo);
    mutableCdo.health = 200;

    std::unique_ptr<MyActor> actor2 = ActorFactory::Create<MyActor>();
    CHECK_EQ(actor2->health, 200);
}
