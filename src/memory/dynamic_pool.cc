#include <utilmm/memory/dynamic_pool.hh>
#include <utilmm/memory/sweep.hh>

#include <boost/cstdint.hpp>

using namespace utilmm::pools;

dynamic_pool::dynamic_pool() : m_size(0) {}
dynamic_pool::~dynamic_pool()
{ sweep(m_free.begin(), m_free.end()); }

dynamic_pool::item_t* dynamic_pool::get_base(void* ptr)
{ 
    byte_t* base = reinterpret_cast<byte_t*>(ptr) - offsetof(item_t, payload);
    return reinterpret_cast<item_t*>(base); 
}

void* dynamic_pool::allocate(size_t size)
{
    if (m_size < size)
    {
        sweep(m_free.begin(), m_free.end());
        m_free.clear();
        m_size = size;
    }

    item_t* item;
    if (m_free.empty())
    {
        byte_t* buffer = new byte_t[sizeof(item_t) + m_size];
        item = new(buffer) item_t;
        item->size = m_size;
    }
    else
    {
        item = m_free.back();
        m_free.pop_back();
    }

    return item->payload;
}

void dynamic_pool::deallocate(void* vbuffer)
{
    if (! vbuffer) return;
        
    item_t* item = get_base(vbuffer);

    if (item->size != m_size)
        delete item;
    else
        m_free.push_back(item);
}

