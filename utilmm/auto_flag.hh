#ifndef UTILMM_AUTOFLAG_H
#define UTILMM_AUTOFLAG_H

#include <boost/noncopyable.hpp>

namespace utilmm
{
    /** Automatically sets and resets a boolean flag
     * on construction and destruction */
    template<typename T> 
    class auto_flag
        : private boost::noncopyable
    {
        int& m_field;
        int  m_mask;
        bool m_restore;

    public:
        auto_flag(int& field, int mask, bool value = true, bool restore_old = true)
            : m_field(field), m_mask(mask)
            , m_restore(restore_old ? get() : !value) 
        {
            set(value);
        }

        ~auto_flag() { set(m_restore); }

        bool get() { return (m_field & m_mask) == m_mask; }
        void set(bool value)
        { m_field = (m_field & ~m_mask) | (m_mask * static_cast<int>(value)); }
            
        
    };

    template<>
    class auto_flag<bool>
        : private boost::noncopyable
    {
        bool& m_flag;
        bool m_restore;

        // Safe bool idiom
        struct safe_bool_struct
        { void method(); };
        typedef void (safe_bool_struct::*safe_bool)();

    public:
        /** Sets \c flag to \c init. On destruction, the flag 
         * will be set to either ! \c init if restore_old is false, or
         * to the initial flag value if restore_old is true
         */
        auto_flag(bool& flag, bool init = true, bool restore_old = true)
            : m_flag(flag), m_restore(restore_old ? flag : !init)
        { m_flag = init; }

        /** Sets the value of the flag according to the restore_old
         * argument of the constructor
         */
        ~auto_flag() { m_flag = m_restore; }

        /** Get the current value of the flag
         * @return the current value of the flag */
        bool get() const { return m_flag; }

        operator safe_bool() const { return m_flag ? &safe_bool_struct::method : 0; }
    };

}

#endif

