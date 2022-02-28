#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <numeric>

using std::string;
using std::cout;
using std::map;
using std::exception;
using std::domain_error;
using std::transform;
using std::accumulate;

using Name = string;
using Size = size_t;

inline Size operator"" _bytes(unsigned long long value) { return Size(value); }
inline Size operator"" _kBytes(unsigned long long value) { return Size(value * 1024); }

class NonCopyable {
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() noexcept = default;
};

class Element : private NonCopyable {
    const Name name;
protected:
    Element(const Name& name) :
        name{ name }
    {
        if (name.empty())
            throw domain_error("invalid element name");
    }
    virtual ~Element() = default;
public:
    virtual Size getSize() const noexcept = 0;

    friend class Folder;
};

class File final : public Element {
    const Size size;
    File(const Name& name, Size size) :
        Element{ name },
        size{ size }
    {
    }
    ~File() = default;

public:
    Size getSize() const noexcept override { return size; }

    friend class Folder;
};


class Folder final : public Element {
    using Key = Name;
    map<Key, const Element*> children;

    using Element::Element;

    ~Folder() {
        for (const auto& [_, element] : children)
            delete element;
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

public:
    static Folder& getRoot() noexcept {
        static Folder root{ "R1" };
        return root;
    }

    File& createFile(const Name& file_name, Size file_size) {
        const Key key{ checkNameAvailability(file_name) };
        File* file{ new File{ file_name, file_size } };
        children[key] = file;
        return *file;
    }

    Folder& createFolder(const Name& folder_name) {
        const Key key{ checkNameAvailability(folder_name) };
        Folder* folder{ new Folder{folder_name} };
        children[key] = folder;
        return *folder;
    }

    Size getSize() const noexcept override {
        return accumulate(
            cbegin(children),
            cend(children),
            0_bytes,
            [](Size sum, const auto& pair) {return sum + pair.second->getSize(); }
        );
    }
};


int main()
{
    try {
        Folder& R1{ Folder::getRoot() };
        File& F1{ R1.createFile("F1", 899_bytes) };
        Folder& R2{ R1.createFolder("R2") };
        R2.createFile("F2", 12_kBytes);
        cout << R1.getSize() << " bytes\n";
    }
    catch (const exception& e) {
        cout << e.what() << "\n";
    }
}
