#include "Customer.h"
#include "StringUtils.h"
#include <stdexcept>
#include <cctype>

void Customer::setId(sqlite_int64 newId) {
    if(newId <= 0) {
        throw std::invalid_argument("ID must be a positive integer.");
    }
    id = newId;
}

void Customer::validate() const {
    validateName(name);
    validateEmail(email);
    validatePhoneNumber(phoneNumber);
}

void Customer::validateName(const std::string &newName) const {
    if (newName.empty()) {
        throw std::invalid_argument("Name cannot be empty.");
    }
}

void Customer::validateEmail(const std::string &newEmail) const {
    if (newEmail.empty()) {
        throw std::invalid_argument("Email cannot be empty.");
    }

    size_t atPos = newEmail.find('@');
    if (atPos == std::string::npos || atPos == 0 || atPos == newEmail.length() - 1) {
        throw std::invalid_argument("Email must contain a valid '@' character.");
    }

    if(newEmail.find('@', atPos + 1) != std::string::npos) {
        throw std::invalid_argument("Email cannot contain multiple '@' characters.");
    }

    size_t dotPos = newEmail.find('.', atPos+1);
    if (dotPos == std::string::npos || dotPos == newEmail.length() - 1) {
        throw std::invalid_argument("Email must contain a valid domain.");
    }

    if(dotPos == atPos + 1) {
        throw std::invalid_argument("@ and '.' cannot be adjacent in the email.");
    }
    
}

void Customer::validatePhoneNumber(const std::string &newPhoneNumber) const {
    if (newPhoneNumber.empty()) {
        throw std::invalid_argument("Phone number cannot be empty.");
    }

    if(newPhoneNumber[0] != '+') {
        throw std::invalid_argument("Phone number must start with a '+' sign.");
    }

    if(newPhoneNumber.length() < 8 || newPhoneNumber.length() > 16) {
        throw std::invalid_argument("Phone number must be between 7 and 15 digits long.");

    }

    for(size_t i = 1; i < newPhoneNumber.length(); ++i) {
        if(!isdigit(static_cast<unsigned char>(newPhoneNumber[i]))) {
            throw std::invalid_argument("Phone number can only contain digits after the '+' sign.");
        }
    }

}

Customer::Customer(const std::string &newName, const std::string &newEmail, const std::string &newPhoneNumber)
    : name(trim(newName)), email(trim(newEmail)), phoneNumber(trim(newPhoneNumber)) {
    validate();
}

const std::string &Customer::getName() const
{
    return name;
}

const std::string &Customer::getEmail() const
{
    return email;
}

const std::string &Customer::getPhoneNumber() const
{
    return phoneNumber;
}

sqlite_int64 Customer::getId() const
{
    return id;
}

void Customer::setName(const std::string &newName) {
    std::string trimmedName = trim(newName);
    validateName(trimmedName);
    name = trimmedName;
}

void Customer::setEmail(const std::string &newEmail) {
    std::string trimmedEmail = trim(newEmail);
    validateEmail(trimmedEmail);
    email = trimmedEmail;
}

void Customer::setPhoneNumber(const std::string &newPhoneNumber) {
    std::string trimmedPhoneNumber = trim(newPhoneNumber);
    validatePhoneNumber(trimmedPhoneNumber);
    phoneNumber = trimmedPhoneNumber;
}
