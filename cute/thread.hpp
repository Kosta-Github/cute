// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <cassert>
#include <thread>

namespace cute {

    struct thread {
        template<typename FUNC>
        inline thread(FUNC func) : m_thread([func]() { try { func(); } catch(...) { } }) { }
        inline thread(thread&& o) CUTE_NOEXCEPT : m_thread(std::move(o.m_thread)) { }
        inline thread& operator=(thread&& o) CUTE_NOEXCEPT { m_thread = std::move(o.m_thread); return *this; }
        inline ~thread() { if(m_thread.joinable()) { m_thread.join(); } }

        inline void join() { m_thread.join(); }

    private:
        std::thread m_thread;
    };

} // namespace cute
