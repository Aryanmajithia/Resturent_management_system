#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib> // For password input
#include <algorithm> // For searching in order history
#include <map> 
#include<fstream>// For table management

using namespace std;

// Class for menu items
class MenuItem {
public:
    MenuItem(const string& name, int price) : name(name), price(price) {}

    const string& getName() const {
        return name;
    }

    int getPrice() const {
        return price;
    }

    void setName(const string& newName) {
        name = newName;
    }

    void setPrice(int newPrice) {
        price = newPrice;
    }

private:
    string name;
    int price;
};

// Class for the shopping cart
class Cart {
public:
    void addItem(const MenuItem& item, int quantity) {
        items.push_back({ item, quantity });
    }

    const vector<pair<MenuItem, int>>& getItems() const {
        return items;
    }

private:
    vector<pair<MenuItem, int>> items;
};

// Class for customer details
class Customer {
public:
    void setName(const string& name) {
        this->name = name;
    }

    const string& getName() const {
        return name;
    }

    void setTableNumber(int tableNumber) {
        this->tableNumber = tableNumber;
    }

    int getTableNumber() const {
        return tableNumber;
    }

    void setStudentId(const string& studentId) {
        this->studentId = studentId;
    }

    const string& getStudentId() const {
        return studentId;
    }

private:
    string name;
    int tableNumber;
    string studentId;
    friend class Restaurant;
};

// Class for order details
class Order {
public:
    Order(const Customer& customer, const Cart& cart, bool dineIn) : customer(customer), cart(cart), dineIn(dineIn) {}

    const Customer& getCustomer() const {
        return customer;
    }

    const Cart& getCart() const {
        return cart;
    }

    bool isDineIn() const {
        return dineIn;
    }

    double getTotalBill() const {
        return totalBill;
    }

    void setTotalBill(double bill) {
        totalBill = bill;
    }

    double getTax() const {
        return tax;
    }

    void setTax(double taxAmount) {
        tax = taxAmount;
    }

    double getPackingCharges() const {
        return packingCharges;
    }

    void setPackingCharges(double charges) {
        packingCharges = charges;
    }

    double getDiscount() const {
        return discount;
    }

    void setDiscount(double discountAmount) {
        discount = discountAmount;
    }

private:
    Customer customer;
    Cart cart;
    bool dineIn;
    double totalBill = 0.0;
    double tax = 0.0;
    double packingCharges = 0.0;
    double discount = 0.0;
};
class TableManager {
public:
    TableManager(int maxTables) : maxTables(maxTables) {
        // Initialize tables with available status
        for (int i = 1; i <= maxTables; ++i) {
            tables[i] = false; // false means available
        }
    }

    // Check if a table is available
    bool isTableAvailable(int tableNumber) const {
        auto it = tables.find(tableNumber);
        return it != tables.end() && !it->second;
    }

    // Reserve a table for a customer
    bool reserveTable(int tableNumber) {
        auto it = tables.find(tableNumber);
        if (it != tables.end() && !it->second) {
            it->second = true; // Set table as occupied
            return true;
        }
        return false; // Table not available or doesn't exist
    }

