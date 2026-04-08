cmake_minimum_required(VERSION 3.16)
project(fail2ban-monitor-banned)
add_executable(
    fail2ban-monitor-banned
    main.cpp
)
target_link_libraries(fail2ban-monitor-banned PRIVATE sqlite3)
install(TARGETS fail2ban-monitor-banned
    RUNTIME)