#include <gtest/gtest.h>
#include "Device.h"

TEST(DeviceTest, ConstructorAndGetters) {
    Device device("Apple", "iPhone 13", "SN123456789");
    EXPECT_EQ(device.getBrand(), "Apple");
    EXPECT_EQ(device.getModel(), "iPhone 13");
    EXPECT_EQ(device.getSerialNumber(), "SN123456789");
}

TEST(DeviceTest,TrimSuccessfully) {
    Device device("  Apple  ", "  iPhone 13  ", "  SN123456789  ");
    EXPECT_EQ(device.getBrand(), "Apple");
    EXPECT_EQ(device.getModel(), "iPhone 13");
    EXPECT_EQ(device.getSerialNumber(), "SN123456789");
}

TEST(DeviceTest, RejectEmptyBrand) {
    EXPECT_THROW(Device("", "iPhone 13", "SN123456789"), std::invalid_argument);
}

TEST(DeviceTest, RejectEmptyModel) {
    EXPECT_THROW(Device("Apple", "", "SN123456789"), std::invalid_argument);
}

TEST(DeviceTest, RejectEmptySerialNumber) {
    EXPECT_THROW(Device("Apple", "iPhone 13", ""), std::invalid_argument);
}

TEST(DeviceTest, RejectsWhitespaceOnlyBrand) {
    EXPECT_THROW(Device("   ", "iPhone 13", "SN123456789"), std::invalid_argument);
}

TEST(DeviceTest, RejectsWhitespaceOnlyModel) {
    EXPECT_THROW(Device("Apple", "   ", "SN123456789"), std::invalid_argument);
}

TEST(DeviceTest, RejectsWhitespaceOnlySerialNumber) {
    EXPECT_THROW(Device("Apple", "iPhone 13", "   "), std::invalid_argument);
}

TEST(DeviceTest, SetBrandSuccessfully) {
    Device device("Apple", "iPhone 13", "SN123456789");
    device.setBrand("Samsung");
    EXPECT_EQ(device.getBrand(), "Samsung");
}

TEST(DeviceTest, SetModelSuccessfully) {
    Device device("Apple", "iPhone 13", "SN123456789");
    device.setModel("Galaxy S21");
    EXPECT_EQ(device.getModel(), "Galaxy S21");
}

TEST(DeviceTest, SetSerialNumberSuccessfully) {
    Device device("Apple", "iPhone 13", "SN123456789");
    device.setSerialNumber("SN987654321");
    EXPECT_EQ(device.getSerialNumber(), "SN987654321");
}

TEST(DeviceTest, SetBrandRejectsEmpty) {
    Device device("Apple", "iPhone 13", "SN123456789");
    EXPECT_THROW(device.setBrand(""), std::invalid_argument);
    EXPECT_EQ(device.getBrand(), "Apple"); 
}

TEST(DeviceTest, SetModelRejectsEmpty) {
    Device device("Apple", "iPhone 13", "SN123456789");
    EXPECT_THROW(device.setModel(""), std::invalid_argument);
    EXPECT_EQ(device.getModel(), "iPhone 13"); 
}

TEST(DeviceTest, SetSerialNumberRejectsEmpty) {
    Device device("Apple", "iPhone 13", "SN123456789");
    EXPECT_THROW(device.setSerialNumber(""), std::invalid_argument);
    EXPECT_EQ(device.getSerialNumber(), "SN123456789"); 
}

TEST(DeviceTest, SettersTrimInput) {
    Device device("Apple", "iPhone 13", "SN123456789");

    device.setBrand("   Samsung   ");
    device.setModel("   Galaxy S21   ");
    device.setSerialNumber("   SN987654321   ");

    EXPECT_EQ(device.getBrand(), "Samsung");
    EXPECT_EQ(device.getModel(), "Galaxy S21");
    EXPECT_EQ(device.getSerialNumber(), "SN987654321");
}