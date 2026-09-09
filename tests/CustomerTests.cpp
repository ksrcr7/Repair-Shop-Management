#include "Customer.h"
#include <gtest/gtest.h>

TEST(CustomerTest, ConstructorInitializesFields) {
    Customer customer("Kasra","ksrcr7@gmail.com","+491782031196");
    EXPECT_EQ(customer.getName(), "Kasra");
    EXPECT_EQ(customer.getEmail(), "ksrcr7@gmail.com");
    EXPECT_EQ(customer.getPhoneNumber(), "+491782031196");
}

TEST(CustomerTest, SetterUpdateName) {
    Customer customer("Kasra","ksrcr7@gmail.com","+491782031196");
    customer.setName("John Doe");
    EXPECT_EQ(customer.getName(), "John Doe");
}

TEST(CustomerTest, SetterUpdateEmail) {
    Customer customer("Kasra","ksrcr7@gmail.com","+491782031196");
    customer.setEmail("john.doe@gmail.com");
    EXPECT_EQ(customer.getEmail(), "john.doe@gmail.com");
}

TEST(CustomerTest, SetterUpdatePhoneNumber) {
    Customer customer("Kasra","ksrcr7@gmail.com","+491782031196");
    customer.setPhoneNumber("+1234567890");
    EXPECT_EQ(customer.getPhoneNumber(), "+1234567890");
}

TEST(CustomerTest, EnterEmptyNameThrowsException) {
    EXPECT_THROW(Customer("", "ksrcr7@gmail.com", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterWhitespaceNameThrowsException) {
    EXPECT_THROW(Customer("   ", "ksrcr7@gmail.com", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterEmptyEmailThrowsException) {
    EXPECT_THROW(Customer("Kasra", "", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterWhitespaceEmailThrowsException) {
    EXPECT_THROW(Customer("Kasra", "   ", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterEmptyPhoneNumberThrowsException) {
    EXPECT_THROW(Customer("Kasra", "ksrcr7@gmail.com", ""), std::invalid_argument);
}

TEST(CustomerTest, EnterWhitespacePhoneNumberThrowsException) {
    EXPECT_THROW(Customer("Kasra", "ksrcr7@gmail.com", "      "), std::invalid_argument);
}

TEST(CustomerTest, EnterInvalidEmailWithoutAtSymbolThrowsException) {
    EXPECT_THROW(Customer("Kasra", "kasra.gmail.com", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterInvalidEmailLastCharacterAtSymbolThrowsException) {
    EXPECT_THROW(Customer("Kasra", "kasra@", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterInvalidEmailFirstCharacterAtSymbolThrowsException) {
    EXPECT_THROW(Customer("Kasra", "@gmail.com", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterInvalidEmailMultipleAtSymbolsThrowsException) {
    EXPECT_THROW(Customer("Kasra", "kasra@gmail@.com", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterInvalidEmailNoDotAfterAtSymbolThrowsException) {
    EXPECT_THROW(Customer("Kasra", "kasra@gmailcom", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterInvalidEmailDotLastCharacterThrowsException) {
    EXPECT_THROW(Customer("Kasra", "kasra@gmail.", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterInvalidEmailDotFirstCharacterAfterAtSymbolThrowsException) {
    EXPECT_THROW(Customer("Kasra", "kasra@.gmail", "+491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterPhoneNumberWithoutPlusSignAsFirstCharacter){
    EXPECT_THROW(Customer("Kasra", "kasra@gmail.com", "491782031196"), std::invalid_argument);
}

TEST(CustomerTest, EnterPhoneNumberOutOfRange){
    EXPECT_THROW(Customer("Kasra", "kasra@gmail.com", "+495221"), std::invalid_argument);
    EXPECT_THROW(Customer("Kasra", "kasra@gmail.com", "+4952212395745622"), std::invalid_argument);
}

TEST(CustomerTest, EnterPhoneNumberNotAllCharactersDigit){
    EXPECT_THROW(Customer("Kasra", "kasra@gmail.com", "+4523336554A@"), std::invalid_argument);
}

TEST(CustomerTest, ConstructorTrimsInput) {
    Customer customer(
        "   Kasra   ",
        "   ksrcr7@gmail.com   ",
        "   +491782031196   "
    );

    EXPECT_EQ(customer.getName(), "Kasra");
    EXPECT_EQ(customer.getEmail(), "ksrcr7@gmail.com");
    EXPECT_EQ(customer.getPhoneNumber(), "+491782031196");
}

TEST(CustomerTest, SettersTrimInput) {
    Customer customer(
        "Kasra",
        "ksrcr7@gmail.com",
        "+491782031196"
    );

    customer.setName("   John Doe   ");
    customer.setEmail("   john.doe@gmail.com   ");
    customer.setPhoneNumber("   +1234567890   ");

    EXPECT_EQ(customer.getName(), "John Doe");
    EXPECT_EQ(customer.getEmail(), "john.doe@gmail.com");
    EXPECT_EQ(customer.getPhoneNumber(), "+1234567890");
}

TEST(CustomerTest, InvalidNameDoesNotChangeExistingName) {
    Customer customer(
        "Kasra",
        "ksrcr7@gmail.com",
        "+491782031196"
    );

    EXPECT_THROW(
        customer.setName("     "),
        std::invalid_argument
    );

    EXPECT_EQ(customer.getName(), "Kasra");
}

TEST(CustomerTest, InvalidEmailDoesNotChangeExistingEmail) {
    Customer customer(
        "Kasra",
        "ksrcr7@gmail.com",
        "+491782031196"
    );

    EXPECT_THROW(
        customer.setEmail("invalid-email"),
        std::invalid_argument
    );

    EXPECT_EQ(customer.getEmail(), "ksrcr7@gmail.com");
}

TEST(CustomerTest, InvalidPhoneNumberDoesNotChangeExistingPhoneNumber) {
    Customer customer(
        "Kasra",
        "ksrcr7@gmail.com",
        "+491782031196"
    );

    EXPECT_THROW(
        customer.setPhoneNumber("491782031196"),
        std::invalid_argument
    );

    EXPECT_EQ(customer.getPhoneNumber(), "+491782031196");
}