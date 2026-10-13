#include <gtest/gtest.h>
#include "RepairTicket.h"

TEST(RepairTicketTests, ConstructorTest) {
    RepairTicket ticket(12, "Screen not working");
    EXPECT_EQ(ticket.getDeviceId(), 12);
    EXPECT_EQ(ticket.getProblemDescription(), "Screen not working");
    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::Received);
    EXPECT_FALSE(ticket.getEstimatedCostCents().has_value());
};

TEST(RepairTicketTests, InvalidDeviceIdTest) {
    EXPECT_THROW(RepairTicket ticket(-1, "Screen not working"), std::invalid_argument);
};

TEST(RepairTicketTests, EmptyProblemDescriptionTest) {
    EXPECT_THROW(RepairTicket ticket(12, ""), std::invalid_argument);
};

TEST(RepairTicketTests, TrimProblemDescriptionTest) {
    RepairTicket ticket(12, "   Screen not working   ");
    EXPECT_EQ(ticket.getProblemDescription(), "Screen not working");
};

TEST(RepairTicketTests, WhitespaceOnlyProblemDescriptionTest) {
    EXPECT_THROW(RepairTicket ticket(12, "   "), std::invalid_argument);
};

TEST(RepairTicketTests, SetEstimatedCostCentsTest) {
    RepairTicket ticket(12, "Screen not working");
    ticket.setEstimatedCostCents(5000);
    EXPECT_TRUE(ticket.getEstimatedCostCents().has_value());
    EXPECT_EQ(ticket.getEstimatedCostCents().value(), 5000);
};

TEST(RepairTicketTests, SetNegativeEstimatedCostCentsTest) {
    RepairTicket ticket(12, "Screen not working");
    EXPECT_THROW(ticket.setEstimatedCostCents(-5000), std::invalid_argument);
};

TEST(RepairTicketTests, SetEstimatedCostCentsToZeroTest) {
    RepairTicket ticket(12, "Screen not working");
    ticket.setEstimatedCostCents(0);
    EXPECT_TRUE(ticket.getEstimatedCostCents().has_value());
    EXPECT_EQ(ticket.getEstimatedCostCents().value(), 0);
};

TEST(RepairTicketTests, SetEstimatedCostCentsToNulloptTest) {
    RepairTicket ticket(12, "Screen not working");

    ticket.setEstimatedCostCents(5000);
    ASSERT_TRUE(ticket.getEstimatedCostCents().has_value());

    ticket.setEstimatedCostCents(std::nullopt);

    EXPECT_FALSE(ticket.getEstimatedCostCents().has_value());
};

TEST(RepairTicketTests, UpdateStatusTest) {
    RepairTicket ticket(12, "Screen not working");
    ticket.updateStatus(RepairTicket::Status::Diagnosed);
    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::Diagnosed);

    ticket.updateStatus(RepairTicket::Status::InRepair);
    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::InRepair);

    ticket.updateStatus(RepairTicket::Status::ReadyForPickup);
    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::ReadyForPickup);

    ticket.updateStatus(RepairTicket::Status::Completed);
    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::Completed);
};

TEST(RepairTicketTests, InvalidStatusTransitionTest) {
    RepairTicket ticket(12, "Screen not working");
    EXPECT_THROW(ticket.updateStatus(RepairTicket::Status::InRepair), std::invalid_argument);
    ticket.updateStatus(RepairTicket::Status::Diagnosed);
    EXPECT_THROW(ticket.updateStatus(RepairTicket::Status::ReadyForPickup), std::invalid_argument);
    ticket.updateStatus(RepairTicket::Status::InRepair);
    EXPECT_THROW(ticket.updateStatus(RepairTicket::Status::Completed), std::invalid_argument);
    ticket.updateStatus(RepairTicket::Status::ReadyForPickup);
    EXPECT_THROW(ticket.updateStatus(RepairTicket::Status::Diagnosed), std::invalid_argument);
};

TEST(RepairTicketTests, CancelledStatusTransitionTest) {
    RepairTicket ticket(12, "Screen not working");
    ticket.updateStatus(RepairTicket::Status::Cancelled);
    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::Cancelled);

    RepairTicket ticket2(12, "Screen not working");
    ticket2.updateStatus(RepairTicket::Status::Diagnosed);
    ticket2.updateStatus(RepairTicket::Status::Cancelled);
    EXPECT_EQ(ticket2.getStatus(), RepairTicket::Status::Cancelled);

    RepairTicket ticket3(12, "Screen not working");
    ticket3.updateStatus(RepairTicket::Status::Diagnosed);
    ticket3.updateStatus(RepairTicket::Status::InRepair);
    ticket3.updateStatus(RepairTicket::Status::Cancelled);
    EXPECT_EQ(ticket3.getStatus(), RepairTicket::Status::Cancelled);
};

TEST(RepairTicketTests, CannotTransitionFromCancelledStatusTest) {
    RepairTicket ticket(12, "Screen not working");

    ticket.updateStatus(RepairTicket::Status::Cancelled);

    EXPECT_THROW(
        ticket.updateStatus(RepairTicket::Status::Diagnosed),
        std::invalid_argument
    );

    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::Cancelled);
};

TEST(RepairTicketTests, CannotTransitionFromCompletedStatus) {
    RepairTicket ticket(12, "Screen not working");

    ticket.updateStatus(RepairTicket::Status::Diagnosed);
    ticket.updateStatus(RepairTicket::Status::InRepair);
    ticket.updateStatus(RepairTicket::Status::ReadyForPickup);
    ticket.updateStatus(RepairTicket::Status::Completed);

    EXPECT_THROW(
        ticket.updateStatus(RepairTicket::Status::Cancelled),
        std::invalid_argument
    );

    EXPECT_EQ(ticket.getStatus(), RepairTicket::Status::Completed);
}

TEST(RepairTicketTests, StringToStatusRejectsUnknownStatus) {
    EXPECT_THROW(
        RepairTicket::stringToStatus("SomethingWrong"),
        std::invalid_argument
    );
}