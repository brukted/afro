#include "log.h"

#define AF_ASSERT(condition)                                                                     \
  if (!(condition)) {                                                                            \
    afro::log::core_critical("{} failed at line {} in file {}", #condition, __LINE__, __FILE__); \
    abort();                                                                                     \
  }

#define AF_ASSERT_MSG(condition, message)                                                                      \
  if (!(condition)) {                                                                                          \
    afro::log::core_critical("{} failed at line {} in file {} : {}", #condition, __LINE__, __FILE__, message); \
    abort();                                                                                                   \
  }