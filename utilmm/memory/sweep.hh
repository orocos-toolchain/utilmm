#ifndef __UTILS_SWEEP_H__
#define __UTILS_SWEEP_H__

namespace utilmm
{
    /** Calls delete for each iterator 
     * value in [begin, end) */
    template<typename Iterator>
    void sweep(Iterator begin, Iterator end)
    {
        while(begin != end)
        {
            delete *begin;
            ++begin;
        }
    }    
    
    /** Calls delete[] for each iterator 
     * value in [begin, end) */
    template<typename Iterator>
    void sweep_arrays(Iterator begin, Iterator end)
    {
        while(begin != end)
        {
            delete[] *begin;
            ++begin;
        }
    }
}

#endif
