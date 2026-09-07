#include <string>
#include <iostream>
#include <sqlite3.h>




class Device {
    private:
        sqlite3_int64 id;
        std::string brand;
        std::string model;
        std::string serialNumber;
        void setId(sqlite3_int64 newId);
        void validate() const;
        void validateBrand(const std::string& newBrand) const;
        void validateModel(const std::string& newModel) const;
        void validateSerialNumber(const std::string& newSerialNumber) const;

    public:
        Device(const std::string& newBrand, const std::string& newModel,
                const std::string& newSerialNumber);
        
        const std::string& getBrand() const;
        const std::string& getModel() const;
        const std::string& getSerialNumber() const; 
        sqlite3_int64 getId() const;  
        void setBrand(const std::string& newBrand);     
        void setModel(const std::string& newModel);     
        void setSerialNumber(const std::string& newSerialNumber);     



};
