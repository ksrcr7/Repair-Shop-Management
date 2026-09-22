#include <gtest/gtest.h>
#include "DatabaseRepairShop.h"

class DatabaseRepairShopTest : public ::testing::Test {
protected:
    DatabaseRepairShop db;
    DatabaseRepairShopTest() : db(":memory:") {}

};

TEST_F(DatabaseRepairShopTest, Initialization) {
    EXPECT_NO_THROW(DatabaseRepairShop db(":memory:"));
}   

TEST_F(DatabaseRepairShopTest, AddCustomerSuccessfully) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    EXPECT_NO_THROW(db.addCustomer(customer));
    EXPECT_GT(customer.getId(), 0);
}

TEST_F(DatabaseRepairShopTest, UniqueEmailConstraint) {
    Customer customer1("John Doe", "john.doe@gmail.com", "+12365792");
    Customer customer2("Jane Smith", "john.doe@gmail.com", "+12365793");
    EXPECT_NO_THROW(db.addCustomer(customer1));
    EXPECT_THROW(db.addCustomer(customer2), std::runtime_error);
}

TEST_F(DatabaseRepairShopTest, AddDeviceSuccessfully) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    Device device("Samsung", "Galaxy S21", "SN123456789");
    EXPECT_NO_THROW(db.addCustomer(customer));
    EXPECT_NO_THROW(db.addDevice(customer.getId(), device));
    EXPECT_GT(device.getId(), 0);
}

TEST_F(DatabaseRepairShopTest, AddDeviceToNonExistentCustomer) {
    Device device("Samsung", "Galaxy S21", "SN123456789");
    EXPECT_THROW(db.addDevice(9999, device), std::runtime_error);
}

TEST_F(DatabaseRepairShopTest, UniqueSerialNumberConstraint) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    Device device1("Samsung", "Galaxy S21", "SN123456789");
    Device device2("Apple", "iPhone 12", "SN123456789");
    EXPECT_NO_THROW(db.addCustomer(customer));
    EXPECT_NO_THROW(db.addDevice(customer.getId(), device1));
    EXPECT_THROW(db.addDevice(customer.getId(), device2), std::runtime_error);
}
