#pragma once
#include <sqlite3.h>
#include <memory>

struct StatementDeleter {
    void operator()(sqlite3_stmt* stmt) const {
        if (stmt != nullptr) {
            sqlite3_finalize(stmt);
        }
    }
};

using UniqueStatementPtr = std::unique_ptr<sqlite3_stmt, StatementDeleter>;