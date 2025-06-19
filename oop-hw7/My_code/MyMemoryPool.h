#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include <vector>
#include <iostream>
#include <array>
#include <cstddef>
#include <new>

class MemoryPool {
public:
    // Guarantee a single instance of MemoryPool be created
    static MemoryPool& instance() noexcept {
        static MemoryPool pool;
        return pool;
    }
    // Forbid copy construction and assignment
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    // Allocate Method
    void *allocate(std::size_t data_size)
    {
        if(data_size == 0)
            return nullptr;
        if(data_size > MaxBlockSize)
            return ::operator new(data_size);
        unsigned idx = calculate_bucket_index(data_size);
        FreeNode* &head = free_head[idx];
        if(!head)
            refill(idx);
        FreeNode* node = head;
        head = head->next;
        return node;
    }
    void deallocate(void* ptr, std::size_t data_size)
    {
        if(ptr == nullptr || data_size == 0)
            return;
        if(data_size > MaxBlockSize) {
            ::operator delete(ptr);
            return;
        }
        unsigned idx = calculate_bucket_index(data_size);
        FreeNode* node = reinterpret_cast<FreeNode*>(ptr);
        node->next = free_head[idx];
        free_head[idx] = node;
    }
private:
    MemoryPool(){free_head.fill(nullptr);};
    ~MemoryPool(){
        for (unsigned i = 0; i < bucket_kind; ++i) {
            for (char* slab : slabs[i]) {
                ::operator delete(slab);
            }
        }
    }
    struct FreeNode { FreeNode* next; };
    static constexpr unsigned bucket_count = 128;
    static constexpr std::size_t MaxBlockSize = 256;
    static constexpr std::array<std::size_t,6> bucket_scale = {8,16,32,64,128,256};
    static constexpr std::size_t bucket_kind = 6;
    std::array<FreeNode*,bucket_kind> free_head;
    std::array<std::vector<char*>,bucket_kind> slabs;
    static unsigned calculate_bucket_index(std::size_t data_size){
        for (unsigned i = 0; i < bucket_kind; ++i) {
            if (data_size <= bucket_scale[i]) 
                return i;
        }
        throw std::bad_alloc(); // Should not reach here, as MaxBlockSize is 256
    }
    void refill(unsigned idx)
    {
        std::size_t bucket_size = bucket_scale[idx];
        std::size_t slab_size = bucket_count * bucket_size;
        char* slab = static_cast<char*>(::operator new(slab_size));
        slabs[idx].push_back(slab);
        for(std::size_t i = 0; i < bucket_count; ++i) {
            FreeNode* node = reinterpret_cast<FreeNode*>(slab + i * bucket_size);
            node->next = free_head[idx];
            free_head[idx] = node;
        }
    }
};

#endif