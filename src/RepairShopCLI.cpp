#include "RepairShopCLI.h"
#include <iomanip>
#include <cmath>
#include <cctype>

sqlite3_int64 readValidID() {
    while (true) {
        sqlite3_int64 id;

        if (!(std::cin >> id)) {
            if (std::cin.eof()) {
                throw std::runtime_error("Input stream closed.");
            }

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            std::cout << "Invalid ID. Enter a positive integer: ";
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

        if (id > 0) {
            return id;
        }

        std::cout << "ID must be positive. Try again: ";
    }
}

sqlite3_int64 parseCostToCents(const std::string& input) {
    if (input.empty()) {
        throw std::invalid_argument("Cost cannot be empty.");
    }

    const size_t dot = input.find('.');

    if (dot != std::string::npos &&
        input.find('.', dot + 1) != std::string::npos) {
        throw std::invalid_argument("Invalid cost format.");
    }

    const std::string euros = input.substr(0, dot);
    const std::string cents =
        dot == std::string::npos ? "" : input.substr(dot + 1);

    if (euros.empty() ||
        (dot != std::string::npos && cents.empty()) ||
        cents.size() > 2) {
        throw std::invalid_argument("Invalid cost format.");
    }

    for (unsigned char ch : euros + cents) {
        if (!std::isdigit(ch)) {
            throw std::invalid_argument("Cost must be a non-negative number.");
        }
    }

    const auto whole = std::stoll(euros);
    const auto fraction = cents.empty() ? 0 :
                          std::stoll(cents) * (cents.size() == 1 ? 10 : 1);

    if (whole > (std::numeric_limits<sqlite3_int64>::max() - fraction) / 100) {
        throw std::out_of_range("Cost is too large.");
    }

    return whole * 100 + fraction;
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
   
    try {
        std::cout << "Enter customer ID: ";
        customerId = readValidID();
        
        std::cout << "Enter device brand: ";
        std::getline(std::cin, brand);

        std::cout << "Enter device model: ";
        std::getline(std::cin, model);

        std::cout << "Enter device serial number: ";
        std::getline(std::cin, serialNumber);

    
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
    try {
        std::cout << "Enter device ID: ";
        deviceId = readValidID();

        std::cout << "Enter problem description: ";
        std::getline(std::cin, problemDescription);

    
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
    try {
        std::cout << "Enter repair ticket ID: ";
        ticketId = readValidID();

    
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
                            RepairTicket updatedTicket = *ticketOpt;
                            updatedTicket.updateStatus(newStatus);

                            if (db.updateRepairTicket(updatedTicket)) {
                                *ticketOpt = updatedTicket;
                                std::cout << "Repair ticket status updated successfully.\n";
                            }
                            else {
                                std::cout << "Failed to update repair ticket status.\n";
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
                            RepairTicket updatedTicket = *ticketOpt;
                            if (costInput.empty()) {
                                updatedTicket.setEstimatedCostCents(std::nullopt);
                            } 
                            else {
                                sqlite3_int64 newCostCents = parseCostToCents(costInput);
                                updatedTicket.setEstimatedCostCents(newCostCents);
                            }
                            if (db.updateRepairTicket(updatedTicket)) {
                                *ticketOpt = updatedTicket;
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
    try {
        std::cout << "Enter customer ID : ";

        customerID = readValidID();

   
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
    try{
        
        std::cout << "Enter Device ID : ";
        DeviceID = readValidID();

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
                std::cout << "Estimated Cost: "
                << std::fixed << std::setprecision(2)
                << *cost / 100.0 << " EUR\n";
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

            if (temp.getEstimatedCostCents().has_value()) {
                double cost = temp.getEstimatedCostCents().value() / 100.0;

                std::cout << "Estimated Cost: "
                << std::fixed << std::setprecision(2)
                << cost << " EUR\n";
        }
            else {
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
        if (std::cin.eof()) {
        std::cout << "\nInput closed. Exiting program.\n";
        return;
    }

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
            if (std::cin.eof()) {
            std::cout << "\nInput closed. Exiting program.\n";
            return;
        }

            std::cin.clear();
            std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

            std::cout << "Invalid input. Please enter a number.\n";
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