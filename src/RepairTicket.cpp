#include "RepairTicket.h"
#include "StringUtils.h"
#include <stdexcept>

void RepairTicket::setId(sqlite3_int64 newId) {
    
    if (newId <= 0) {
        throw std::invalid_argument("ID must be a positive integer");
    }
    id = newId;
}

void RepairTicket::validate() const
{
    validateDeviceId(deviceId);
    validateProblemDescription(problemDescription);
}

void RepairTicket::validateProblemDescription(const std::string &newProblemDescription) const
{
    if (newProblemDescription.empty())
    {
        throw std::invalid_argument("Problem description cannot be empty");
    }
}

void RepairTicket::validateDeviceId(sqlite3_int64 newDeviceId) const
{
    if (newDeviceId <= 0)
    {
        throw std::invalid_argument("Device ID must be a positive integer");
    }
}

void RepairTicket::validateEstimatedCostCents(std::optional<sqlite3_int64> newEstimatedCostCents) const {
    
    if (newEstimatedCostCents.has_value() && newEstimatedCostCents.value() < 0) {
        throw std::invalid_argument("Estimated cost cannot be negative");
    }
}

bool RepairTicket::isValidStatusTransition(Status newStatus) const {
    
    switch (status) {
        case Status::Received:
            return newStatus == Status::Diagnosed || newStatus == Status::Cancelled;
        case Status::Diagnosed:
            return newStatus == Status::InRepair || newStatus == Status::Cancelled;
        case Status::InRepair:
            return newStatus == Status::ReadyForPickup || newStatus == Status::Cancelled;
        case Status::ReadyForPickup:
            return newStatus == Status::Completed;
        case Status::Completed:
        case Status::Cancelled:
            return false; 
    }
    return false;
    
}

RepairTicket::RepairTicket(sqlite3_int64 newDeviceId, const std::string &newProblemDescription) : 
deviceId(newDeviceId),
problemDescription(trim(newProblemDescription)),
status(Status::Received),
estimatedCostCents(std::nullopt)
{
    validate();
}

sqlite3_int64 RepairTicket::getId() const
{
    return id;
}

sqlite3_int64 RepairTicket::getDeviceId() const
{
    return deviceId;
}

const std::string &RepairTicket::getProblemDescription() const
{
    return problemDescription;
}

RepairTicket::Status RepairTicket::getStatus() const
{
    return status;
}

std::optional<sqlite3_int64> RepairTicket::getEstimatedCostCents() const
{
    return estimatedCostCents;
}

void RepairTicket::setEstimatedCostCents(std::optional<sqlite3_int64> newEstimatedCostCents)
{
    validateEstimatedCostCents(newEstimatedCostCents);
    estimatedCostCents = newEstimatedCostCents;
}

void RepairTicket::updateStatus(Status newStatus)
{
    if (!isValidStatusTransition(newStatus)) {
        throw std::invalid_argument("Invalid status transition");
    } 

    status = newStatus;
}
