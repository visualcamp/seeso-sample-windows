#include "user_status_option.h"
/**
 * Written by June on 2021-04-26.
 * User Satus Option Class
 * */

namespace seeso {

void UserStatusOption::setUseAttention(bool isUseAttention) {
  this->isUseAttention = isUseAttention;
}

void UserStatusOption::setUseBlink(bool isUseBlink) {
  this->isUseBlink = isUseBlink;
}

void UserStatusOption::setUseDrowsiness(bool isUseDrowsiness) {
  this->isUseDrowsiness = isUseDrowsiness;
}

bool UserStatusOption::getUseAttention() const {
  return isUseAttention;
}

bool UserStatusOption::getUseBlink() const {
  return isUseBlink;
}

bool UserStatusOption::getUseDrowsiness() const {
  return isUseDrowsiness;
}

} // namespace seeso
