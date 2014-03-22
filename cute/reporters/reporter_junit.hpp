// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../test_result.hpp"
#include "../test_suite_result.hpp"

#include <ostream>
#include <mutex>

namespace cute {
    namespace detail {

        inline void junit_write_lead_in(
            std::ostream& out,
            test_suite_result const& suite,
            std::vector<test_result> const& tests
        ) {
            out << "<testsuites>" << std::endl;
            out << "  <testsuite ";
            out <<    "errors=\"" << suite.test_cases_passed << "\" ";
            out <<    "failures=\"0\" ";
            out <<    "tests=\"" << tests.size() << "\" ";
            out <<    "hostname=\"" << xml_encode("tbd") << "\" ";
            out <<    "time=\"" << (suite.duration_ms / 1000.0f) << "\" ";
            out <<    "timestamp=\"" << xml_encode("tbd") << "\"";
            out << ">" << std::endl;
        }

        inline void junit_write_test(
            std::ostream& out,
            test_result const& test
        ) {
            out << "    <testcase ";
            out <<      "classname=\"" << xml_encode("global") << "\" ";
            out <<      "name=\"" << xml_encode(test.test) << "\" ";
            out <<      "time=\"" << (test.duration_ms / 1000.0f) << "\"";

            if(test.result == result_type::pass) {
                out <<      "/>" << std::endl;
                return;
            }

            std::string type;
            switch(test.result) {
                case result_type::pass:     assert(false);
                case result_type::fail:     type = "fail";  break;
                case result_type::fatal:    type = "fatal"; break;
                default:                    assert(false);
            }

            out <<      ">" << std::endl;
            out << "      <failure ";
            out <<        "message=\"" << xml_encode(test.reason) << "\" ";
            out <<        "type=\"" << type << "\" ";
            out <<        ">" << std::endl;
            out << "at " << xml_encode(test.file) << ":" << test.line << std::endl;
            out << "      </failure>" << std::endl;
            out << "    </testcase>" << std::endl;
        }

        inline void junit_write_lead_out(
            std::ostream& out
        ) {
            out << "  </testsuite>" << std::endl;
            out << "/<testsuites>" << std::endl;
        }

    } // namespace detail

    inline std::ostream& reporter_junit(
        std::ostream& out,
        test_suite_result const& suite,
        std::vector<test_result> const& tests
    ) {
        detail::junit_write_lead_in(out, suite, tests);

        // write resut for each test
        for(auto&& test : tests) {
            detail::junit_write_test(out, test);
        }

        detail::junit_write_lead_out(out);

        return out;
    }

} // namespace cute
