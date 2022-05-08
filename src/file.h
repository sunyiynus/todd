#ifndef TODD_FILE_H
#define TODD_FILE_H

#include <memory>
#include <string>
#include <vector>

class DatabaseFile;

class DatabaseFileBitmap
{
public:
    DatabaseFileBitmap():mem(nullptr) {}
    friend class DatabaseFile;
    void setBit(const uint64_t pos, bool flag);
    void setBitOn(const uint64_t pos);
    void setBitOff(const uint64_t pos);
    void reverseBit(const uint64_t pos);
    void countBit(bool flag);
private:
    std::vector<bool> bitmap;
    char *mem;
};



class DatabaseFile
{
public:
    static std::shared_ptr<DatabaseFile> createDatabaseFile(const std::string& fname);
    DatabaseFile(const std::string& fname);
private:
    
};

#endif // TODD_FILE_H
