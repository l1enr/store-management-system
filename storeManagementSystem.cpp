#include <iostream>
#include <string>
using namespace std;

// ==================== Product Structure & BST Implementation ====================
struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
    Product* left;
    Product* right;

    Product(int id, string name, string category, double price, int quantity)
        : id(id), name(name), category(category), price(price), quantity(quantity), left(nullptr), right(nullptr) {}
};

class ProductBST {
private:
    Product* root;

    Product* insert(Product* node, Product* newProduct) {
        if (node == nullptr) return newProduct;
        if (newProduct->id < node->id)
            node->left = insert(node->left, newProduct);
        else if (newProduct->id > node->id)
            node->right = insert(node->right, newProduct);
        return node;
    }

    Product* search(Product* node, int id) {
        if (node == nullptr || node->id == id) return node;
        if (id < node->id) return search(node->left, id);
        return search(node->right, id);
    }

    Product* findMin(Product* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    Product* deleteNode(Product* node, int id) {
        if (node == nullptr) return nullptr;
        if (id < node->id) node->left = deleteNode(node->left, id);
        else if (id > node->id) node->right = deleteNode(node->right, id);
        else {
            if (node->left == nullptr) {
                Product* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Product* temp = node->left;
                delete node;
                return temp;
            }
            Product* temp = findMin(node->right);
            node->id = temp->id;
            node->name = temp->name;
            node->category = temp->category;
            node->price = temp->price;
            node->quantity = temp->quantity;
            node->right = deleteNode(node->right, temp->id);
        }
        return node;
    }

   void inOrderDisplay (Product* node) {
        if (node == nullptr) return;
        inOrderDisplay(node->left);
        cout << node->id << " - "
         << node->name << " - "
         << node->category << " - "
         << node->price << " SAR - Qty: "
         << node->quantity;

    if (node->quantity <= 10)
    {
        cout << "   WARNING: LOW STOCK!";
    }

    cout << endl;

    inOrderDisplay(node->right);
}
    

public:
    ProductBST() : root(nullptr) {}

    void addProduct(int id, string name, string category, double price, int quantity) {
        Product* newProduct = new Product(id, name, category, price, quantity);
        root = insert(root, newProduct);
        cout << "Product added successfully to BST catalog!\n";
    }

    Product* searchProduct(int id) {
        return search(root, id);
    }

    void deleteProduct(int id) {
        root = deleteNode(root, id);
        cout << "Product deleted successfully!\n";
    }

    void displayCatalog() {
        if (root == nullptr) {
            cout << "Catalog is empty.\n";
            return;
        }
        cout << "\n=== Product Catalog (Sorted by ID) ===\n";
        inOrderDisplay(root);
        cout << "======================================\n";
    }
};

// ==================== Manual Queue Implementation ====================
struct PurchaseOrder {
    int productId;
    int quantity;
    PurchaseOrder* next;
    
    PurchaseOrder(int pid, int qty) : productId(pid), quantity(qty), next(nullptr) {}
};

class OrderQueue {
private:
    PurchaseOrder* front;
    PurchaseOrder* rear;
    int size;

public:
    OrderQueue() : front(nullptr), rear(nullptr), size(0) {}

    void enqueue(int productId, int quantity) {
        PurchaseOrder* newOrder = new PurchaseOrder(productId, quantity);
        if (rear == nullptr) {
            front = rear = newOrder;
        } else {
            rear->next = newOrder;
            rear = newOrder;
        }
        size++;
    }

    PurchaseOrder* dequeue() {
        if (front == nullptr) return nullptr;
        PurchaseOrder* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        size--;
        return temp;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void displayQueue() {
        if (isEmpty()) {
            cout << "Order queue is empty.\n";
            return;
        }
        cout << "\n=== Pending Orders ===\n";
        PurchaseOrder* current = front;
        int count = 1;
        while (current) {
            cout << count++ << ". Product ID: " << current->productId 
                 << ", Quantity: " << current->quantity << endl;
            current = current->next;
        }
        cout << "=====================\n";
    }
};

// ==================== Manual Stack Implementation ====================
struct Operation {
    string type;
    int productId;
    int oldQuantity;
    Operation* next;
    
    Operation(string t, int pid, int oq = 0) : type(t), productId(pid), oldQuantity(oq), next(nullptr) {}
};

class UndoStack {
private:
    Operation* top;

public:
    UndoStack() : top(nullptr) {}

    void push(string type, int productId, int oldQuantity = 0) {
        Operation* newOp = new Operation(type, productId, oldQuantity);
        newOp->next = top;
        top = newOp;
    }

    Operation* pop() {
        if (top == nullptr) return nullptr;
        Operation* temp = top;
        top = top->next;
        return temp;
    }

    bool isEmpty() {
        return top == nullptr;
    }
};

// ==================== Main System ====================
class StoreSystem {
private:
    ProductBST catalog;
    OrderQueue orderQueue;
    UndoStack undoStack;

