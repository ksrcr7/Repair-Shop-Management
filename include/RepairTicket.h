#pragma once

#include <sqlite3.h>
#include <string>
#include <optional>

class RepairTicket{
    friend class DatabaseRepairShop;
    
    public:
        RepairTicket(sqlite3_int64 newDeviceId, const std::string& newProblemDescription);
        enum class Status {Received, Diagnosed, InRepair, ReadyForPickup, Completed, Cancelled};
        sqlite3_int64 getId() const;
        sqlite3_int64 getDeviceId() const;
        const std::string& getProblemDescription() const;
        Status getStatus() const;
        std::optional<sqlite3_int64> getEstimatedCostCents() const;

        void setEstimatedCostCents(std::optional<sqlite3_int64> newEstimatedCostCents);
        void updateStatus(Status newStatus);
        static std::string statusToString(Status status);
        static Status stringToStatus(const std::string& statusStr);
   
   
    private:
        sqlite3_int64 id;
        sqlite3_int64 deviceId;
        std::string problemDescription;
        
        Status status;
        std::optional<sqlite3_int64> estimatedCostCents;
        void setId(sqlite3_int64 newId);
        void validate() const;
        void validateProblemDescription(const std::string& newProblemDescription) const;
        void validateDeviceId(sqlite3_int64 newDeviceId) const;
        void validateEstimatedCostCents(std::optional<sqlite3_int64> newEstimatedCostCents) const;
        bool isValidStatusTransition(Status newStatus) const;
        void setStatus(Status newStatus);

    

};
