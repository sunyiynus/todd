#include "datastruct.h"
#include "utilities.h"

#include <cstdint>
#include <memory>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <vector>
#include <bitset>

using namespace todd;
using namespace std;

Page::Page(const uint64_t id, char *e, const char* ph)
    :index(id), end(e), pageHead(ph) {}

Page::~Page()
{
    // TODO
}

Pages::Pages(const SizeType sc): cache_size(sc) {}

Pages::PageIterator Pages::get(const uint64_t index)
{
    assert(fileMapper.get() != nullptr);
    auto itr = hash.find(index);
    if (itr == hash.end()) {
        return fetch(index);
    }

    cache.splice(cache.begin(), cache, itr->second);
    return itr->second;
}


Pages::PageIterator Pages::fetch(const uint64_t index)
{
    if (cache.size() >= cache_size) {
        // release source
        auto itr = cache.end();
        std::advance(itr, -1);
        std::shared_ptr<Page> pagePtr = *itr;
        auto itrh = hash.erase(pagePtr->index);
        //std::advance(itrh, -1);
        cache.erase(itr);
        
    }
    cache.push_front(fileMapper->mapToMem(index));
    hash[index] = cache.begin();
    assert(cache.size() == hash.size());
    return cache.begin();
}


void Pages::setFileMapper(std::shared_ptr<AbsFileMapper> fmapper)
{
    fileMapper = fmapper;
}


std::shared_ptr<Pages> Pages::getSharedPtr()
{
    std::shared_ptr<Pages> ptr = shared_from_this();
    return ptr;
}


const Pages::SizeType Pages::size() const
{
    return cache.size();
}


AbsFileMapper::AbsFileMapper(const std::string& fname, int ps)
    :mps(ps)
{
    //openFile(fname);
}


int FileMapper::openFile(const std::string& fname)
{
    // open file mode should have some improvement
    fd = open(fname.c_str(), O_RDWR);
    if (fd ==  -1) {
        error |= ERRORS::FILE_OPEN_FAILURE;
    } else {
        mapFlag |= 0x1;
    }
    return fd;
}

char *FileMapper::memmap(const uint64_t index)
{
    if (mapFlag & 0x1)
        return nullptr;
    void* addr = mmap(nullptr, static_cast<size_t>(mapPageSize), 
                         PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, index*mapPageSize);
    if (addr == MAP_FAILED) {
        mapFlag |= 0x10;
    } else {
        return static_cast<char*>(addr);
    }
    return nullptr;
}


bool FileMapper::createDatabaseFile(const std::string& fname) {
    if (access(fname.c_str(), F_OK) == 0) {
        return false;
    }

    constexpr uint64_t pageBits = 4 * 1024 * 1024 / 4;
    int fd = open(fname.c_str(), O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    // write first page
    uint64_t pagesAlloc = 129;
    std::shared_ptr<RawPage> pagePtr = std::make_shared<RawPage>();
    pagePtr->copyIn(static_cast<void*>(&pagesAlloc), sizeof(uint64_t));
    size_t bytes = write(fd, &(pagePtr->rawpage), PAGE_SIZE);

    std::bitset<pageBits> bitmap;
    for (int i = 0; i < 129; ++i) {
        bitmap[i] = true;
    } 
    const unsigned char* bitptr = WriteBitsetToUchar(bitmap);
  
    //off_t res = lseek(fd, 4*1024, SEEK_SET);
    close(fd);
    return true;
}




