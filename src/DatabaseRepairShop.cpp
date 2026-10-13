#include "DatabaseRepairShop.h"
#include "SQLiteUtils.h"

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

void DatabaseRepairShop::createRepairTicketTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS repairTickets ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "deviceId INTEGER NOT NULL,"
                      "problemDescription TEXT NOT NULL,"
                      "status TEXT NOT NULL,"
                      "estimatedCostCents INTEGER,"
                      "FOREIGN KEY(deviceId) REFERENCES devices(id)"
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
        createRepairTicketTable();
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

void DatabaseRepairShop::addCustomer(Customer &customer) {
    const char* sql = "INSERT INTO customers (name,email,phonenumber) VALUES (?,?,?);";
    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 1, customer.getName().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 2, customer.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 3, customer.getPhoneNumber().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    sqlite_int64 newId = sqlite3_last_insert_rowid(db);
    customer.setId(newId);

}



void DatabaseRepairShop::addDevice(sqlite_int64 customerId, Device &device) {
    const char* sql = "INSERT INTO devices (customerId, brand, model, serialNumber) VALUES (?,?,?,?);";
    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 1, customerId);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 2, device.getBrand().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 3, device.getModel().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 4, device.getSerialNumber().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    sqlite_int64 newId = sqlite3_last_insert_rowid(db);
    device.setId(newId);
}

std::optional<Customer> DatabaseRepairShop::getCustomerById(sqlite3_int64 customerId) {

    const char* sql = "SELECT id, name, email, phoneNumber FROM customers WHERE id = ?;";
    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 1, customerId);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc == SQLITE_ROW) {
        sqlite3_int64 id = sqlite3_column_int64(stmt.get(), 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 2));
        const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 3));

        Customer customer(name, email, phoneNumber);
        customer.setId(id);
        return customer;
    } 
    
    else if (rc == SQLITE_DONE) {
        return std::nullopt; 
    } 
    
    throwSQLiteError();
}

std::vector<Device> DatabaseRepairShop::getDevicesByCustomerId(sqlite3_int64 id) {

    std::vector<Device> result;
    const char* sql = "SELECT id, brand, model, serialNumber FROM devices WHERE customerId = ?;";
    sqlite3_stmt* rawStmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 1, id);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    while ((rc = sqlite3_step(stmt.get())) == SQLITE_ROW) {
        sqlite3_int64 deviceId = sqlite3_column_int64(stmt.get(), 0);
        const char* brand = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        const char* model = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 2));
        const char* serialNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 3));

        Device device(brand, model, serialNumber);
        device.setId(deviceId);
        result.push_back(device);
    }

    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    return result;
    
}

bool DatabaseRepairShop::deleteCustomerById(sqlite3_int64 customerId) {
    const char* sql = "DELETE FROM customers WHERE id = ?;";
    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 1, customerId);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseRepairShop::deleteDeviceById(sqlite3_int64 deviceId) {
    const char* sql = "DELETE FROM devices WHERE id = ?;";
    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 1, deviceId);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    return sqlite3_changes(db) > 0;

}

bool DatabaseRepairShop::updateCustomer(const Customer &customer) {

    const char* sql = "UPDATE customers SET name = ?, email = ?, phoneNumber = ? WHERE id = ?;";
    sqlite3_stmt* rawStmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 1, customer.getName().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 2, customer.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 3, customer.getPhoneNumber().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 4, customer.getId());
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    return sqlite3_changes(db) > 0;
}

bool DatabaseRepairShop::updateDevice(const Device &device) {
    const char* sql = "UPDATE devices SET brand = ?, model = ?, serialNumber = ? WHERE id = ?;";
    sqlite3_stmt* rawStmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 1, device.getBrand().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 2, device.getModel().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 3, device.getSerialNumber().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 4, device.getId());
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    return sqlite3_changes(db) > 0;

}

void DatabaseRepairShop::addRepairTicket(RepairTicket &ticket) {
    const char* sql = "INSERT INTO repairTickets (deviceId, problemDescription, status, estimatedCostCents) VALUES (?,?,?,?);";
    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 1, ticket.getDeviceId());
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 2, ticket.getProblemDescription().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 3, RepairTicket::statusToString(ticket.getStatus()).c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    auto estimatedCost = ticket.getEstimatedCostCents();
    if (estimatedCost) {
        rc = sqlite3_bind_int64(stmt.get(), 4, *estimatedCost);
    } 
    else {
        rc = sqlite3_bind_null(stmt.get(), 4);
    }

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    sqlite3_int64 newId = sqlite3_last_insert_rowid(db);
    ticket.setId(newId);


}

std::vector<RepairTicket> DatabaseRepairShop::getRepairTicketsByDeviceId(sqlite3_int64 deviceId) {

    std::vector<RepairTicket> result;
    const char* sql = "SELECT id, problemDescription, status, estimatedCostCents FROM repairTickets WHERE deviceId = ? ORDER BY id;";

    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 1, deviceId);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    while ((rc = sqlite3_step(stmt.get())) == SQLITE_ROW) {
        sqlite3_int64 ticketId = sqlite3_column_int64(stmt.get(), 0);
        const char* problemDescription = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        const char* statusStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 2));
        std::optional<sqlite3_int64> estimatedCostCents;

        if (sqlite3_column_type(stmt.get(), 3) != SQLITE_NULL) {
            estimatedCostCents = sqlite3_column_int64(stmt.get(), 3);
        }

        RepairTicket::Status status = RepairTicket::stringToStatus(statusStr);
        RepairTicket ticket(deviceId, problemDescription);
        ticket.setId(ticketId);
        ticket.setStatus(status);
        ticket.setEstimatedCostCents(estimatedCostCents);

        result.push_back(ticket);


    }

    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    return result;
}

bool DatabaseRepairShop::updateRepairTicket(const RepairTicket &ticket) {
    const char* sql = "UPDATE repairTickets SET problemDescription = ?, status = ?, estimatedCostCents = ? WHERE id = ?;";

    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &rawStmt, nullptr);
    UniqueStatementPtr stmt(rawStmt);

    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 1, ticket.getProblemDescription().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_text(stmt.get(), 2, RepairTicket::statusToString(ticket.getStatus()).c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    auto estimatedCost = ticket.getEstimatedCostCents();
    if (estimatedCost) {
        rc = sqlite3_bind_int64(stmt.get(), 3, *estimatedCost);
    } 
    else {
        rc = sqlite3_bind_null(stmt.get(), 3);
    }
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_bind_int64(stmt.get(), 4, ticket.getId());
    if (rc != SQLITE_OK) {
        throwSQLiteError();
    }

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE) {
        throwSQLiteError();
    }

    return sqlite3_changes(db) > 0;
}
