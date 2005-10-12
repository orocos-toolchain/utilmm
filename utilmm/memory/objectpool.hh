#ifndef SIM_OBJECTPOOL_HH
#define SIM_OBJECTPOOL_HH

#include <list>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <utilmm/memory/sweep.hh>

namespace utilmm
{
    namespace pools
    {
        /** A pool of already-constructed objects
         *
         * This class manages a list of heap-allocated
         * constructed objects.
         *
         * The pool is thread-safe
         */

        template<typename T>
        class object_pool : boost::noncopyable
        {
            boost::mutex m_mutex;
            std::list<T*> m_available;

            typedef boost::mutex mutex;

        public:
            ~object_pool()
            { sweep(m_available.begin(), m_available.end()); }

            T* get() 
            { mutex::scoped_lock lock(m_mutex); 
                if (m_available.empty())
                    m_available.push_back(new T());

                T* ret = m_available.front();
                m_available.pop_front();
                return ret;
            }

            void put(T* object)
            { mutex::scoped_lock lock(m_mutex);
                m_available.push_back(object);
            }
        };

        /** Get a pointer on a T object from an object pool,
         * and returns it to the pool when destroyed
         */
        template<typename T>
        class use : boost::noncopyable
        {
            object_pool<T>& m_pool;
            T* m_object;
        
        public:
            use(object_pool<T>& pool)
                : m_pool(pool), m_object(pool.get()) {}
            ~use() { m_pool.put(m_object); }

            T* operator -> () { return m_object; }
            T const* operator ->() const { return m_object; }
            T& operator *() { return *m_object; }
            T const& operator *() const { return *m_object; }
        };
    }
}

#endif

