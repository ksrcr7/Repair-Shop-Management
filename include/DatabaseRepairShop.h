#pragma once

#include <sqlite3.h>
#include <optional>
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

        void addCustomer(Customer& customer);
        void addDevice(sqlite3_int64 customerId, Device& device);
        std::optional<Customer> getCustomerById(sqlite3_int64 customerId);

};