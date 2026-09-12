#include <sqlite3.h>
#include "Device.h"
#include "Customer.h"



class DatabaseRepairShop {

    private:
        sqlite3* db = nullptr;
        std::string dbFilePath;
        void createDeviceTable();
        void createCustomerTable();
        [[noreturn]] void throwSQLiteError() const;

    public:
        DatabaseRepairShop(const std::string& path);
        ~DatabaseRepairShop();
        DatabaseRepairShop(const DatabaseRepairShop&) = delete;
        DatabaseRepairShop& operator=(const DatabaseRepairShop&) = delete;

};