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
