
#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H
#include <iostream>
#include <memory>

float curMem = 0;
float totalAllocations = 0;
float totalDeallocations = 0;
float maxMem = 0;

// new normally includes return std::malloc(numBytes);
// all we are adding is the print out




void* operator new(std::size_t numBytes) {
    std::cout << "allocating " << numBytes << " bytes of memory\n";
    curMem += numBytes;
    maxMem = curMem;
    totalAllocations++;
    return std::malloc(numBytes);
}

void operator delete(void* memoryLocation, std::size_t numBytes) {
    std::cout << "freeing " << numBytes << " bytes of memory\n";
    std::free(memoryLocation);
    curMem -= numBytes;
    totalDeallocations++;

}



struct ArraySize {
    // defines the size of the array that is being made
    std::size_t numBytes;
};

void* operator new[](std::size_t numBytes) {
    std::cout << "allocating an array of " << numBytes << " bytes of memory\n";
    ArraySize* array = reinterpret_cast<ArraySize*>(std::malloc(numBytes + sizeof(ArraySize)));
    array->numBytes = numBytes;
    curMem += array->numBytes;
    maxMem = curMem;
    totalAllocations++;


    return array + 1;
    // +1 to include numBytes (offset)
}

void operator delete[](void* memoryLocation) {
    ArraySize* array = reinterpret_cast<ArraySize*>(memoryLocation) - 1;
    // -1 to exclude numBytes
    std::cout << "freeing array " << array->numBytes << " bytes of memory\n";
    curMem -= array->numBytes;
    totalDeallocations++;

    free(array);
}





#endif