#ifndef TODD_DATASTRUCT_H
#define TODD_DATASTRUCT_H

#include <cstddef>
#include <cstdint>

#include <cstring>
#include <list>
#include <unordered_map>
#include <memory>
#include <string>

namespace todd {

class AbsFileMapper;

constexpr uint64_t PAGE_SIZE = 1024 * 4; 

struct Page{
    Page(const uint64_t id, char *e, const char* ph);
    uint64_t index;
    char *end;
    const char* pageHead;
    int fd;
    ~Page();
protected:
    Page(const Page& page) = delete;
    Page() = delete;
};

struct RawPage
{
    RawPage():wrOff(0) {
        std::memset(rawpage, 0, PAGE_SIZE);
    }
    unsigned char rawpage[PAGE_SIZE];
    bool copyIn(const void* src, std::size_t count);
    std::size_t wrOff;
};


class Pages: public std::enable_shared_from_this<Pages> {
public:
    typedef std::list<std::shared_ptr<Page>>::size_type SizeType;
    typedef std::list<std::shared_ptr<Page>>::iterator PageIterator;
    Pages(const SizeType cs);
    
    PageIterator get(const uint64_t index);
    PageIterator fetch(const uint64_t index);
    void setFileMapper(std::shared_ptr<AbsFileMapper> fmapper);
    std::shared_ptr<Pages> getSharedPtr();

    const SizeType size() const;

private:
    std::list<std::shared_ptr<Page>>::size_type cache_size;
    std::list<std::shared_ptr<Page>> cache;
    std::unordered_map<uint64_t, std::list<std::shared_ptr<Page>>::iterator> hash; 
    std::shared_ptr<AbsFileMapper> fileMapper;
};


class AbsFileMapper:public std::enable_shared_from_this<AbsFileMapper>
{
public:
    enum ERRORS {FILE_OPEN_FAILURE = 0x1};
    AbsFileMapper(const std::string& fname, int ps = PAGE_SIZE);
    virtual std::shared_ptr<Page> mapToMem(const uint64_t index) = 0;
    virtual bool release(std::shared_ptr<Page> page) = 0;
    std::shared_ptr<AbsFileMapper> getPtr();

protected:
    AbsFileMapper():mps(PAGE_SIZE) {}
    uint64_t mps;
};


struct FileHeader
{
    uint64_t pages;
};

struct LinkedList
{
    uint64_t pageNextOffset;
    uint64_t pagePrevOffset;
};

struct AddressBookHeader
{
    LinkedList list;
    uint64_t owner;
    uint64_t pagesCount;
    uint64_t pages;
};


// 1. map file into memory
// 2. manage pages
class FileMapper: public AbsFileMapper
{
public:
    virtual std::shared_ptr<Page> mapToMem(const uint64_t index) = 0;
    virtual bool release(std::shared_ptr<Page> page) = 0;

    // create and init file format;
    static bool createDatabaseFile(const std::string& fname);
    static std::shared_ptr<FileMapper> openDatabaseFile(const std::string& fname);

protected:
    int openFile(const std::string& fname);
    char *memmap(const uint64_t index);

private:
    int mapPageSize;
    int fd;
    uint64_t filePageSize;
    uint64_t mapFlag;
    uint64_t error;

};


inline bool RawPage::copyIn(const void *src, std::size_t count)
{
    if ((PAGE_SIZE - wrOff) >= count) {
        std::memcpy(&rawpage[wrOff], src, count);
        wrOff += count;
        return true;
    } else {
        return false;
    }
}


inline std::shared_ptr<AbsFileMapper> AbsFileMapper::getPtr()
{
    std::shared_ptr<AbsFileMapper> ptr = shared_from_this();
    return ptr;
}

}  //namespace todd


#endif // TODD_DATASTRUCT_H
