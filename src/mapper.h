#ifndef TODD_SRC_MAPPER_H
#define TODD_SRC_MAPPER_H

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdint>
#include <cassert>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <list>

namespace todd {


class AbsMapper
{
public:
    AbsMapper();
    AbsMapper(AbsMapper &&) = default;
    AbsMapper(const AbsMapper &) = default;
    AbsMapper &operator=(AbsMapper &&) = default;
    AbsMapper &operator=(const AbsMapper &) = default;
    ~AbsMapper();

    virtual void autoAdjustMapedSize() = 0;
    virtual void *map(size_t pageIndex) = 0;
    virtual void unmap(size_t pageIndex) = 0;
    virtual void setFile(int fd) = 0;
private:
    
};

inline AbsMapper::AbsMapper()
{
}

inline AbsMapper::~AbsMapper()
{
}

// 1. map file into memory
// 2. manage pages
class FileMapper: public AbsMapper
{
public:
    FileMapper(const int fd, const std::size_t cacheSize);
    void autoAdjustMapedSize() override;
    void *map(size_t pageIndex) override;
    void unmap(size_t pageIndex) override;
    void setFile(int fd) override;

protected:
    FileMapper(): fd(-1) {}

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

inline FileMapper::FileMapper(const int fid, const std::size_t cacheSize)
{
    if (fid >= -1 && fid < 3) {
        error |= 0b10;
    } else {
        struct stat statbuf;
        fstat(fd, &statbuf);
    }
}

} // namespace todd;

#endif // !TODD_SRC_MAPPER_H
