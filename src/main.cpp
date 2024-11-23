//
// Created by eboyh on 11/22/2024.
//

#include <iostream>
#include <libpq-fe.h>
int main() {
    std::cout << "Enter password: " << std::endl;
    std::string pw;
    std::cin >> pw;
    std::cout << std::endl;
    std::string connect = "postgresql://postgres.eyjgqtoxgvibvauumhuw:" + pw + "@aws-0-us-east-1.pooler.supabase.com:6543/postgres";
    PGconn *conn = PQconnectdb(connect.c_str());
    if (PQstatus(conn) == CONNECTION_OK) {
        std::cout << "Connected" << std::endl;
    }

    return 0;
}