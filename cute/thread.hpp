// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <cassert>
#include <thread>

namespace cute {

    template<typename FUNC>
    inline std::thread thread(FUNC&& func) {
        return std::thread([func]() { try { func(); } catch(...) { } });
    }

} // namespace cute
