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

TEST_F(DatabaseRepairShopTest, GetCustomerByIdSuccessfully) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    EXPECT_NO_THROW(db.addCustomer(customer));

    auto retrievedCustomer = db.getCustomerById(customer.getId());
   
    ASSERT_TRUE(retrievedCustomer.has_value());
    EXPECT_EQ (retrievedCustomer->getId(),customer.getId());
    EXPECT_EQ(retrievedCustomer->getEmail(),customer.getEmail());
    EXPECT_EQ(retrievedCustomer->getName(),customer.getName());
    EXPECT_EQ(retrievedCustomer->getPhoneNumber(),customer.getPhoneNumber());
}

TEST_F(DatabaseRepairShopTest, GetCustomerByIdReturnsEmptyForNonExistentCustomer) {
    auto retrievedCustomer = db.getCustomerById(999);
    ASSERT_FALSE(retrievedCustomer.has_value());

}

TEST_F(DatabaseRepairShopTest, GetDevicesByCustomerIdSuccessfully) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    Device device1("Samsung", "Galaxy S21", "SN123456789");
    Device device2("Apple", "iPhone 12", "SN987654321");
    EXPECT_NO_THROW(db.addCustomer(customer));
    EXPECT_NO_THROW(db.addDevice(customer.getId(), device1));
    EXPECT_NO_THROW(db.addDevice(customer.getId(), device2));

    auto devices = db.getDevicesByCustomerId(customer.getId());
    ASSERT_EQ(devices.size(), 2);
    EXPECT_EQ(devices[0].getBrand(), device1.getBrand());
    EXPECT_EQ(devices[0].getModel(), device1.getModel());
    EXPECT_EQ(devices[0].getSerialNumber(), device1.getSerialNumber());
    EXPECT_EQ(devices[1].getBrand(), device2.getBrand());
    EXPECT_EQ(devices[1].getModel(), device2.getModel());
    EXPECT_EQ(devices[1].getSerialNumber(), device2.getSerialNumber());
}

TEST_F(DatabaseRepairShopTest, GetDevicesByCustomerIdReturnsEmptyListWhenCustomerHasNoDevices) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    EXPECT_NO_THROW(db.addCustomer(customer));
    auto devices = db.getDevicesByCustomerId(customer.getId());
    ASSERT_TRUE(devices.empty());
}

TEST_F(DatabaseRepairShopTest, GetDevicesByCustomerIdReturnsEmptyListForNonExistentCustomer) {
    auto devices = db.getDevicesByCustomerId(9999);

    EXPECT_TRUE(devices.empty());
}

TEST_F(DatabaseRepairShopTest, DeleteCustomerByIdSuccessfully) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    EXPECT_NO_THROW(db.addCustomer(customer));
    EXPECT_TRUE(db.deleteCustomerById(customer.getId()));

    auto retrievedCustomer = db.getCustomerById(customer.getId());
    ASSERT_FALSE(retrievedCustomer.has_value());
}

TEST_F(DatabaseRepairShopTest, DeleteCustomerByIdReturnsFalseForNonExistentCustomer) {
    EXPECT_FALSE(db.deleteCustomerById(9999));
}

TEST_F(DatabaseRepairShopTest, CanNotDeleteCustomerWithDevices) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    Device device("Samsung", "Galaxy S21", "SN123456789");
    EXPECT_NO_THROW(db.addCustomer(customer));
    EXPECT_NO_THROW(db.addDevice(customer.getId(), device));
    EXPECT_THROW(db.deleteCustomerById(customer.getId()), std::runtime_error);

    auto retrievedCustomer = db.getCustomerById(customer.getId());
    ASSERT_TRUE(retrievedCustomer.has_value());
    EXPECT_EQ(retrievedCustomer->getId(), customer.getId());
}

TEST_F(DatabaseRepairShopTest, DeleteDeviceByIdSuccessfully) {
    Customer customer("John Doe", "john.doe@gmail.com", "+12365792");
    Device device("Samsung", "Galaxy S21", "SN123456789");
    EXPECT_NO_THROW(db.addCustomer(customer));
    EXPECT_NO_THROW(db.addDevice(customer.getId(), device));
    EXPECT_TRUE(db.deleteDeviceById(device.getId()));
    EXPECT_TRUE(db.getDevicesByCustomerId(customer.getId()).empty());
    EXPECT_TRUE(db.getCustomerById(customer.getId()).has_value());
}

TEST_F(DatabaseRepairShopTest, DeleteDeviceByIdReturnsFalseForNonExistentDevice) {
    EXPECT_FALSE(db.deleteDeviceById(9999));
}
