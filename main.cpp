#include <iostream>
#include <vector>
#include <string>

class Cycle {
private:
    std::string cycleId;
    std::string brand;
    std::string model;
    double basePricePerDay;
    bool isAvailable;

public:
    Cycle(std::string id, std::string b, std::string m, double price)
        : cycleId(id), brand(b), model(m), basePricePerDay(price), isAvailable(true) {}

    std::string getCycleId() const {
        return cycleId;
    }

    std::string getBrand() const {
        return brand;
    }

    std::string getModel() const {
        return model;
    }

    double calculatePrice(int rentalDays) const {
        return basePricePerDay * rentalDays;
    }

    bool isAvailableCycle() const {
        return isAvailable;
    }

    void rent() {
        isAvailable = false;
    }

    void returnCycle() {
        isAvailable = true;
    }
};

class Customer {
private:
    std::string customerId;
    std::string name;

public:
    Customer(std::string id, std::string n) : customerId(id), name(n) {}

    std::string getCustomerId() const {
        return customerId;
    }

    std::string getName() const {
        return name;
    }
};

class Rental {
private:
    Cycle* cycle;
    Customer* customer;
    int days;

public:
    Rental(Cycle* c, Customer* cust, int d) : cycle(c), customer(cust), days(d) {}

    Cycle* getCycle() const {
        return cycle;
    }

    Customer* getCustomer() const {
        return customer;
    }

    int getDays() const {
        return days;
    }
};

class CycleRentalSystem {
private:
    std::vector<Cycle> cycles;
    std::vector<Customer> customers;
    std::vector<Rental> rentals;

public:
    void addCycle(const Cycle& cycle) {
        cycles.push_back(cycle);
    }

    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    void rentCycle(Cycle& cycle, Customer& customer, int days) {
        if (cycle.isAvailableCycle()) {
            cycle.rent();
            rentals.emplace_back(&cycle, &customer, days);
        } else {
            std::cout << "Cycle is not available for rent.\n";
        }
    }

    void returnCycle(Cycle& cycle) {
        cycle.returnCycle();
        for (auto it = rentals.begin(); it != rentals.end(); ++it) {
            if (it->getCycle() == &cycle) {
                rentals.erase(it);
                return;
            }
        }
        std::cout << "Cycle was not rented.\n";
    }

    void menu() {
        while (true) {
            std::cout << "===== Cycle Rental System =====\n";
            std::cout << "1. Rent a Cycle\n";
            std::cout << "2. Return a Cycle\n";
            std::cout << "3. Exit\n";
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore(); // Consume newline

            if (choice == 1) {
                std::cout << "\n== Rent a Cycle ==\n";
                std::cout << "Enter your name: ";
                std::string customerName;
                std::getline(std::cin, customerName);

                std::cout << "\nAvailable Cycles:\n";
                for (const auto& cycle : cycles) {
                    if (cycle.isAvailableCycle()) {
                        std::cout << cycle.getCycleId() << " - " << cycle.getBrand() << " " << cycle.getModel() << "\n";
                    }
                }

                std::cout << "\nEnter the cycle ID you want to rent: ";
                std::string cycleId;
                std::getline(std::cin, cycleId);

                std::cout << "Enter the number of days for rental: ";
                int rentalDays;
                std::cin >> rentalDays;
                std::cin.ignore(); // Consume newline

                Customer newCustomer("CUS" + std::to_string(customers.size() + 1), customerName);
                addCustomer(newCustomer);

                Cycle* selectedCycle = nullptr;
                for (auto& cycle : cycles) {
                    if (cycle.getCycleId() == cycleId && cycle.isAvailableCycle()) {
                        selectedCycle = &cycle;
                        break;
                    }
                }

                if (selectedCycle) {
                    double totalPrice = selectedCycle->calculatePrice(rentalDays);
                    std::cout << "\n== Rental Information ==\n";
                    std::cout << "Customer ID: " << newCustomer.getCustomerId() << "\n";
                    std::cout << "Customer Name: " << newCustomer.getName() << "\n";
                    std::cout << "Cycle: " << selectedCycle->getBrand() << " " << selectedCycle->getModel() << "\n";
                    std::cout << "Rental Days: " << rentalDays << "\n";
                    std::cout << "Total Price: $" << totalPrice << "\n";

                    std::cout << "\nConfirm rental (Y/N): ";
                    std::string confirm;
                    std::getline(std::cin, confirm);

                    if (confirm == "Y" || confirm == "y") {
                        rentCycle(*selectedCycle, newCustomer, rentalDays);
                        std::cout << "\nCycle rented successfully.\n";
                    } else {
                        std::cout << "\nRental canceled.\n";
                    }
                } else {
                    std::cout << "\nInvalid cycle selection or cycle not available for rent.\n";
                }
            } else if (choice == 2) {
                std::cout << "\n== Return a Cycle ==\n";
                std::cout << "Enter the cycle ID you want to return: ";
                std::string cycleId;
                std::getline(std::cin, cycleId);

                Cycle* cycleToReturn = nullptr;
                for (auto& cycle : cycles) {
                    if (cycle.getCycleId() == cycleId && !cycle.isAvailableCycle()) {
                        cycleToReturn = &cycle;
                        break;
                    }
                }

                if (cycleToReturn) {
                    Customer* customer = nullptr;
                    for (const auto& rental : rentals) {
                        if (rental.getCycle() == cycleToReturn) {
                            customer = rental.getCustomer();
                            break;
                        }
                    }

                    if (customer) {
                        returnCycle(*cycleToReturn);
                        std::cout << "Cycle returned successfully by " << customer->getName() << "\n";
                    } else {
                        std::cout << "Cycle was not rented or rental information is missing.\n";
                    }
                } else {
                    std::cout << "Invalid cycle ID or cycle is not rented.\n";
                }
            } else if (choice == 3) {
                break;
            } else {
                std::cout << "Invalid choice. Please enter a valid option.\n";
            }
        }

        std::cout << "\nThank you for using the Cycle Rental System!\n";
    }
};

int main() {
    CycleRentalSystem rentalSystem;

    Cycle cycle1("C001", "Giant", "Escape 3", 15.0);
    Cycle cycle2("C002", "Trek", "FX 3", 20.0);
    Cycle cycle3("C003", "Specialized", "Sirrus", 25.0);
    rentalSystem.addCycle(cycle1);
    rentalSystem.addCycle(cycle2);
    rentalSystem.addCycle(cycle3);

    rentalSystem.menu();

    return 0;
}
