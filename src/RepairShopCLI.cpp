#include "RepairShopCLI.h"
#include <iomanip>
#include <cmath>

sqlite3_int64 readValidID() {
    while (true) {
        sqlite3_int64 id;
        std::cin >> id;

        if (std::cin.fail() || id <= 0) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Invalid input. Please enter a positive integer for the ID: ";
        } 
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            return id;
        }
    }
}

void RepairShopCLI::addCustomer() {
    std::string name, email, phoneNumber;
    std::cout << "\n--- Add Customer ---\n";

    std::cout << "Enter customer name: ";
    std::getline(std::cin, name);

    std::cout << "Enter customer email: ";
    std::getline(std::cin, email);

    std::cout << "Enter customer phone number: ";
    std::getline(std::cin, phoneNumber);

    try{
        Customer newCustomer(name, email, phoneNumber);
        db.addCustomer(newCustomer);
        std::cout << "Customer added successfully with ID: " << newCustomer.getId() << std::endl;
    } 
    
    catch (const std::exception &e) {
        std::cerr << "Error adding customer: " << e.what() << std::endl;
    }
}

void RepairShopCLI::registerDevice() {
    std::string brand, model, serialNumber;
    sqlite3_int64 customerId;

    std::cout << "\n--- Register Device ---\n";
   
   
    std::cout << "Enter customer ID: ";
    customerId = readValidID();
    
    std::cout << "Enter device brand: ";
    std::getline(std::cin, brand);

    std::cout << "Enter device model: ";
    std::getline(std::cin, model);

    std::cout << "Enter device serial number: ";
    std::getline(std::cin, serialNumber);

    try {
        Device newDevice(brand, model, serialNumber);
        db.addDevice(customerId, newDevice);
        std::cout << "Device registered successfully with ID: " << newDevice.getId() << std::endl;
    } 
    catch (const std::exception &e) {
        std::cerr << "Error registering device: " << e.what() << std::endl;
    }


    
}

void RepairShopCLI::createRepairTicket() {
    sqlite3_int64 deviceId;
    std::string problemDescription;

    std::cout << "\n--- Create Repair Ticket ---\n";
    std::cout << "Enter device ID: ";
    deviceId = readValidID();

    std::cout << "Enter problem description: ";
    std::getline(std::cin, problemDescription);

    try {
        RepairTicket newTicket(deviceId, problemDescription);
        db.addRepairTicket(newTicket);
        std::cout << "Repair ticket created successfully with ID: " << newTicket.getId() << std::endl;
    } 
    catch (const std::exception &e) {
        std::cerr << "Error creating repair ticket: " << e.what() << std::endl;
    }
}

void RepairShopCLI::updateRepairTicket() {
    sqlite3_int64 ticketId;
    std::cout << "\n--- Update Repair Ticket ---\n";
    std::cout << "Enter repair ticket ID: ";
    ticketId = readValidID();

    try {
        auto ticketOpt = db.getRepairTicketById(ticketId);
        if (!ticketOpt.has_value()) {
            std::cout << "Repair ticket with ID " << ticketId << " not found." << std::endl;
            return;
        }

        while(true) {
            std::cout << "Ticket #" << ticketId << std::endl;
            std::cout << "Estimated Cost: ";
            if (ticketOpt->getEstimatedCostCents().has_value()) {
                std::cout << ticketOpt->getEstimatedCostCents().value() / 100.0
                          << " EUR" << std::endl;
            }           
            else {
                std::cout << "Not set" << std::endl;
            }

            std::cout << "\n1. Update Status" << std::endl;
            std::cout << "2. Update Estimated Cost" << std::endl;
            std::cout << "0. Back" << std::endl;
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;
            if (std::cin.fail()) {
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                std::cout << "Invalid input. Please enter a number." << std::endl;
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
                case 1:
                    {
                        std::cout << "Enter new status (Diagnosed, InRepair, ReadyForPickup, Completed, Cancelled): ";
                        std::string newStatusStr;
                        std::getline(std::cin, newStatusStr);
                        try {
                            RepairTicket::Status newStatus = RepairTicket::stringToStatus(newStatusStr);
                            ticketOpt->updateStatus(newStatus);
                            if (db.updateRepairTicket(*ticketOpt)) {
                                std::cout << "Repair ticket status updated successfully." << std::endl;
                            } 
                            else {
                                std::cout << "Failed to update repair ticket status." << std::endl;
                            }
                        } 
                        catch (const std::exception &e) {
                            std::cerr << "Error updating status: " << e.what() << std::endl;
                        }
                    }
                    break;
                
                case 2:
                    {
                        std::cout << "Enter new estimated cost in euros (or leave empty to unset): ";
                        std::string costInput;
                        std::getline(std::cin, costInput);
                        try {
                            if (costInput.empty()) {
                                ticketOpt->setEstimatedCostCents(std::nullopt);
                            } 
                            else {
                                double newCost = std::stod(costInput);
                                sqlite3_int64 newCostCents = static_cast<sqlite3_int64>(std::llround(newCost * 100));
                                ticketOpt->setEstimatedCostCents(newCostCents);

                            }
                            if (db.updateRepairTicket(*ticketOpt)) {
                                std::cout << "Repair ticket estimated cost updated successfully." << std::endl;
                            } 
                            else {
                                std::cout << "Failed to update repair ticket estimated cost." << std::endl;
                            }
                        } 
                        catch (const std::exception &e) {
                            std::cerr << "Error updating estimated cost: " << e.what() << std::endl;
                        }
                    }
                    break;    

                    case 0:
                        return;

                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        break;    
            }



        }
    }
    catch (const std::exception &e) {
        std::cerr << "Error retrieving repair ticket: " << e.what() << std::endl;
    }
}

