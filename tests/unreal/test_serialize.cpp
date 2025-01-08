#include <doctest.h>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <type_traits>

namespace
{
    class Serializable
    {
    public:
        virtual ~Serializable() = default;

        virtual void serialize(std::ostream &os) const = 0;
        virtual void deserialize(std::istream &is) = 0;
    };

    template <typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>> Serialize(std::ostream &os, const T &obj)
    {
        os.write(reinterpret_cast<const char *>(&obj), sizeof(T));
    }

    template <typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>> Deserialize(std::istream &is, T &obj)
    {
        is.read(reinterpret_cast<char *>(&obj), sizeof(T));
    }

    template <typename T>
    std::enable_if_t<!std::is_trivially_copyable_v<T>> Serialize(std::ostream &os, const T &obj)
    {
        obj.serialize(os);
    }

    template <typename T>
    std::enable_if_t<!std::is_trivially_copyable_v<T>> Deserialize(std::istream &is, T &obj)
    {
        obj.deserialize(is);
    }

    void Serialize(std::ostream &os, const std::string &str)
    {
        size_t size = str.size();
        Serialize(os, size);
        os.write(str.data(), size);
    }

    void Deserialize(std::istream &is, std::string &str)
    {
        size_t size;
        Deserialize(is, size);
        str.resize(size);
        is.read(&str[0], size);
    }

    template <typename T>
    void Serialize(std::ostream &os, const std::vector<T> &vec)
    {
        size_t size = vec.size();
        Serialize(os, size);
        for (const auto &elem : vec)
        {
            Serialize(os, elem);
        }
    }

    template <typename T>
    void Deserialize(std::istream &is, std::vector<T> &vec)
    {
        size_t size;
        Deserialize(is, size);
        vec.resize(size);
        for (auto &elem : vec)
        {
            Deserialize(is, elem);
        }
    }

    class InventoryItem : public Serializable
    {
    public:
        std::string name;
        int quantity;

        InventoryItem() = default;
        InventoryItem(const std::string &name, int quantity) : name(name), quantity(quantity) {}
        InventoryItem(const InventoryItem &) = default;
        InventoryItem &operator=(const InventoryItem &) = default;
        InventoryItem(InventoryItem &&) noexcept = default;
        InventoryItem &operator=(InventoryItem &&) noexcept = default;

        bool operator==(const InventoryItem &other) const
        {
            return name == other.name && quantity == other.quantity;
        }

        void serialize(std::ostream &os) const override
        {
            Serialize(os, name);
            Serialize(os, quantity);
        }

        void deserialize(std::istream &is) override
        {
            Deserialize(is, name);
            Deserialize(is, quantity);
        }
    };

    class Player : public Serializable
    {
    public:
        std::string name;
        int level;
        std::vector<InventoryItem> inventory;

        Player() = default;
        Player(const std::string &name, int level, const std::vector<InventoryItem> &inventory)
            : name(name), level(level), inventory(inventory) {}
        Player(const Player &) = default;
        Player &operator=(const Player &) = default;
        Player(Player &&) noexcept = default;
        Player &operator=(Player &&) noexcept = default;

        bool operator==(const Player &other) const
        {
            return name == other.name && level == other.level && inventory == other.inventory;
        }

        void serialize(std::ostream &os) const override
        {
            Serialize(os, name);
            Serialize(os, level);
            Serialize(os, inventory);
        }

        void deserialize(std::istream &is) override
        {
            Deserialize(is, name);
            Deserialize(is, level);
            Deserialize(is, inventory);
        }
    };

    template <typename T>
    void checkSerialize(const T &obj)
    {
        std::stringstream ss;
        Serialize(ss, obj);
        T obj2;
        Deserialize(ss, obj2);
        CHECK_EQ(obj, obj2);
    }
}

TEST_CASE("serialize")
{
    SUBCASE("primitive")
    {
        checkSerialize(1);
        checkSerialize(1.0);
        checkSerialize('a');
        checkSerialize(true);
    }

    SUBCASE("std")
    {
        checkSerialize(std::string("hello"));
        checkSerialize(std::vector<int>{1, 2, 3});
        checkSerialize(std::vector<std::string>{"hello", "world"});
    }

    SUBCASE("simple class")
    {
        checkSerialize(InventoryItem("Sword", 1));
    }

    SUBCASE("complex class")
    {
        checkSerialize(Player("Player1", 1, {InventoryItem("Sword", 1), InventoryItem("Shield", 1)}));
    }
}