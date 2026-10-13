#pragma once

#include <sqlite3.h>
#include <optional>
#include <vector>
#include "Device.h"
#include "Customer.h"
#include "RepairTicket.h"



class DatabaseRepairShop {

    private:
        sqlite3* db = nullptr;
        std::string dbFilePath;
        void createDeviceTable();
        void createCustomerTable();
        void createRepairTicketTable();
        [[noreturn]] void throwSQLiteError() const;

    public:
        DatabaseRepairShop(const std::string& path);
        ~DatabaseRepairShop();
        DatabaseRepairShop(const DatabaseRepairShop&) = delete;
        DatabaseRepairShop& operator=(const DatabaseRepairShop&) = delete;

        void addCustomer(Customer& customer);
        void addDevice(sqlite3_int64 customerId, Device& device);
        std::optional<Customer> getCustomerById(sqlite3_int64 customerId);
        std::vector <Device> getDevicesByCustomerId(sqlite3_int64 id);
        bool deleteCustomerById(sqlite3_int64 customerId);
        bool deleteDeviceById(sqlite3_int64 deviceId);
        bool updateCustomer(const Customer& customer);
        bool updateDevice(const Device& device);

        void addRepairTicket(RepairTicket& ticket);
        std::vector<RepairTicket> getRepairTicketsByDeviceId(sqlite3_int64 deviceId);
        bool updateRepairTicket(const RepairTicket& ticket);


};