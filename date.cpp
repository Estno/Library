#include "date.h"
unsigned int encodeDate(const std::chrono::year_month_day& date) {
    std::chrono::sys_days epoch{ std::chrono::year{1970} / std::chrono::January / 1 };
    std::chrono::sys_days dateDays = std::chrono::sys_days(date);
    auto daysSinceEpoch = dateDays - epoch;
    return static_cast<unsigned int>(daysSinceEpoch.count());
}

std::chrono::year_month_day decodeDate(unsigned int daysSinceEpoch) {
    std::chrono::sys_days epoch{ std::chrono::year{1970} / std::chrono::January / 1 };
    std::chrono::sys_days dateDays = epoch + std::chrono::days{ daysSinceEpoch };
    return std::chrono::year_month_day{ dateDays };
}

unsigned int today() {
    return encodeDate(std::chrono::year_month_day{ std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now()) });
}
const std::string dateToString(const unsigned int date) {
    std::chrono::year_month_day ymd = decodeDate(date);
    std::ostringstream oss;
    oss << ymd;
    return oss.str();
}