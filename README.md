# Repair Shop Management System

A console-based repair shop management application developed in **C++20**, using **SQLite3** for persistent data storage and **GoogleTest** for automated testing.

The application manages customers, their registered devices, and repair tickets throughout the repair process.

## Features

- **Customer Management:** Register customers and store their contact information.
- **Device Registration:** Register devices and associate them with existing customers.
- **Repair Ticket Management:** Create repair tickets, update their statuses, and manage estimated repair costs.
- **Repair History:** View repair tickets associated with a specific device.
- **Active Repairs:** Display all active repair tickets across the repair shop.
- **Data Validation:** Validate model data, user inputs, and repair ticket status transitions.
- **Persistent Storage:** Store application data in a relational SQLite database.
- **Error Handling:** Handle invalid inputs and database operation errors.

## Technologies

| Technology | Purpose |
|---|---|
| C++20 | Application development |
| SQLite3 | Relational database |
| CMake | Build system |
| GoogleTest | Automated testing |
| Git & GitHub | Version control |

## Project Structure

```text
RepairShopManagementSystem/
├── include/              # Class declarations and headers
├── src/                  # Application implementations
├── tests/                # GoogleTest test cases
├── CMakeLists.txt        # CMake configuration
└── README.md             # Project documentation
```

## Build Instructions

### Prerequisites

- A C++20-compatible compiler
- CMake 3.16 or newer
- SQLite3 development libraries
- Git and an internet connection for downloading GoogleTest during initial configuration

### Build

Clone the repository and navigate to the project directory.

```bash
git clone <https://github.com/ksrcr7/Repair-Shop-Management.git>
cd <Repair-Shop-Management>
```

Configure and build the project:

```bash
cmake -S . -B build
cmake --build build
```

### Run the Application

On Windows, with a single-configuration generator:

```powershell
.\build\RepairShopManagementSystem.exe
```

The executable location may vary depending on the CMake generator and build configuration.

## Command-Line Interface

The application provides an interactive menu:

```text
Repair Shop Management System

1. Add Customer
2. Register Device
3. Create Repair Ticket
4. Update Repair Ticket
5. Show Customer Devices
6. Show Device Repair History
7. Show Active Repair Tickets
0. Exit
```

Users can create customer records, register devices, manage repair tickets, and retrieve repair information through the console.

## Database Design

The application uses three main entities:

- **Customers:** Store customer information.
- **Devices:** Store device information and reference their owners.
- **Repair Tickets:** Store repair details and reference registered devices.

Foreign-key relationships maintain referential integrity between the database tables.

SQLite prepared statements and RAII-based resource management are used for database operations.

## Repair Ticket Workflow

Repair tickets support the following statuses:

- Received
- Diagnosed
- InRepair
- ReadyForPickup
- Completed
- Cancelled

Status transitions are validated by the application.

Estimated repair costs are stored as integer cents to avoid floating-point precision issues in persistent storage.

## Testing

Automated tests are implemented using GoogleTest.

The test suite covers customer, device, repair ticket, and database functionality.

Run the tests with:

```bash
ctest --test-dir build --output-on-failure
```

## Future Improvements

- Additional automated tests for CLI input handling
- Improved reporting and search capabilities
- Further separation of user-interface and business logic

## Project Purpose

This project was developed to practice modern C++ programming, object-oriented design, relational database integration, resource management, automated testing, and software development workflows.