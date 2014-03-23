cute: C++ unit test executor [![Build Status](https://travis-ci.org/Kosta-Github/cute.png)](https://travis-ci.org/Kosta-Github/cute)
============================
Feature set:
- uses C++11 features
- no external dependencies
- header-only
- thread-safe
- supports tags for test case filtering
- can use custom reporters (provided: simple ide/console reporter, JUnit XML reporter)
- provides capabilities for mocking classes and interfaces (planned)

status: alpha
=============
The development has just started and is not considered to be stable at the moment.

related work
============
Based on the following ideas and frameworks:
- [lest](https://github.com/martinmoene/lest)
- [catch](https://github.com/philsquared/Catch)
- [turtle](http://turtle.sourceforge.net/)

dependencies
============
- [cmake](http://cmake.org/) (only required for building and executing the self-test unit test suite)