    void processPurchase(int productId, int quantity) {
        Product* product = catalog.searchProduct(productId);
        if (product == nullptr) {
            cout << "ERROR: Product not found!\n";
            return;
        }
        
        if (quantity <= 0) {
            cout << "ERROR: Invalid quantity!\n";
            return;
        }

        if (quantity > product->quantity) {
            cout << "WARNING: Insufficient stock! Available: " << product->quantity << endl;
            return;
        }

        // Save old quantity for undo
        undoStack.push("PURCHASE", productId, product->quantity);
        
        // Process purchase
        product->quantity -= quantity;
        double total = product->price * quantity;
        
        cout << "\n=== Purchase Confirmed ===\n";
        cout << "Product: " << product->name << " (ID: " << productId << ")\n";
        cout << "Quantity: " << quantity << "\n";
        cout << "Price per unit: " << product->price << " SAR\n";
        cout << "Total: " << total << " SAR\n";
        cout << "Remaining stock: " << product->quantity << "\n";
        cout << "==========================\n";
    }

    void processNextOrder() {
        if (orderQueue.isEmpty()) {
            cout << "No pending orders.\n";
            return;
        }

        PurchaseOrder* order = orderQueue.dequeue();
        if (order) {
            cout << "\nProcessing order - Product ID: " << order->productId 
                 << ", Quantity: " << order->quantity << endl;
            processPurchase(order->productId, order->quantity);
            delete order;
        }
    }

    void undoLastOperation() {
        Operation* lastOp = undoStack.pop();
        if (lastOp == nullptr) {
            cout << "No operations to undo.\n";
            return;
        }

        Product* product = catalog.searchProduct(lastOp->productId);
        if (product) {
            if (lastOp->type == "PURCHASE") {
                product->quantity = lastOp->oldQuantity;
                cout << "Undid purchase for product ID: " << lastOp->productId 
                     << ". Quantity restored to: " << lastOp->oldQuantity << endl;
            }
        }
        delete lastOp;
    }

public:
    void initializeSampleProducts() {
        catalog.addProduct(101, "Coffee Beans", "Food", 25.0, 50);
        catalog.addProduct(102, "Tea", "Food", 12.0, 40);
        catalog.addProduct(103, "Sugar", "Food", 8.0, 100);
        catalog.addProduct(104, "Milk", "Dairy", 7.0, 30);
        catalog.addProduct(105, "Cookies", "Snacks", 10.0, 60);
        catalog.addProduct(106, "Honey", "Food", 35.0, 20);
    }

    void run() {
        initializeSampleProducts();
        
        while (true) {
            cout << "\n=== Store Management System ===\n";
            cout << "1. Display Product Catalog\n";
            cout << "2. Search Product by ID\n";
            cout << "3. Add New Product\n";
            cout << "4. Delete Product\n";
            cout << "5. Purchase Product\n";
            cout << "6. Add Order to Queue\n";
            cout << "7. Process Next Order\n";
            cout << "8. Show Pending Orders\n";
            cout << "9. Undo Last Operation\n";
            cout << "10. Exit\n";
            cout << "Choose option: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    catalog.displayCatalog();
                    break;

                case 2: {
                    int id;
                    cout << "Enter product ID to search: ";
                    cin >> id;
                    Product* result = catalog.searchProduct(id);
                    if (result) {
                        cout << "FOUND -> " << result->id << " - " << result->name 
                             << " - " << result->category << " - " << result->price 
                             << " SAR - Qty: " << result->quantity << endl;
                    } else {
                        cout << "Product not found.\n";
                    }
                    break;
                }

                case 3: {
                    int id, quantity;
                    string name, category;
                    double price;
                    
                    cout << "Enter product ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter product name: ";
                    getline(cin, name);
                    cout << "Enter category: ";
                    getline(cin, category);
                    cout << "Enter price: ";
                    cin >> price;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    
                    catalog.addProduct(id, name, category, price, quantity);
                    break;
                }

                case 4: {
                    int id;
                    cout << "Enter product ID to delete: ";
                    cin >> id;
                    catalog.deleteProduct(id);
                    break;
                }

                case 5: {
                    int id, quantity;
                    cout << "Enter product ID: ";
                    cin >> id;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    processPurchase(id, quantity);
                    break;
                }

                case 6: {
                    int id, quantity;
                    cout << "Enter product ID: ";
                    cin >> id;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    orderQueue.enqueue(id, quantity);
                    cout << "Order added to queue.\n";
                    break;
                }

                case 7:
                    processNextOrder();
                    break;

                case 8:
                    orderQueue.displayQueue();
                    break;

                case 9:
                    undoLastOperation();
                    break;

                case 10:
                    cout << "Exiting system...\n";
                    return;

                default:
                    cout << "Invalid choice. Try again.\n";
            }
        }
    }
};

// ==================== Main Function ====================
int main() {
    StoreSystem store;
    store.run();
    return 0;
}
