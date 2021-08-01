include(${CMAKE_CURRENT_LIST_DIR}/GitInfo.cmake)

string(TIMESTAMP build_date "%B %d %Y" UTC)
string(TIMESTAMP build_time "%H:%M:%S" UTC)

set(build_branch ${GIT_BRANCH})
set(build_hash ${GIT_REVISION})
