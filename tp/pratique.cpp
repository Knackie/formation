#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <optional>
#include <memory>
#include <functional>

 

using std::string;
using std::cout;
using std::map;
using std::exception;
using std::domain_error;
using std::transform;
using std::accumulate;
using std::optional;
using std::ostream;
using std::unique_ptr;
using std::default_delete;
using std::ref;

 

using Name = string;
using Size = size_t;
using Capacity = Size;

 

inline Size operator"" _bytes(unsigned long long value) { return Size(value); }
inline Size operator"" _kBytes(unsigned long long value) { return Size(value * 1024); }

 

class NonCopyable {
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

 

protected:
    NonCopyable() noexcept = default;
};

 


template <typename T>
class Counter {
    static size_t& getCounter() {
        static size_t counter{};
        return counter;
    }
public:
    Counter() { ++getCounter(); }
    Counter(const Counter&) : Counter{} { }
    Counter(Counter&&) noexcept : Counter{} { }
    ~Counter() { --getCounter(); }
    static size_t getNbInstances() { return getCounter(); }
};

 


class Element : private NonCopyable {
    const Name name;
    const class Folder* const parent;
    virtual const string& getType() const noexcept = 0;
    virtual void onElementDisplayed(ostream& o, unsigned indentLevel) const noexcept = 0;
    void output(ostream& o, unsigned indentLevel) const noexcept {
        o << string(indentLevel * 2, ' ') << getType() << ": " << name;
        onElementDisplayed(o, indentLevel);
    }

 

protected:
    Element(const Name& name, const Folder* parent) :
        name{ name },
        parent{ parent }
    {
        if (name.empty())
            throw domain_error("invalid element name");
    }
    virtual ~Element() = default;
    const Folder* getParent() const noexcept { return parent; }

 

public:
    virtual Size getSize() const noexcept = 0;
    Name getAbsoluteName() const noexcept;

 

    friend ostream& operator<< (ostream& o, const Element& e) {
        e.output(o, 0);
        return o;
    }

 

    friend Folder;
    friend default_delete <const Element>;
};

 


class File final : public Element, public Counter<File> {
    const Size size;
    File(const Name& name, Size size, const Folder& parent);
    ~File() = default;

 

    const string& getType() const noexcept override {
        static const string type{ "File" };
        return type;
    }

 

    void onElementDisplayed(ostream& o, unsigned indentLevel) const noexcept override {}

 

public:
    Size getSize() const noexcept override { return size; }

 

    friend class Folder;
};

 


class Folder : public Element, public Counter<Folder> {
    using Key = Name;
    map<Key, unique_ptr<const Element>> children;
    mutable optional<Size> computedSize;

 

    template <class T, typename... Args>
    T& createElement(const Name& element_name, Args... args) {
        const Key key{ checkNameAvailability(element_name) };
        T* element{ new T{element_name, args...} };
        children[key].reset(element);
        if (element->getSize())
            invalidateSize();
        return *element;
    }

 

    void invalidateSize() const {
        computedSize.reset();
        if (getParent())
            getParent()->invalidateSize();
    }
    
    const string& getType() const noexcept override {
        static const string type{ "Folder" };
        return type;
    }

 

    void onElementDisplayed(ostream& o, unsigned indentLevel) const noexcept override {
        ++indentLevel;
        for (const auto& [_, element] : children) {
            o << "\n";
            element->output(o, indentLevel);
        }
    }
    
    static Key keyFromName(const Name& element_name) noexcept {
        Key key{ element_name };
        std::transform(cbegin(key), cend(key), begin(key), ::tolower);
        return key;
    }

 

    Key checkNameAvailability(const Name& element_name) const {
        const Key key{ keyFromName(element_name) };
        if (children.find(key) != cend(children))
            throw domain_error{ element_name + " already exists" };
        return key;
    }

 

protected:
    using Element::Element;
    ~Folder() = default;

 

public:

 

    Folder& createFolder(const Name& folder_name) {
        return createElement <Folder>(folder_name, this);
    }

 

    File& createFile(const Name& file_name, Size file_size) {
        return createElement <File>(file_name, file_size, ref(*this));
    }

 

    void removeElement(const Name& element_name) {
        auto itor{ children.find(keyFromName(element_name)) };
        if (itor == cend(children))
            throw domain_error{ element_name + " doesn't exist" };
        if (itor->second->getSize())
            invalidateSize();
        children.erase(itor);
    }

 

    Size getSize() const noexcept override {
        if (!computedSize)
            computedSize = accumulate(
                cbegin(children),
                cend(children),
                0_bytes,
                [](Size sum, const auto& pair) {return sum + pair.second->getSize(); }
        );
        return computedSize.value();
    }
};

 

Name Element::getAbsoluteName() const noexcept {
    Name leftPart{ parent != nullptr ? parent->getAbsoluteName() : "" };
    return leftPart + "/" + name;
}

 


class Partition final : public Folder {
    const Capacity capacity;
    Partition(const Name& name, Capacity capacity) :
        Folder{ name, nullptr },
        capacity{ capacity }
    {
    }
    ~Partition() = default;

 

    void checkRemainingSize(Size desiredSize) {
        if (capacity - getSize() < desiredSize)
            throw domain_error{ "capacity overflow" };
    }

 

    const string& getType() const noexcept override {
        static const string type{ "Partition" };
        return type;
    }

 

public:
    static Partition& getInstance() {
        static Partition root{ "R1", 30_kBytes };
        return root;
    }

 

    friend class File;
};

 

File::File(const Name& name, Size size, const Folder& parent) :
    Element{ name, &parent },
    size{ size }
{
    Partition::getInstance().checkRemainingSize(size);
}

 

int main()
{
    try {
        Partition& R1{ Partition::getInstance() };
        File& F1{ R1.createFile("F1", 899_bytes) };
        Folder& R2{ R1.createFolder("R2") };
        File& F2{ R2.createFile("F2", 20_kBytes) };
        cout << R1 << "\n";
        cout << File::getNbInstances() << " file(s)\n";
        cout << Folder::getNbInstances() << " folder(s)\n";
        cout << R1.getSize() << " bytes\n";
    }
    catch (const exception& e) {
        cout << e.what() << "\n";
    }
}
