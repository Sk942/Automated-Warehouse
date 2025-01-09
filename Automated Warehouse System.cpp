#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class Part {
public:
    int partID;
    std::string name;
    int quantity;
    int reorderLevel;

    // Default constructor
    Part() : partID(0), name(""), quantity(0), reorderLevel(0) {}

    // Parameterized constructor
    Part(int id, std::string n, int q, int r) : partID(id), name(n), quantity(q), reorderLevel(r) {}

    void updateQuantity(int q) {
        quantity += q;
    }

    bool needsReordering() {
        return quantity < reorderLevel;
    }
};

class Order {
public:
    int partID;
    int quantity;
    std::string status;

    Order(int id, int q) : partID(id), quantity(q), status("Pending") {}
};

class Warehouse {
private:
    std::unordered_map<int, Part> parts;

public:
    void addPart(const Part& part) {
        parts[part.partID] = part;
    }

    void updateQuantity(int partID, int quantity) {
        if (parts.find(partID) != parts.end()) {
            parts[partID].updateQuantity(quantity);
        }
    }

    std::vector<Order> checkReorderLevels() {
        std::vector<Order> orders;
        for (auto& pair : parts) {
            if (pair.second.needsReordering()) {
                orders.emplace_back(pair.second.partID, pair.second.reorderLevel - pair.second.quantity);
            }
        }
        return orders;
    }

    std::unordered_map<int, int> getInventoryStatus() {
        std::unordered_map<int, int> status;
        for (auto& pair : parts) {
            status[pair.first] = pair.second.quantity;
        }
        return status;
    }
};

class Job {
public:
    int jobID;
    std::unordered_map<int, int> requiredParts;

    Job(int id) : jobID(id) {}

    void addRequiredPart(int partID, int quantity) {
        requiredParts[partID] = quantity;
    }

    void allocateParts(Warehouse& warehouse) {
        for (auto& pair : requiredParts) {
            if (warehouse.getInventoryStatus().find(pair.first) != warehouse.getInventoryStatus().end() &&
                warehouse.getInventoryStatus()[pair.first] >= pair.second) {
                warehouse.updateQuantity(pair.first, -pair.second);
                requiredParts[pair.first] = 0;  // Mark as allocated
            }
        }
    }

    std::unordered_map<int, int> getJobStatus() {
        return requiredParts;
    }
};

int main() {
    Warehouse warehouse;
    int choice;

    while (true) {
        std::cout << "1. Add Part\n2. Update Quantity\n3. Check Reorder Levels\n4. Allocate Parts to Job\n5. Exit\nEnter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            int id, quantity, reorderLevel;
            std::string name;
            std::cout << "Enter Part ID: ";
            std::cin >> id;
            std::cout << "Enter Part Name: ";
            std::cin >> name;
            std::cout << "Enter Quantity: ";
            std::cin >> quantity;
            std::cout << "Enter Reorder Level: ";
            std::cin >> reorderLevel;
            Part part(id, name, quantity, reorderLevel);
            warehouse.addPart(part);
        } else if (choice == 2) {
            int id, quantity;
            std::cout << "Enter Part ID: ";
            std::cin >> id;
            std::cout << "Enter Quantity to Update: ";
            std::cin >> quantity;
            warehouse.updateQuantity(id, quantity);
        } else if (choice == 3) {
            std::vector<Order> orders = warehouse.checkReorderLevels();
            for (auto& order : orders) {
                std::cout << "Order placed for PartID " << order.partID << " with quantity " << order.quantity << std::endl;
            }
        } else if (choice == 4) {
            int jobID, partID, quantity;
            std::cout << "Enter Job ID: ";
            std::cin >> jobID;
            Job job(jobID);
            std::cout << "Enter number of parts to allocate: ";
            int numParts;
            std::cin >> numParts;
            for (int i = 0; i < numParts; ++i) {
                std::cout << "Enter Part ID: ";
                std::cin >> partID;
                std::cout << "Enter Quantity: ";
                std::cin >> quantity;
                job.addRequiredPart(partID, quantity);
            }
            job.allocateParts(warehouse);
            std::cout << "Job Status:" << std::endl;
            for (auto& pair : job.getJobStatus()) {
                std::cout << "PartID " << pair.first << ": " << (pair.second == 0 ? "Allocated" : "Pending") << std::endl;
            }
        } else if (choice == 5) {
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

        std::cout << "Inventory Status:" << std::endl;
        for (auto& pair : warehouse.getInventoryStatus()) {
            std::cout << "PartID " << pair.first << ": " << pair.second << std::endl;
        }
    }

    return 0;
}