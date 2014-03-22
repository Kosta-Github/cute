// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "macros.hpp"
#include "detail/decomposer.hpp"

#include <thread>

namespace cute {

    struct tick {
        inline tick(int start_value_ = 0) : m_value(start_value_), m_value_working_on(start_value_) { }

        template<typename FUNC>
        inline void at_tick(int tick, FUNC&& func) {
            while(m_value < tick) { std::this_thread::yield(); }
            ++m_value_working_on;
            CUTE_ASSERT(m_value == tick, CUTE_CAPTURE("at_tick() before execution()"));
            CUTE_ASSERT(m_value_working_on == tick+1, CUTE_CAPTURE("at_tick() before execution"));
            func();
            CUTE_ASSERT(m_value == tick, CUTE_CAPTURE("at_tick() after execution"));
            CUTE_ASSERT(m_value_working_on == tick+1, CUTE_CAPTURE("at_tick() after execution"));
            ++m_value;
        }

        inline void reached_tick(int tick) {
            at_tick(tick, []() { });
        }

        template<typename FUNC>
        inline void blocks_until_tick(int tick, FUNC&& func) {
            func();
            CUTE_ASSERT(m_value_working_on >= tick, CUTE_CAPTURE("blocks_until_tick() after execution"));
        }

        template<typename DELAY>
        inline void delay_tick_for(int tick, const DELAY& delay) {
            at_tick(tick, [=]() { std::this_thread::sleep_for(delay); });
        }

    private:
        std::atomic<int> m_value;
        std::atomic<int> m_value_working_on;
    };

} // namespace cute
