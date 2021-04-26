/**
 * Written by June on 2021-04-26.
 * User Satus Option Class
 * */

#ifndef SEESO_WINDOWS_USER_STATUS_OPTION_H
#define SEESO_WINDOWS_USER_STATUS_OPTION_H

namespace seeso {


class UserStatusOption {
 public:
   explicit UserStatusOption();

   void setUseAttention(bool isUseAttention);
   void setUseBlink(bool isUseBlink);
   void setUseDrowsiness(bool isUseDrowsiness);

   bool getUseAttention();
   bool getUseBlink();
   bool getUseDrowsiness();

private:
  int isUseAttention = false;
  int isUseBlink = false;
  int isUseDrowsiness = false;
};

} // namespace seeso

#endif //SEESO_WINDOWS_USER_STATUS_OPTION_H