    // Mark a table as vacant when the customer leaves
    bool vacateTable(int tableNumber) {
        auto it = tables.find(tableNumber);
        if (it != tables.end() && it->second) {
            it->second = false; // Set table as available
            return true;
        }
        return false; // Table not occupied or doesn't exist
    }

private:
    int maxTables;
    map<int, bool> tables; // Table number to availability status
};
// Class for the restaurant
class Restaurant {
public:
 Restaurant() {
        //orders = loadOrderHistoryFromFile(); // Load order history from the file
    }
    // Customer menu
    void customerMenu() {
        char customerOption;

        while (true) {
            cout << "\nCustomer Menu\n";
            cout << "1. Dine In\n";
            cout << "2. Take Away\n";
             cout << "3. Generate Bill\n";
            cout << "4. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> customerOption;

            switch (customerOption) {
                case '1':
                    dineIn();
                    break;
                case '2':
                    takeAway();
                    break;
                     case '3':
                    generateBill();
                    break;
                case '4':
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    // Administrative menu
    void administrativeMenu() {
        string adminPassword;
        cout << "Enter the administrative password: ";
        cin >> adminPassword;

        if (adminPassword == "******") { // Change the password as needed
            char adminOption;

            while (true) {
                cout << "-----------------------------------\n";
                cout << "     Administrative Menu\n";
                cout << "-----------------------------------\n";
                cout << "1. View All Orders\n";
                cout << "2. Add New Item\n";
                cout << "3. Change Price\n";
                cout << "4. Rename Item\n";
                cout << "5. Set Special Item of the Day\n";
                cout << "6. Back to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> adminOption;

                switch (adminOption) {
                    case '1': // View All Orders
                        viewAllOrders();
                        break;
                    case '2': // Add New Item
                        addNewItem();
                        break;
                    case '3': // Change Price
                        changePrice();
                        break;
                    case '4': // Rename Item
                        renameItem();
                        break;
                    case '5': // Set Special Item of the Day
                        setSpecialItem();
                        break;
                    case '6': // Back to Main Menu
                        return;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
            }
        } else {
            cout << "Access denied. Incorrect password." << endl;
        }
    }

    // View all orders
    void viewAllOrders() {
    cout << "-----------------------------------\n";
    cout << "     All Orders\n";
    cout << "-----------------------------------\n";

    if (orders.empty()) {
        cout << "No orders found in the history." << endl;
    } else {
        ofstream outputFile("order_history.txt", ios::app);  // Open the file in append mode

        if (!outputFile.is_open()) {
            cout << "Unable to open the file for writing." << endl;
        } else {
            for (const Order& order : orders) {
                displayOrderDetails(order);
                outputFile << "Order by " << order.getCustomer().getName() << " (Table " << order.getCustomer().getTableNumber() << "):\n";
                outputFile << "Order Type: " << (order.isDineIn() ? "Dine In" : "Take Away") << endl;

                for (const auto& item : order.getCart().getItems()) {
                    const string& itemName = item.first.getName();
                    int quantity = item.second;
                    outputFile << setw(2) << quantity << " x " << setw(20) << left << itemName << " - Rs " << (quantity * item.first.getPrice()) << endl;
                }

                outputFile << "Total Bill: Rs " << order.getTotalBill() << endl;
                outputFile << "Tax: Rs " << order.getTax() << endl;
                if (order.isDineIn()) {
                    outputFile << "Discount: Rs " << order.getDiscount() << endl;
                }
                outputFile << "Packing Charges: Rs " << order.getPackingCharges() << endl;
                outputFile << "Grand Total: Rs " << (order.getTotalBill() + order.getTax() + order.getPackingCharges() - order.getDiscount()) << endl;
                outputFile << "-----------------------------------\n";
            }
            outputFile.close();
            cout << "Order history saved to 'order_history.txt'." << endl;
        }
    }

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}


    // View order history
    void viewOrderHistory() {
        cout << "-----------------------------------\n";
        cout << "     Order History\n";
        cout << "-----------------------------------\n";

        if (orders.empty()) {
            cout << "No orders found in the history." << endl;
        } else {
            string customerName;
            cout << "Enter customer name to search orders: ";
            cin.ignore();
            getline(cin, customerName);

            bool found = false;

            for (const Order& order : orders) {
                const Customer& customer = order.getCustomer();
                const Cart& cart = order.getCart();

                if (customer.getName() == customerName) {
                    displayOrderDetails(order);
                    found = true;
                }
            }

            if (!found) {
                cout << "No orders found for customer: " << customerName << endl;
            }
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    // Add a new item to the menu
    void addNewItem() {
        string itemName;
        int itemPrice;

        cout << "Enter the name of the new item: ";
        cin.ignore();
        getline(cin, itemName);

        cout << "Enter the price of the new item: ";
        cin >> itemPrice;

        menu.push_back(MenuItem(itemName, itemPrice));
        cout << "New item added: " << itemName << " - Rs " << itemPrice << endl;
    }

    // Change the price of an existing menu item
    void changePrice() {
        int itemNumber, newPrice;

        displayMenu();

        cout << "Enter the item number to change the price: ";
        cin >> itemNumber;

        if (itemNumber >= 1 && itemNumber <= menu.size()) {
            cout << "Enter the new price for " << menu[itemNumber - 1].getName() << ": ";
            cin >> newPrice;

            menu[itemNumber - 1].setPrice(newPrice);
            cout << "Price for " << menu[itemNumber - 1].getName() << " updated to Rs " << newPrice << endl;
        } else {
            cout << "Invalid item number. Please try again." << endl;
        }
    }

    // Rename an existing menu item
    void renameItem() {
        int itemNumber;
        string newName;

        displayMenu();

        cout << "Enter the item number to rename: ";
        cin >> itemNumber;

        if (itemNumber >= 1 && itemNumber <= menu.size()) {
            cout << "Enter the new name for " << menu[itemNumber - 1].getName() << ": ";
            cin.ignore();
            getline(cin, newName);

            menu[itemNumber - 1].setName(newName);
            cout << "Item renamed to " << newName << endl;
        } else {
            cout << "Invalid item number. Please try again." << endl;
        }
    }

    // Set a menu item as the Special Item of the Day
    void setSpecialItem() {
        int itemNumber;

        displayMenu();

        cout << "Enter the item number to set as Special Item of the Day: ";
        cin >> itemNumber;

        if (itemNumber >= 1 && itemNumber <= menu.size()) {
            specialItem = menu[itemNumber - 1].getName();
            specialPrice = menu[itemNumber - 1].getPrice();
            cout << menu[itemNumber - 1].getName() << " has been set as the Special Item of the Day." << endl;
        } else {
            cout << "Invalid item number. Please try again." << endl;
        }
    }

    // Display the menu with grouped sections
    void displayMenu() {
        cout << "Menu:\n";
        
        // Grouped sections
        displaySection("Appetizers", 1, 10);
        displaySection("soups", 11, 20);
        displaySection("Salads", 21, 30);
        displaySection("Drinks", 31, 40);
        displaySection("Desserts", 41, 50);
        displaySection("Continental", 51, 60);
        displaySection("Italian", 61, 70);
        displaySection(" Breads", 71,84);
        displaySection("Pulao",85,92);
        displaySection("Briyani",93,104);
        displaySection("Non-Vegetarian Dishes",105,121);
        displaySection("Indian Vegetarian Dishes",122,132);
        displaySection("Panner dishes",133,142);
        
        // Special Item of the Day
        cout << "0. " << specialItem << " - Rs " << specialPrice << " (Special)" << endl;
    }

    // Helper function to display a section of the menu
    void displaySection(const string& sectionName, int startIdx, int endIdx) {
        cout << "-----------------------------------" << endl;
        cout << " " << sectionName << " Menu:" << endl;
        cout << "-----------------------------------" << endl;

        for (int i = startIdx; i <= endIdx && i <= menu.size(); ++i) {
            cout << i << ". " << menu[i - 1].getName() << " - Rs " << menu[i - 1].getPrice() << endl;
        }
    }

    // Calculate the total bill
    double calculateBill(const Cart& cart, bool isStudent = false, bool isTakeaway = false) {
    double totalBill = 0.0;

    for (const auto& item : cart.getItems()) {
        const MenuItem& menuItem = item.first;
        int quantity = item.second;
        totalBill += quantity * menuItem.getPrice();
    }

    if (isStudent && !isTakeaway) {
        totalBill -= totalBill * 0.30; // 30% student discount for dine-in
    }

    return totalBill;
}


    // Calculate taxes for the bill
    double calculateTax(double billAmount) {
        // You can implement your tax calculation logic here
        // For example, you can calculate 18% tax on the bill amount
        return 0.18 * billAmount;
    }

    // Calculate packing charges for take away
    double calculatePackingCharges(double billAmount) {
        // You can implement your packing charges logic here
        // For example, 10% packing charges on the bill amount for take away
        return 0.10 * billAmount;
    }

    // Add an order to the history
    void addOrder(const Order& order) {
        orders.push_back(order);
    }

    // Display order details including taxes, discounts, packing charges, and order mode
   void displayOrderDetails(const Order& order) {
    const Customer& customer = order.getCustomer();
    const Cart& cart = order.getCart();
    bool isDineIn = order.isDineIn();
    double totalBill = order.getTotalBill();
    double taxAmount = order.getTax();
    double packingCharges = isDineIn ? 0.0 : order.getPackingCharges();
    double discountAmount = isDineIn ? order.getDiscount() : 0.0; // Get the discount amount for dine-in

    cout << "Order by " << customer.getName() << " (Table " << customer.getTableNumber() << "):\n";
    cout << "Order Type: " << (isDineIn ? "Dine In" : "Take Away") << endl;

    for (const auto& item : cart.getItems()) {
        const string& itemName = item.first.getName();
        int quantity = item.second;

        cout << setw(2) << quantity << " x " << setw(20) << left << itemName << " - Rs " << (quantity * item.first.getPrice()) << endl;
    }

    cout << "Total Bill: Rs " << totalBill << endl;
    cout << "Tax: Rs " << taxAmount << endl;

    if (isDineIn) {
        cout << "Discount: Rs " << discountAmount << endl;
    }

    cout << "Packing Charges: Rs " << packingCharges << endl;
    cout << "Grand Total: Rs " << (totalBill + taxAmount + packingCharges - discountAmount) << endl;
}
void generateBill() {
        string customerName;
        cout << "Enter the customer's name for the bill: ";
        cin.ignore();
        getline(cin, customerName);

        // Search for the customer's order in the order history
        for (const Order& order : orders) {
            if (order.getCustomer().getName() == customerName) {
                displayOrderDetails(order);
                return;
            }
        }

        // If the order is not found
        cout << "No orders found for customer: " << customerName << endl;
    }

    // Additional functions for Dine In and Take Away
    void dineIn() {
        Customer customer;
        Cart cart;
        string studentId;

        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, customerName);

        customer.setName(customerName);

        int tableNumber;
        cout << "Enter table number: ";
        cin >> tableNumber;
        customer.setTableNumber(tableNumber);
        // Check table availability
         tableNumber = customer.getTableNumber();
        if (!tableManager.isTableAvailable(tableNumber)) {
            cout << "Table " << tableNumber << " is already occupied. Please choose another table." << endl;
            return;
        }

        // Reserve the table
        if (!tableManager.reserveTable(tableNumber)) {
            cout << "Table " << tableNumber << " is not available or doesn't exist." << endl;
            return;
        }

        char studentChoice;
        cout << "Is the customer a student? (y/n): ";
        cin >> studentChoice;

        if (studentChoice == 'y' || studentChoice == 'Y') {
            cout << "Enter student ID: ";
            cin >> studentId;
            customer.setStudentId(studentId);
        }

        displayMenu();
        cout << "Enter the items and quantities (item_number quantity), e.g., '1 2' for 2 servings of the first item. Enter '0' to finish.\n";

        while (true) {
            int itemNumber, quantity;
            cin >> itemNumber;

            if (itemNumber == 0) {
                break;
            }

            if (itemNumber >= 1 && itemNumber <= menu.size()) {
                cout << "Enter quantity for " << menu[itemNumber - 1].getName() << ": ";
                cin >> quantity;
                cart.addItem(menu[itemNumber - 1], quantity);
            } else {
                cout << "Invalid item number. Please try again." << endl;
            }
        }

        bool isStudent = (studentChoice == 'y' || studentChoice == 'Y');
    int totalBill = calculateBill(cart, isStudent);
    double taxAmount = calculateTax(totalBill);
    double discountAmount = isStudent ? (totalBill * 0.30) : 0.0; // Apply student discount for dine-in

    Order order(customer, cart, true); // Dine In
    order.setTotalBill(totalBill);
    order.setTax(taxAmount);
    order.setDiscount(discountAmount);
    addOrder(order);

    cout << "Order placed for " << customer.getName() << " (Table " << customer.getTableNumber() << "). Total Bill: Rs " << totalBill << endl;
    cout << "Enjoy your meal!" << endl;
}

    void takeAway() {
        Customer customer;
        Cart cart;
        string studentId;

        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, customerName);

        customer.setName(customerName);

        char studentChoice;
        cout << "Is the customer a student? (y/n): ";
        cin >> studentChoice;

        if (studentChoice == 'y' || studentChoice == 'Y') {
            cout << "Enter student ID: ";
            cin >> studentId;
            customer.setStudentId(studentId);
        }

        displayMenu();
        cout << "Enter the items and quantities (item_number quantity), e.g., '1 2' for 2 servings of the first item. Enter '0' to finish.\n";

        while (true) {
            int itemNumber, quantity;
            cin >> itemNumber;

            if (itemNumber == 0) {
                break;
            }

            if (itemNumber >= 1 && itemNumber <= menu.size()) {
                cout << "Enter quantity for " << menu[itemNumber - 1].getName() << ": ";
                cin >> quantity;
                cart.addItem(menu[itemNumber - 1], quantity);
            } else {
                cout << "Invalid item number. Please try again." << endl;
            }
        }

       bool isStudent = (studentChoice == 'y' || studentChoice == 'Y');
    int totalBill = calculateBill(cart, isStudent, true); // Pass true to indicate take away
    double taxAmount = calculateTax(totalBill);
    double packingCharges = calculatePackingCharges(totalBill);

    Order order(customer, cart, false); // Take Away
    order.setTotalBill(totalBill);
    order.setTax(taxAmount);
    order.setPackingCharges(packingCharges);
    addOrder(order);

    cout << "Take Away order placed for " << customer.getName() << ". Total Bill: Rs " << totalBill << endl;
    cout << "Thank you for choosing us!" << endl;
}
//const vector<Order>& getOrders() const {
  //      return orders;
    //}
private:
// vector<Order> orders;
 string customerName;
    vector<MenuItem> menu = {
        // Appetizers
{"Vegetable Spring Rolls", 120},
{"Chicken Wings", 180},
{"Onion Rings", 100},
{"Chilli Paneer", 160},
{"Fish Fingers", 200},
{"Mutton Kebabs", 220},
{"Crispy Tofu", 140},
{"Garlic Bread", 90},
{"Potato Wedges", 110},
{"Assorted Non-Veg Platter", 250},
// Soups
{"Tomato Soup", 90},
{"Chicken Noodle Soup", 120},
{"Mushroom Soup", 100},
{"Lentil Soup", 80},
{"Hot and Sour Soup", 110},
{"Minestrone Soup", 100},
{"Creamy Broccoli Soup", 120},
{"Sweet Corn Soup", 90},
{"Chicken Corn Soup", 120},
{"Spinach and Lentil Soup", 100},

      // Salads
        {"Caesar Salad", 150},
        {"Greek Salad", 140},
        {"Coleslaw Salad", 120},
        {"Fruit Salad", 100},
        {"Caprese Salad", 160},
        {"Quinoa Salad", 180},
        {"Cucumber Salad", 110},
        {"Bean Salad", 130},
        {"Pasta Salad", 170},
        {"Chicken Caesar Salad", 180},

        // Drinks
        {"Coke", 40},
        {"Pepsi", 40},
        {"Lemonade", 50},
        {"Iced Tea", 50},
        {"Orange Juice", 60},
        {"Mango Lassi", 70},
        {"Strawberry Shake", 80},
        {"Cappuccino", 90},
        {"Espresso", 80},
        {"Mineral Water", 30},

        // Desserts
        {"Chocolate Cake", 120},
        {"Apple Pie", 110},
        {"Ice Cream Sundae", 130},
        {"Fruit Tart", 140},
        {"Cheesecake", 150},
        {"Tiramisu", 160},
        {"Gulab Jamun", 90},
        {"Rasgulla", 80},
        {"Brownie with Ice Cream", 170},
        {"Panna Cotta", 130},

        // Continental
        {"Spaghetti Carbonara", 200},
        {"Grilled Salmon", 250},
        {"Beef Burger", 180},
        {"Club Sandwich", 160},
        {"Veggie Wrap", 150},
        {"Chicken Alfredo", 220},
        {"Caesar Wrap", 160},
        {"Mushroom Risotto", 210},
        {"Quiche Lorraine", 180},
        {"Garlic Shrimp Pasta", 230},

        // Italian
        {"Margherita Pizza", 180},
        {"Pasta Primavera", 160},
        {"Lasagna", 220},
        {"Calzone", 190},
        {"Bruschetta", 80},
        {"Minestrone Soup", 100},
        {"Tiramisu", 150},
        {"Ravioli", 180},
        {"Panna Cotta", 130},
        {"Gnocchi", 170},

        // Breads
         {"Garlic Naan", 40},
        {"Butter Naan", 30},
        {"Tandoori Roti", 20},
        {"Kulcha", 35},
        {"Paratha", 50},
        {"Bhature", 40},
        {"Puri", 30},
        {"Focaccia Bread", 60},
        {"Baguette", 50},
        {"Whole Wheat Bread", 45},
        {"Spaghetti Carbonara", 350},
        {"Grilled Salmon", 420},
        {"Steak", 480},
        {"Lasagna", 370},
        // Pulao
{"Veg Pulao", 200},
{"Chicken Pulao", 280},
{"Mutton Pulao", 330},
{"Egg Pulao", 220},
{"Paneer Pulao", 240},
{"Mixed Veg Pulao", 210},
{"Schezwan Pulao", 250},

// Biryani
{"Vegetable Biryani", 220},
{"Chicken Biryani", 300},
{"Mutton Biryani", 350},
{"Egg Biryani", 260},
{"Paneer Biryani", 240},
{"Hyderabadi Biryani", 320},
{"Mushroom Biryani", 250},
{"Veg Dum Biryani", 220},
{"Chicken Dum Biryani", 320},
{"Mutton Dum Biryani", 380},
{"Fish Dum Biryani", 300},

// Non-Vegetarian Dishes
{"Chicken Tikka Masala", 280},
{"Butter Chicken", 320},
{"Fish Curry", 280},
{"Mutton Rogan Josh", 350},
{"Egg Curry", 240},
{"Chicken Korma", 290},
{"Fish Fry", 260},
{"Tandoori Chicken", 270},
{"Chicken Chilli", 260},
{"Mutton Keema", 310},
{"Chicken Shawarma", 220},
{"Fish Tandoori", 290},
{"Chicken Malai Kebab", 250},
{"Mutton Do Pyaza", 340},
{"Chicken 65", 240},
{"Fish Masala", 270},

// Indian Vegetarian Dishes
{"Paneer Tikka", 180},
{"Aloo Gobi", 150},
{"Chana Masala", 160},
{"Baingan Bharta", 170},
{"Palak Paneer", 180},
{"Dal Tadka", 140},
{"Vegetable Biryani", 200},
{"Matar Paneer", 180},
{"Malai Kofta", 190},
{"Chole Bhature", 150},

// Paneer Dishes
{"Paneer Butter Masala", 190},
{"Kadai Paneer", 180},
{"Paneer Makhani", 200},
{"Shahi Paneer", 210},
{"Paneer Bhurji", 160},
{"Paneer Tikka Masala", 200},
{"Paneer Do Pyaza", 190},
{"Paneer Korma", 200},
{"Paneer Jalfrezi", 180},
};
    string specialItem = "pizza";
    int specialPrice = 485;
    vector<Order> orders;
    int maxTables = 10; // Maximum number of tables
    TableManager tableManager = TableManager(maxTables); 
};

int main() {
    Restaurant restaurant;
    char option;

    while (true) {
        cout << "-----------------------------------\n";
        cout << "     Restaurant Management System\n";
        cout << "-----------------------------------\n";
        cout << "1. Customer screen\n";
        cout << "2. Administrative screen \n";
        cout << "3. View Order History\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> option;

        switch (option) {
            case '1':
                restaurant.customerMenu();
                break;
            case '2':
                restaurant.administrativeMenu();
                break;
            case '3':
                restaurant.viewOrderHistory();
                break;
            case '4':
                cout << "Exiting the program. Goodbye!" << endl;
                //saveOrderHistoryToFile(restaurant.getOrders());
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}