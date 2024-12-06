#include <iostream>

using namespace std;

// Grocery Items class
class GroceryItem {
public:
    string Itemname;
    double price;
    GroceryItem* next;

    // Constructor
    GroceryItem(string Itemname, double price) {
        this->Itemname = Itemname;
        this->price = price;
        next = NULL;
    }
};
// Customer class
class Customer {
public:
    int customerId;
    string name;
    Customer* next;

    Customer(int id, string name) {
        this->customerId = id;
        this->name = name;
        next = NULL;
    }
};
class OrderItem {
public:
    string Itemname;
    int Quantity;
    double Price;
    OrderItem* next;

    OrderItem(string itemName, int quantity, double price) {
        Itemname = itemName;
        Quantity = quantity;
        Price = price;
        next = NULL;
    }
};

class Order {
public:
    int OrderId;
    string customerName;
    OrderItem* items; // Linked list of items
    Order* next;

    // Constructor
    Order(int ID, string name) {
        OrderId = ID;
        customerName = name;
        items = NULL;
        next = NULL;
    }

     //Add item to the order
    void addItem(string itemName, int quantity, double price) {
        OrderItem* newItem = new OrderItem(itemName, quantity, price);
        if (items == NULL) {
            items = newItem;
        } else {
            OrderItem* temp = items;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newItem;
        }
    }



    // Display all items in the order
    void displayItems() {
        OrderItem* temp = items;
        while (temp != NULL) {
            cout << "  Item: " << temp->Itemname
                 << ", Quantity: " << temp->Quantity
                 << ", Price: $" << temp->Price << endl;
            temp = temp->next;
        }
    }

};

// Delivery System class
class DeliverySystem {
private:
    GroceryItem* head;
    Order* front;
    Order* rear;
    int nextOrderId;
    Customer* customerHead;

public:
    DeliverySystem() {
        head = NULL;
        front = NULL;
        rear = NULL;
        customerHead=NULL;
        nextOrderId = 1;
        
  }

    // Add Grocery Items
    void addGroceryItem(string name, double price) {
        GroceryItem* newItem = new GroceryItem(name, price);
        if (head == NULL) {
            head = newItem;
        } else {
            GroceryItem* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newItem;
        }
        
    }

    // Display Grocery Items
    void DisplayGroceryItems() {
        if (head == NULL) {
            cout << "No items available in the store." << endl;
            return;
        }
        cout << "Available Grocery Items:\n";
        
        GroceryItem* temp = head;
        while (temp != NULL) {
            cout << "- " << temp->Itemname << ": $" << temp->price << endl;
            temp = temp->next;
   
        }
    }


// Add a customer
   void addCustomer(int customerId, string name) {
    // Check for duplicate customer ID
    Customer* temp = customerHead;
    while (temp != NULL) {
        if (temp->customerId == customerId) {
            cout << "Customer ID already exists. Please try again with a unique ID." << endl;
            return;
        }
        temp = temp->next;
    }

    // Add new customer
    Customer* newCustomer = new Customer(customerId, name);
    if (customerHead == NULL) {
        customerHead = newCustomer;
    } else {
        Customer* temp = customerHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newCustomer;
    }
    cout << "Customer added successfully: " << name << " (ID: " << customerId << ")" << endl;
}
    // Display customers
    void displayCustomers() {
        if (customerHead == NULL) {
            cout << "No customers available!" << endl;
            return;
        }
        cout << "Registered Customers:\n";
        Customer* temp = customerHead;
        while (temp != NULL) {
            cout << "- ID: " << temp->customerId << ", Name: " << temp->name << endl;
            temp = temp->next;
        }
    }