void RepairShopCLI::showCustomerDevices() {
    sqlite3_int64 customerID;

    std::cout << "\n--- Show Customer Devices ---\n";
    std::cout << "Enter customer ID : ";

    customerID = readValidID();

    try {
        auto result = db.getDevicesByCustomerId(customerID);

        if(result.empty()){
            std::cout << "No devices found for customer ID : " << customerID << std::endl;
            return;
        }

        for(const auto& temp : result){
            std::cout << "Device ID : " << temp.getId() << std::endl;
            std::cout << "Brand : " << temp.getBrand() << std::endl;
            std::cout << "Model : " << temp.getModel() << std::endl;
            std::cout << "Serial Number : " << temp.getSerialNumber() << std::endl;
            std::cout << "--------------------------\n";
        }

    }

    catch (const std::exception& e){
        std::cout << "Error retrieving customer devices: " << e.what() << std::endl;

    }
}

void RepairShopCLI::showDeviceRepairHistory() {
    sqlite3_int64 DeviceID;

    std::cout << "\n---- Show Device RepairTicket History ----\n";
    std::cout << "Enter Device ID : ";
    DeviceID = readValidID();

    try{
        auto result = db.getRepairTicketsByDeviceId(DeviceID);

        if(result.empty()){
            std::cout << "No RepairTickets found for Device ID : " << DeviceID << std::endl;
            return;
        }

        for(const auto& temp : result){
            std::cout << "Ticket ID : " << temp.getId() << std::endl;
            std::cout << "Problem Description : " << temp.getProblemDescription() << std::endl;
            std::cout << "Status : " << temp.statusToString(temp.getStatus()) << std::endl;
            
            auto cost = temp.getEstimatedCostCents();
            if(cost.has_value()){
                std::cout << "Estimated Cost : " << *cost / 100 << std::endl;
            }

            else{
                std::cout << "Estimated Cost : Not available" << std::endl; 
            }
            
        }
    }

    catch (const std::exception& e){
        std::cout << "Error retrieving Device Tickets : " << e.what() << std::endl;
    }
}

void RepairShopCLI::showAllActiveRepairTickets() {

    try{
        auto result = db.getAllActiveRepairTickets();

        if(result.empty()){
            std::cout << "No active repair tickets found\n";
            return;
        }

        for(const RepairTicket& temp : result){
            std::cout << "\n-------------------------\n";
            std::cout << "Ticket ID : " << temp.getId() << std::endl;
            std::cout << "Device ID : " << temp.getDeviceId() << std::endl;
            std::cout << "Problem : " << temp.getProblemDescription() << std::endl;
            std::cout << "Status : " << RepairTicket::statusToString(temp.getStatus()) << std::endl;

            if(temp.getEstimatedCostCents().has_value()) {
                double cost = temp.getEstimatedCostCents().value() / 100;
                std::cout << "Estimated Cost: " << cost << " EUR\n";
            }
            else{
                std::cout << "Estimated Cost: Not set\n";
            }
        }

        std::cout << "-------------------------\n";

    }
    
    catch (const std::exception& e){
        std::cout << "Error showing active repair tickets: "
                  << e.what() << '\n';
        
    }


}

void RepairShopCLI::run()
{
    while (true) {
        int choice;
        std::cout << "Repair Shop Management System" << std::endl;
        std::cout << "1. Add Customer" << std::endl;
        std::cout << "2. Register Device" << std::endl;
        std::cout << "3. Create Repair Ticket" << std::endl;
        std::cout << "4. Update Repair Ticket" << std::endl;
        std::cout << "5. Show Customer Devices" << std::endl;
        std::cout << "6. Show Device Repair History" << std::endl;
        std::cout << "7. Show Active Repair Tickets" << std::endl;
        std::cout << "0. Exit" << std::endl;

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        
        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                registerDevice();
                break;
            case 3:
                createRepairTicket();
                break;
            case 4:
                updateRepairTicket();
                break;
            case 5:
                showCustomerDevices();
                break;
            case 6:
                showDeviceRepairHistory();
                break;  
            case 7:
                showAllActiveRepairTickets();
                break;
            case 0:
                std::cout << "Exiting the program." << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;                      
        }

    }
}