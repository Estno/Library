#pragma once
#include <chrono>

unsigned int encodeDate(const std::chrono::year_month_day&);
std::chrono::year_month_day decodeDate(unsigned int);
unsigned int today();
const std::string dateToString(const unsigned int);
