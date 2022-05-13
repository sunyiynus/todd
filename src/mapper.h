#ifndef TODD_SRC_MAPPER_H
#define TODD_SRC_MAPPER_H

#include <cstdint>
#include <fcntl.h>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <list>

namespace todd {



// 1. map file into memory
// 2. manage pages
class FileMapper
{
public:
    FileMapper(): fd(-1) {}
    //virtual std::shared_ptr<Page> mapToMem(const uint64_t index) = 0;
    //virtual bool release(std::shared_ptr<Page> page) = 0;

    void autoAdjustMapedSize();

    // create and init file format;
    //static bool createDatabaseFile(const std::string& fname, FILE_FORMAT format);
    //static std::shared_ptr<FileMapper> openDatabaseFile(const std::string& fname);

    void *map(size_t pageIndex);
    void unmap(size_t pageIndex);

    void setFile(int fd);

protected:

private:
    int fd;
    std::size_t fileTotalPages;
    uint64_t error;
    struct MapUnit {
        std::size_t pageIndex;
        void *addr;
    };

    std::size_t maxMapSize;

    std::list<MapUnit> cache;
    std::unordered_map<std::size_t, std::list<MapUnit>::iterator> mapped;

};

} // namespace todd;

#endif // !TODD_SRC_MAPPER_H
