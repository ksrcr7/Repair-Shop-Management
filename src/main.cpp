#include <iostream>
#include "RepairShopCLI.h"
#include "DatabaseRepairShop.h"

int main() {
    try {
        DatabaseRepairShop database("repair_shop.db");
        RepairShopCLI cli(database);

        cli.run();
    } 
    
    catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
}