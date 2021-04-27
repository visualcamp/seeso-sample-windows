/**
 * Written by June on 2021-04-26.
 * User Satus Option Class
 * */

#ifndef SEESO_FRAMEWORK_USER_STATUS_OPTION_H_
#define SEESO_FRAMEWORK_USER_STATUS_OPTION_H_

namespace seeso {

class UserStatusOption {
 public:


  void setUseAttention(bool isUseAttention);
  void setUseBlink(bool isUseBlink);
  void setUseDrowsiness(bool isUseDrowsiness);

  bool getUseAttention() const;
  bool getUseBlink() const;
  bool getUseDrowsiness() const;

 private:
  int isUseAttention = false;
  int isUseBlink = false;
  int isUseDrowsiness = false;
};

} // namespace seeso

#endif //SEESO_FRAMEWORK_USER_STATUS_OPTION_H_
