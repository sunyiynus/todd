#ifndef TODD_UTILITIES_H
#define TODD_UTILITIES_H

#include <random>
#include <bitset>
#include <cstring>


namespace todd {


const unsigned char SIG_BIT[] = {
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001,
};


class RandomGenerator
{
public:
    static RandomGenerator& global();


    template<typename T>
    const T uniformRealDist(const T a, const T b);
    template<typename T>
    std::uniform_real_distribution<T> getUniformRealDist(const T a, const T b);
    std::default_random_engine& randomEngine();

    
    std::random_device rd;
    std::default_random_engine dre;
};


inline RandomGenerator& RandomGenerator::global()
{
  static RandomGenerator randg;
  return randg;
}

template<typename T>
const T RandomGenerator::uniformRealDist(const T a, const T b)
{
    std::uniform_real_distribution<T> urd(a,b);
    return urd(dre);
}


template<typename T>
std::uniform_real_distribution<T> RandomGenerator::getUniformRealDist(const T a, const T b)
{
    std::uniform_real_distribution<T> urd(a,b);
    return urd;
}


inline std::default_random_engine& RandomGenerator::randomEngine()
{
    return dre;
}


template<std::size_t Nb>
const unsigned char* WriteBitsetToUchar(const std::bitset<Nb>& bitmap)
{
    if (bitmap.size() == 0) {
        return nullptr;
    }
    std::size_t byteSize = (bitmap.size()% sizeof(unsigned char) == 0)? 
                bitmap.size()/sizeof(unsigned char) : (bitmap.size()/sizeof(unsigned char) + 1);
    unsigned char* bitmapchar {new unsigned char[byteSize]};
    if (!bitmapchar) {
        return nullptr;
    }
    std::memset(bitmapchar, 0, byteSize);
    for (int i = 0; i < bitmap.size(); ++i) {
        std::size_t bpos = i / sizeof(unsigned char);
        std::size_t cpos = i % sizeof(unsigned char);
        if (bitmap[i])
            bitmapchar[bpos] |= (SIG_BIT[cpos]);
    }
    return bitmapchar;
}



} // namespace todd

#endif // !TODD_UTILITIES_H

