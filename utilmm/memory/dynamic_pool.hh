#ifndef __UTILS_SLABS_H__
#define __UTILS_SLABS_H__

#include <vector>
#include <boost/cstdint.hpp>

namespace utilmm
{
    namespace pools
    {
        template<typename T>
        class dynamic_auto;

        /** A memory_pool is a way to handle allocation and reallocation of 
         * fixed-length memory items, but with the ability to change the 
         * size at runtime. It is designed to be used for small classes with 
         * subclasses 
         *
         * NEVER put the pointer returned by dynamic_pool into an auto_ptr. Use
         * dynamic_ptr instead.
         */
        
        class dynamic_pool
        {
            template<typename T>
            friend class dynamic_auto;
                
        public:
            typedef boost::uint32_t size_t;

        private:
            typedef boost::uint8_t byte_t;
            
            struct item_t
            {
                size_t  size;
                byte_t payload[];
            };

            typedef std::vector<item_t*> Buffers;

            static item_t* get_base(void* ptr);

            size_t  m_size;
            Buffers m_listpool, m_free;

        public:
            dynamic_pool();
            ~dynamic_pool();

            void* allocate(size_t size);
            void deallocate(void* vbuffer);
        };


        template<typename T>
        class dynamic_auto
        {
            T* m_ptr;

            struct safe_bool_struct
            { void method(); };
            typedef void (safe_bool_struct::*safe_bool)();

        public:
            dynamic_auto(T* t) throw()
                : m_ptr(t) {}

            ~dynamic_auto()
            { 
                if (m_ptr)
                    delete[] dynamic_pool::get_base(m_ptr); 
            }

            dynamic_auto(dynamic_auto const& copy) throw()
            {
                m_ptr = copy.m_ptr;
                copy.m_ptr = 0;
            }

            void reset(T* t) throw()
            { m_ptr = t; }

            operator safe_bool() const throw()
            { return m_ptr ? &safe_bool_struct::method : 0; }

            T* get() throw() { return m_ptr; }
            T const* get() const throw() { return m_ptr; }
            T* release() throw()
            {
                T* temp = m_ptr;
                m_ptr = 0;
                return temp;
            }
        };

    }
}

#endif


