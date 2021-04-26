#include "user_status_option.h"
/**
 * Written by June on 2021-04-26.
 * User Satus Option Class
 * */

namespace seeso {


UserStatusOption::UserStatusOption() {
  isUseAttention = false;
  isUseBlink = false;
  isUseDrowsiness = false;
}

void UserStatusOption::setUseAttention(bool isUseAttention) {
  this->isUseAttention = isUseAttention;
}

void UserStatusOption::setUseBlink(bool isUseBlink) {
  this->isUseBlink = isUseBlink;
}

void UserStatusOption::setUseDrowsiness(bool isUseDrowsiness) {
  this->isUseDrowsiness = isUseDrowsiness;
}

bool UserStatusOption::getUseAttention() {
  return isUseAttention;
}

bool UserStatusOption::getUseBlink() {
  return isUseBlink;
}

bool UserStatusOption::getUseDrowsiness() {
  return isUseDrowsiness;
}

} // namespace seeso
