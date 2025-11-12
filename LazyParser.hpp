// lazyparser.hpp 
// 
// A Lightweight, lazy delimited string parser implemented in modern C++
//
// Version 0.0.1 Built 251101
// Copyright 2025 DOF Studio

#include <vector>
#include <string>
#include <string_view>
#include <stdexcept>

#pragma once

/* 
 * Must Support C++17
 */
#if !defined(_MSC_VER) && __cplusplus < 201703L
#error "C++17 or higher is required"
#elif defined(_MSC_VER) && _MSC_VER < 1914
#error "MSVC 2017 (15.7) or higher is required for C++17 support"
#endif

/*
 * Lazy Parser - enabling Online Parsing
 */
class LazyParser {
private:
    std::string_view _string;                            // original view
    std::vector<std::pair<size_t, size_t>> _start_poses; // <start_pos, len>
    size_t _cur_pos = 0;                                 // current position, 1st byte of next or 1 byte exceeding last
    char _delim = ',';

public:
    // Constructor
    LazyParser(std::string_view row, char delim) noexcept : _string(row), _delim(delim) {
        this->_start_poses.reserve(this->_string.size() / 10); // apprx
    }

    // Constructor by moving
    LazyParser(std::string_view&& row, char delim) noexcept : _string(std::move(row)), _delim(delim) {
        this->_start_poses.reserve(this->_string.size() / 10); // apprx
    }

    // Destructor
    ~LazyParser() noexcept {

    }

private:
    // Parse next element (return false if already to the end)
    inline bool _parse_next() noexcept {
        // If exceeding the length, return false
        const size_t tsize = this->_string.size();
        if (this->_cur_pos >= tsize) {
            return false;
        }

        // Let's check if the next char is just the delim, if so, directly save nothing
        if (this->_string[this->_cur_pos] == this->_delim) {
            // Append one
            this->_start_poses.emplace_back(this->_cur_pos, 0);
            this->_cur_pos++;
            return true;
        }

        // Else, try to parse until delim
        size_t end_pos = this->_cur_pos;
        while (end_pos < tsize) {
            // Meets the delim
            if (this->_string[end_pos] == this->_delim) {
                // Append one
                this->_start_poses.emplace_back(this->_cur_pos, end_pos - this->_cur_pos);
                this->_cur_pos = end_pos + 1;

                // If at the end of string and if last char was a delimiter, append an empty field
                if (this->_cur_pos == tsize) {
                    this->_start_poses.emplace_back(this->_cur_pos, 0);
                }
                return true;
            }
            ++end_pos;
        }

        // If has parsed something
        if (end_pos > this->_cur_pos) {
            this->_start_poses.emplace_back(this->_cur_pos, end_pos - this->_cur_pos);
            this->_cur_pos = end_pos; // already end
            return true;
        }
        else {
            return false;
        }
    }

public:
    // Get the ith element (may throw)
    inline std::string_view get_ith(size_t ith) {
        // If ith is already parsed
        const size_t inum = ith + 1;
        if (inum <= this->_start_poses.size()) {
            return this->_string.substr(_start_poses[ith].first, _start_poses[ith].second);
        }

        // Parse until meet
        while (inum > this->_start_poses.size()) {
            bool _ret = this->_parse_next();
            if (_ret == false) {
                // out of range
                throw std::runtime_error("In get_ith(ith = " + std::to_string(ith) + "), arg ith exceeded the maximum element contained.");
            }
        }
        return this->_string.substr(_start_poses[ith].first, _start_poses[ith].second);
    }
};
