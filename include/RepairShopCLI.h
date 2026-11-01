#pragma once

#include <iostream>
#include <string>
#include "DatabaseRepairShop.h"





class RepairShopCLI {
private:
    DatabaseRepairShop &db;
    void addCustomer();
    void registerDevice();
    void createRepairTicket();
    void updateRepairTicket();
    void showCustomerDevices();
    void showDeviceRepairHistory();
    void showAllActiveRepairTickets();
    
public:
    RepairShopCLI(DatabaseRepairShop &database) : db(database) {}

    void run();
    
};


