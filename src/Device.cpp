#include "Device.h"
#include "StringUtils.h"
#include <stdexcept>

void Device::setId(sqlite3_int64 newId) {
    if (newId <= 0) {
        throw std::invalid_argument("ID must be a positive integer.");
    }
    id = newId;

}

void Device::validateBrand(const std::string& newBrand) const {
    if (newBrand.empty()) {
        throw std::invalid_argument("Brand cannot be empty.");
    }
}

void Device::validateModel(const std::string &newModel) const{
    if (newModel.empty()) {
        throw std::invalid_argument("Model cannot be empty.");
    }
}

void Device::validateSerialNumber(const std::string &newSerialNumber) const {
    if (newSerialNumber.empty()) {
        throw std::invalid_argument("Serial number cannot be empty.");
    }

}

Device::Device(const std::string &newBrand, const std::string &newModel,
    const std::string &newSerialNumber)
    : brand(trim(newBrand)), model(trim(newModel)), serialNumber(trim(newSerialNumber)) {
     
    validate();
}

const std::string &Device::getBrand() const {
    return brand;
    
}

const std::string &Device::getModel() const {
    return model;
    
}

const std::string &Device::getSerialNumber() const {
    return serialNumber;
    
}

void Device::setBrand(const std::string &newBrand) {
    std::string trimmedBrand = trim(newBrand);
    validateBrand(trimmedBrand);
    brand = trimmedBrand;
}

void Device::setModel(const std::string &newModel) {
    std::string trimmedModel = trim(newModel);
    validateModel(trimmedModel);
    model = trimmedModel;
}

void Device::setSerialNumber(const std::string &newSerialNumber) {
    std::string trimmedSerialNumber = trim(newSerialNumber);
    validateSerialNumber(trimmedSerialNumber);
    serialNumber = trimmedSerialNumber;
}

sqlite3_int64 Device::getId() const {
    return id;
}

void Device::validate() const {
    validateBrand(brand);
    validateModel(model);
    validateSerialNumber(serialNumber);
}
