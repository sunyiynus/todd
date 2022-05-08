#include <iostream>
#include <memory>
#include <vector>
#include <catch2/catch_test_macros.hpp>

#include "datastruct.h"
#include "utilities.h"

using namespace todd;
using namespace std;


class StubFileMapper: public AbsFileMapper 
{
public:
    StubFileMapper(const uint64_t pc)
            :AbsFileMapper("nothing"),pageCount(pc) {}
    std::shared_ptr<Page> mapToMem(const uint64_t index) override;
    bool release(std::shared_ptr<Page> page) override;
protected:
    StubFileMapper() = default;
private:
    uint64_t pageCount;
};


std::shared_ptr<Page> StubFileMapper::mapToMem(const uint64_t index)
{
    std::shared_ptr<Page> ptr;
    if (index < pageCount) {
        ptr = std::make_shared<Page>(index, nullptr, nullptr);
    }
    return ptr;
}

bool StubFileMapper::release(std::shared_ptr<Page> page)
{
    if (page.get()) {
        return true;
    }
    return false;
}


TEST_CASE("File map to memory", "[class FileMapper]")
{
    std::shared_ptr<StubFileMapper> stubFileMapper (new StubFileMapper(50));
    auto ptr = stubFileMapper->getPtr();
    INFO("ptr " << stubFileMapper->getPtr());
    REQUIRE( ptr.get() == stubFileMapper.get());
    REQUIRE( 1 == 1);

    SECTION("test stub file mapper map and release") {
        auto pptr = stubFileMapper->mapToMem(10);
        REQUIRE(pptr->index == 10);
        REQUIRE(stubFileMapper->release(pptr) == true);
    }
    SECTION("test stub file mapper map and release page out of file") {
        auto pptr = stubFileMapper->mapToMem(70);
        REQUIRE(pptr.get() == nullptr);
        REQUIRE(stubFileMapper->release(pptr) == false);
    }
}

TEST_CASE("Pages manager for cache pages light 50Pages", "[class Pages]")
{
    std::shared_ptr<StubFileMapper> stubFileMapper (new StubFileMapper(50));
    std::shared_ptr<Pages> pages (new Pages(10));
    pages->setFileMapper(stubFileMapper);
    vector<int> getSeq;
    for(int i =0; i < 20; ++i) getSeq.push_back(i);

    SECTION("test get() ") {
        pages->get(10);
        REQUIRE(pages->size() == 1);
        pages->get(10);
        pages->get(10);
        pages->get(10);
        pages->get(10);
        pages->get(10);
        pages->get(10);
        REQUIRE(pages->size() == 1);
        pages->get(9);
        pages->get(8);
        pages->get(7);
        pages->get(6);
        pages->get(5);
        pages->get(4);
        pages->get(3);
        pages->get(1);
        pages->get(2);
        REQUIRE(pages->size() == 10);
        for (auto idx: getSeq) {
            pages->get(idx);
        }
        REQUIRE(pages->size() == 10);
    }
    SECTION("test max 100000") {
        auto rand = RandomGenerator::global().getUniformRealDist<double>(0, 40);
        auto dre = RandomGenerator::global().randomEngine(); 
        for (int i = 0; i < 10000000; ++i) {
            pages->get(rand(dre));
        }
        REQUIRE(pages->size() == 10); 
    }
}


TEST_CASE("Pages manager for cache pages 1G size", "[class Pages]")
{
    // 1G / 4k
    std::shared_ptr<StubFileMapper> stubFileMapper (new StubFileMapper(262144));
}


TEST_CASE("RawPage testing", "[struct RawPage]") {
    std::shared_ptr<RawPage> pagePtr = std::make_shared<RawPage>();
    unsigned char tmp[] = {0b011000, 0b000001, 0b000010};
    pagePtr->copyIn(reinterpret_cast<void*>(&tmp), sizeof(tmp));    
    REQUIRE(pagePtr->wrOff == 3);
    SECTION("test limit") {
        REQUIRE(pagePtr->wrOff == 3);
        unsigned char tmp[PAGE_SIZE];
        REQUIRE(pagePtr->copyIn(reinterpret_cast<void*>(&tmp), sizeof(tmp)) == false);
    }
    SECTION("test limit - 1") {
        pagePtr->wrOff = 0;
        unsigned char tmp[PAGE_SIZE];
        REQUIRE(pagePtr->copyIn(reinterpret_cast<void*>(&tmp), sizeof(tmp)) == true);
    }
}


TEST_CASE("Create database file and init it's format...", "[class FileMapper::createDatabaseFile()]")
{
    std::string fname {"./test-1.dbf"};
    auto res = FileMapper::createDatabaseFile(fname);
    REQUIRE(res == false);
}



