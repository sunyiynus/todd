#include "mapper.h"
#include "datastruct.h"

#include<sys/mman.h>
#include <list>

using namespace todd;


void FileMapper::autoAdjustMapedSize()
{
    if (cache.size() > maxMapSize) {
        auto del = cache.end();
        del--;
        auto hashDel = mapped.find(del->pageIndex);
        unmap(del->pageIndex);
        mapped.erase(hashDel);
        cache.erase(del);
    }
}


void* FileMapper::map(size_t pageIndex)
{
    if (error) {
        return nullptr;
    }

    auto res = mapped.find(pageIndex);
    if (res != mapped.end()) {
        cache.splice(cache.begin(), cache, res->second);
        return (*res->second).addr;
    }
    void* addr = mmap(nullptr, PAGE_SIZE, 
                         PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, pageIndex*PAGE_SIZE);
    if (addr == MAP_FAILED) {
        error |= 0x10;
    } else {
        autoAdjustMapedSize();
        cache.push_front(MapUnit());
        auto& el = cache.front();
        el.pageIndex = pageIndex;
        el.addr = addr;
        mapped[pageIndex] = cache.begin();
        return addr;
    }
    return nullptr;
}


void FileMapper::unmap(size_t pageIndex)
{
    auto res = mapped.find(pageIndex);
    if (res == mapped.end()) {
        return;
    }

    // umap page
    if (munmap(res->second->addr, PAGE_SIZE) == -1) {
        // TODO some error handle
    }
    mapped.erase(pageIndex);
    return;
}


void FileMapper::setFile(int fid)
{
    if (fid == fd || fid == -1) {
        return;
    }
    for( auto itr : mapped) {
        munmap(itr.second->addr, PAGE_SIZE);
    }
    mapped.clear();
}