    // Check if a customer exists
    bool customerExists(int customerId) {
        Customer* temp = customerHead;
        while (temp != NULL) {
            if (temp->customerId == customerId) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
 
    // Check if a customer has a pending or delivered order
bool isOrderAlreadyProcessed(int customerId) {
    // Check pending orders
    Order* temp = front;
    while (temp) {
        if (temp->customerName == getCustomerNameById(customerId)) {
            return true; // Pending order exists
        }
        temp = temp->next;
    }
    return false; // No pending orders for the customer
}

// Helper function to get customer name by ID
string getCustomerNameById(int customerId) {
    Customer* temp = customerHead;
    while (temp) {
        if (temp->customerId == customerId) {
            return temp->name;
        }
        temp = temp->next;
    }
    return ""; // Return empty string if not found
}

// Place an order
void PlaceOrder(int customerId) {
    if (!customerExists(customerId)) {
        cout << "Customer ID " << customerId << " not found. Please register the customer first.\n";
        return;
    }

    if (isOrderAlreadyProcessed(customerId)) {
        cout << "An order for Customer ID " << customerId << " has already been processed or is pending delivery.\n";
        cout << "Order cannot be placed again.\n";
        return;
    }

    string customerName = getCustomerNameById(customerId);

    double totalCost = 0.0;
    char addMore;

    Order* newOrder = new Order(nextOrderId++, customerName);

    do {
        string itemName;
        int quantity;
        cout << "Enter item name: ";
        cin >> itemName;

        GroceryItem* temp = head;
        bool found = false;
        while (temp) {
            if (temp->Itemname == itemName) {
                found = true;
                cout << "Enter quantity: ";
                cin >> quantity;
                totalCost += temp->price * quantity;
                newOrder->addItem(itemName, quantity, temp->price);
                cout << "Added to the order:\n"
                     << "  Item Name: " << itemName << "\n"
                     << "  Quantity: " << quantity << "\n"
                     << "  Price: $" << temp->price << endl;
                break;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "Item not found!" << endl;
        }

        cout << "Add more items? (y/n): ";
        cin >> addMore;

    } while (addMore == 'y' || addMore == 'Y');

    if (!newOrder->items) {
        // No items were added to the order
        cout << "No valid items were added to the order. Order cannot be placed.\n";
        delete newOrder;
        return;
    }

    // Add order to the queue
    if (!front) {
        front = rear = newOrder;
    } else {
        rear->next = newOrder;
        rear = newOrder;
    }

    cout << "Order placed successfully! Total cost: $" << totalCost << endl;
}



    // Search for an item in the grocery list by its name
void searchGroceryItem(string itemName) {
    GroceryItem* temp = head;
    bool found = false;
    while (temp) {
        if (temp->Itemname == itemName) {
            cout << "Item found: " << temp->Itemname << ", Price: $" << temp->price << endl;
            found = true;
            break;
        }
        temp = temp->next;
    }
    if (!found) {
        cout << "Item not found in the grocery list." << endl;
    }
}
void cancelOrder(int orderId) {
    if (front == NULL) {
        cout << "No pending orders to cancel!" << endl;
        return;
    }

    if (front->OrderId == orderId) {
        Order* temp = front;
        front = front->next;
        delete temp;
        cout << "Order ID " << orderId << " canceled successfully." << endl;
        return;
    }

    Order* current = front;
    Order* prev = NULL;
    while (current && current->OrderId != orderId) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        cout << "Order ID not found!" << endl;
        return;
    }

    prev->next = current->next;
    if (current == rear) {
        rear = prev;
    }
    delete current;
    cout << "Order ID " << orderId << " canceled successfully." << endl;
}
// Remove an item from a specific order
void removeItemFromOrder(int orderId, string itemName) {
    Order* tempOrder = front;
    while (tempOrder) {
        if (tempOrder->OrderId == orderId) {
            OrderItem* tempItem = tempOrder->items;
            OrderItem* prevItem = NULL;
            bool found = false;

            // Traverse through the order items to find the item
            while (tempItem) {
                if (tempItem->Itemname == itemName) {
                    if (prevItem == NULL) { // Removing the first item
                        tempOrder->items = tempItem->next;
                    } else {
                        prevItem->next = tempItem->next; // Remove item from list
                    }
                    delete tempItem;
                    cout << "Item " << itemName << " removed from Order ID " << orderId << "." << endl;
                    found = true;
                    break;
                }
                prevItem = tempItem;
                tempItem = tempItem->next;
            }

            if (!found) {
                cout << "Item " << itemName << " not found in Order ID " << orderId << "." << endl;
            }
            return;
        }
        tempOrder = tempOrder->next;
    }

    cout << "Order ID " << orderId << " not found!" << endl;
}


    // Deliver Order
    void deliverOrder() {
        if (front == NULL) {
            cout << "No orders to deliver!" << endl;
            return;
        }

        Order* temp = front;
        cout << "Delivering Order ID: " << temp->OrderId << " - "
             << "Customer: " << temp->customerName << endl;
        temp->displayItems(); // Display all items in the order

        // Remove the order from the queue
        front = front->next;
        delete temp;
    }

    // Display Pending Orders
    void displayPendingOrders() {
        if (front == NULL) {
            cout << "No pending orders!" << endl;
            return;
        }

        cout << "Pending Orders:\n";
        Order* temp = front;
        while (temp != NULL) {
            cout << "Order ID: " << temp->OrderId << ", Customer: " << temp->customerName << endl;
            temp->displayItems(); // Display all items in the order
            temp = temp->next;
        }
    }
};
int main() {
    DeliverySystem s;
    int choice;
    s.addGroceryItem("Apple", 10);
    s.addGroceryItem("Banana", 15);
    s.addGroceryItem("Milk", 8);
    s.addGroceryItem("Bread", 14);
    s.addGroceryItem("Eggs", 12);
    s.addGroceryItem("Orange", 7);
    s.addGroceryItem("Yougurt", 2);
    s.addGroceryItem("Grapes", 17);
    s.addGroceryItem("Cucumber", 4);
    s.addGroceryItem("Potatos", 9);

    
do {
    cout << "\n--- Grocery Delivery Management System ---\n";
    cout << "1. Add Item\n";
    cout << "2. Display Items\n";
     cout << "3. Add Customer\n";
        cout << "4. Display Customers\n";
    cout << "5. Place Order\n";
    cout << "6. Deliver Order\n";
    cout << "7. Display Pending Orders\n";
    cout << "8. Search for an Item\n";
    cout << "9. Cancel an Order\n";
    cout << "10. Remove an Item from Order\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    	case 1: {
                string Itemname;
                double price;
                cout << "Enter item name: ";
                cin >> Itemname;
                cout << "Enter item price: ";
                cin >> price;
                s.addGroceryItem(Itemname, price); // Call to add grocery items
                break;
            }
            case 2:
                s.DisplayGroceryItems(); // Call to display grocery items
                break;
         case 3: {
    int customerId;
    string name;
    cout << "Enter customer ID: ";
    cin >> customerId;
    cout << "Enter customer name: ";
    cin >> name;
    s.addCustomer(customerId, name);
    break;
}

        case 4:
            s.displayCustomers();
            break;
        case 5: {
            int customerId;
            cout << "Enter customer ID: ";
            cin >> customerId;
            s.PlaceOrder(customerId);
            break;}

            
            case 6:
                s.deliverOrder(); // Call to deliver the next order
                break;
            case 7:
                s.displayPendingOrders(); // Call to display all pending orders
                break;
        case 8: {
            string itemName;
            cout << "Enter item name to search: ";
            cin >> itemName;
            s.searchGroceryItem(itemName);
            break;
        }
        case 9: {
            int orderId;
            cout << "Enter order ID to cancel: ";
            cin >> orderId;
            s.cancelOrder(orderId);
            break;
        }
        case 10: {
            int orderId;
            string itemName;
            cout << "Enter order ID to remove item from: ";
            cin >> orderId;
            cout << "Enter item name to remove: ";
            cin >> itemName;
            s.removeItemFromOrder(orderId, itemName);
            break;
        }
        case 0:
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        
        
        // Add other cases as needed
}
} while (choice != 0);
return 0;
}
