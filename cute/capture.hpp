// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <string>
#include <vector>

namespace cute {

    struct capture {
        std::string const name;
        std::string const value;

        inline capture(std::string name_ = "", std::string value_ = "") : name(std::move(name_)), value(std::move(value_)) { }
    };
    
    struct captures {
        inline captures(
            capture cap1_ = capture(),
            capture cap2_ = capture(),
            capture cap3_ = capture(),
            capture cap4_ = capture(),
            capture cap5_ = capture()
        ) {
            if(!cap1_.name.empty()) { list.emplace_back(std::move(cap1_)); }
            if(!cap2_.name.empty()) { list.emplace_back(std::move(cap2_)); }
            if(!cap3_.name.empty()) { list.emplace_back(std::move(cap3_)); }
            if(!cap4_.name.empty()) { list.emplace_back(std::move(cap4_)); }
            if(!cap5_.name.empty()) { list.emplace_back(std::move(cap5_)); }
        }
        
        inline captures(
            captures caps1_,
            captures caps2_
        ) :
            list(std::move(caps1_.list))
        {
            for(auto&& c : caps2_.list) {
                list.emplace_back(std::move(c));
            }
        }
        
        std::vector<capture> list;
    };

} // namespace cute
