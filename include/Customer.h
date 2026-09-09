#include <string>
#include <sqlite3.h>
#include "Device.h"



class Customer {
    private:
        sqlite_int64 id;
        std::string name;
        std::string email;
        std::string phoneNumber;
        void setId(sqlite_int64 newId);
        void validate() const;
        void validateName(const std::string& newName) const;
        void validateEmail(const std::string& newEmail) const;
        void validatePhoneNumber(const std::string& newPhoneNumber) const;

    public:
        Customer(const std::string& newName, const std::string& newEmail,
                const std::string& newPhoneNumber);
        
        const std::string& getName() const;
        const std::string& getEmail() const;
        const std::string& getPhoneNumber() const; 
        sqlite_int64 getId() const;  
        void setName(const std::string& newName);     
        void setEmail(const std::string& newEmail);     
        void setPhoneNumber(const std::string& newPhoneNumber);

};