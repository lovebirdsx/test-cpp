#include <doctest.h>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>

namespace
{
    class Serializable
    {
    public:
        virtual ~Serializable() = default;

        virtual void serialize(std::ostream &os) = 0;
        virtual void deserialize(std::istream &is) = 0;
    };

    template <typename T>
    typename std::enable_if<std::is_trivial<T>::value>::type Serialize(std::ostream &os, const T &obj)
    {
        os.write(reinterpret_cast<const char *>(&obj), sizeof(T));
    }

    template <typename T>
    typename std::enable_if<std::is_trivial<T>::value>::type Deserialize(std::istream &is, T &obj)
    {
        is.read(reinterpret_cast<char *>(&obj), sizeof(T));
    }

    template <typename T>
    typename std::enable_if<!std::is_trivial<T>::value>::type Serialize(std::ostream &os, const T &obj)
    {
        obj.serialize(os);
    }

    template <typename T>
    typename std::enable_if<!std::is_trivial<T>::value>::type Deserialize(std::istream &is, T &obj)
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
        is.read(str.data(), size);
    }

    template <typename T>
    void Serialize(std::ostream &os, const std::vector<T> &vec)
    {
        size_t size = vec.size();
        Serialize(os, size);
        for (const auto &elem : vec)
        {
            Serialize<T>(os, elem);
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
            Deserialize<T>(is, elem);
        }
    }

    class InventoryItem : public Serializable
    {
    public:
        std::string name;
        int quantity;

        InventoryItem() : name(""), quantity(0) {}
        InventoryItem(const std::string &name, int quantity) : name(name), quantity(quantity) {}

        void serialize(std::ostream &os) override
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

        Player() : name(""), level(0), inventory() {}
        Player(const std::string &name, int level, const std::vector<InventoryItem> &inventory) : name(name), level(level), inventory(inventory) {}

        void serialize(std::ostream &os) override
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
}

TEST_CASE("serialize")
{
    SUBCASE("simple")
    {

    }

    SUBCASE("complex")
    {
        Player player("Alice", 1, {InventoryItem("Sword", 1), InventoryItem("Shield", 1)});

        std::stringstream ss;
        player.serialize(ss);

        // Player player2;
        // player2.deserialize(ss);

        // CHECK_EQ(player2.name, "Alice");
        // CHECK_EQ(player2.level, 1);
        // CHECK_EQ(player2.inventory.size(), 2);
        // CHECK_EQ(player2.inventory[0].name, "Sword");
        // CHECK_EQ(player2.inventory[0].quantity, 1);
        // CHECK_EQ(player2.inventory[1].name, "Shield");
        // CHECK_EQ(player2.inventory[1].quantity, 1);
    }
}
