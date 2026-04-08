#include <algorithm>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <stdio.h>
#include <sqlite3.h>
#include <string>

sqlite3* db;
const char* db_path = "/var/lib/fail2ban/fail2ban.sqlite3";
const char* query_str = "SELECT ip, timeofban, bantime, jail FROM bips ORDER BY jail, (timeofban + bantime)";

const char* dh = "═";
const char* dx = "╬";
const char* dv = "║";

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


int main(int argc, char* argv[])
{
    bool showAll = cmdOptionExists(argv, argv + argc, "-a");
    bool verbose = cmdOptionExists(argv, argv + argc, "-v");

    int opened = sqlite3_open(db_path, &db);
    if (opened)
    {
        printf("Database didn't open: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        if (verbose) printf("Database opened\n");
    }


    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query_str, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error uccured: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        const char* delimiter = "╬══════════════════════╬══════════════════╬══════════════════╬\n";
        time_t now = std::time(0);
        if (verbose) printf("Current time: %s\n", std::ctime(&now));
        int ncols = sqlite3_column_count(stmt);
        bool done = false;
        std::string lastjail;
        while (!done)
        {
            switch (sqlite3_step(stmt))
            {
                case SQLITE_ROW:
                {

                    const char* ip = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    int timeofban = atof(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                    int bantime = atof(reinterpret_cast<const char*>(sqlite3_column_text(stmt,2)));
                    const char* jail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

                    if (lastjail != jail)
                    {
                        printf("%s", delimiter);
                        printf("║ \e[1;32m%20s\e[0;0m ║ %-16s ║ %-16s ║\n", jail, "Initial ban time", "Ban time left");
                        lastjail = jail;
                    }

                    int timeleft = timeofban + bantime - now;

                    if (timeleft < 0)
                    {
                        if (showAll)
                        {
                            printf
                            (
                                "║ %-20s ║ %2d days %02d:%02d:%02d ║ %-16s ║\n", 
                                ip, 
                                bantime / 86400, 
                                bantime % 86400 / 3600, 
                                bantime % 86400 % 3600 / 60, 
                                bantime % 86400 % 3600 % 3600 / 60,
                                "FREE"
                            );
                        }
                    }
                    else
                    {
                        printf
                        (
                            "║ %-20s ║ %2d days %02d:%02d:%02d ║ %2d days %02d:%02d:%02d ║\n", 
                            ip, 
                            bantime / 86400, 
                            bantime % 86400 / 3600, 
                            bantime % 86400 % 3600 / 60, 
                            bantime % 86400 % 3600 % 60,
                            timeleft / 86400,  
                            timeleft % 86400 / 3600,  
                            timeleft % 86400 % 3600 / 60,  
                            timeleft % 86400 % 3600 % 60
                        );
                    }

                    

                    break;
                }

                case SQLITE_DONE:
                {
                    printf("%s", delimiter);
                    if (verbose) printf("Reading done\n");
                    sqlite3_finalize(stmt);
                    done = true;
                    break;
                }
            }
        }
    }

    int closed = sqlite3_close(db);
    if (closed)
    {
        printf("Database didn't close: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        if (verbose) printf("Database closed\n");
    }

    return 0;
}