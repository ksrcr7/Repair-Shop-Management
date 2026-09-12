#include "DatabaseRepairShop.h"

void DatabaseRepairShop::createDeviceTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS devices ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "customerId INTEGER NOT NULL,"
                      "brand TEXT NOT NULL,"
                      "model TEXT NOT NULL,"
                      "serialNumber TEXT NOT NULL UNIQUE,"
                      "FOREIGN KEY(customerId) REFERENCES customers(id)"
                      ");";

    
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }                  
}

void DatabaseRepairShop::createCustomerTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS customers ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "name TEXT NOT NULL,"
                      "email TEXT NOT NULL UNIQUE,"
                      "phoneNumber TEXT NOT NULL"
                      ");";

    int rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }                  
}

void DatabaseRepairShop::throwSQLiteError() const {
    throw std::runtime_error(sqlite3_errmsg(db));
}

DatabaseRepairShop::DatabaseRepairShop(const std::string &path) : dbFilePath(path) {
    int rc = sqlite3_open(dbFilePath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::string errorMessage =
        db ? sqlite3_errmsg(db) : "Failed to open database";

    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }

    throw std::runtime_error(errorMessage);
}

    try{
        rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
         if (rc != SQLITE_OK) {
            throwSQLiteError();
        }
    
        createCustomerTable();
        createDeviceTable();
    }

    catch(...){
        sqlite3_close(db);
        db = nullptr;
        throw;
    }

    
    
}

DatabaseRepairShop::~DatabaseRepairShop() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}
