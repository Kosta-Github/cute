// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "to_string.hpp"

namespace cute {
    namespace detail {

        struct decomposer {

            template<typename T>
            struct expression {
                const T obj;

                inline expression(T obj_) : obj(std::move(obj_)) { }

                inline operator std::string() const { return to_string(obj); }

                template<typename R> inline std::string operator!() const { std::ostringstream os; os << '!' << to_string(obj); return os.str(); }

                template<typename R> inline std::string operator==(R const& rhs) const { return bin_op_to_string("==", rhs); }
                template<typename R> inline std::string operator!=(R const& rhs) const { return bin_op_to_string("!=", rhs); }
                template<typename R> inline std::string operator< (R const& rhs) const { return bin_op_to_string("<" , rhs); }
                template<typename R> inline std::string operator<=(R const& rhs) const { return bin_op_to_string("<=", rhs); }
                template<typename R> inline std::string operator> (R const& rhs) const { return bin_op_to_string(">" , rhs); }
                template<typename R> inline std::string operator>=(R const& rhs) const { return bin_op_to_string(">=", rhs); }

                template<typename R> inline std::string operator&&(R const& rhs) const { return bin_op_to_string("&&", rhs); }
                template<typename R> inline std::string operator||(R const& rhs) const { return bin_op_to_string("||", rhs); }

                template<typename L>
                inline std::string unary_op_to_string(std::string const& op, L const& lhs) { std::ostringstream os; os << op << ' ' << to_string(lhs); return os.str(); }

                template<typename R>
                inline std::string bin_op_to_string(char const* op, R const& rhs) const {
                    std::ostringstream os; os << to_string(obj) << ' ' << op << ' ' << to_string(rhs); return os.str();
                }
                
            };

            template<typename T>
            inline expression<T const&> operator->*(T const& op) {
                return expression<T const&>(op);
            }
        };

    } // namespace detail
} // namespace cute
