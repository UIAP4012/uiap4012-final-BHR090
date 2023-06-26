#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Product {
public:
    Product(string id, string name, int quantity, double price) : id(std::move(id)), name(std::move(name)), quantity(quantity), price(price) {}

    explicit Product(int quantity) : quantity(quantity) {}

     string get_id() const {
        return id;
    }

     string get_name() const {
        return name;
    }

     int get_quantity() const;

    void set_quantity(int new_quantity) {
        quantity = new_quantity;
    }

     double get_price() const {
        return price;
    }

    double price{};
private:
    string id;
    string name;
    int quantity;
};

int Product::get_quantity() const {
    return quantity;
}

class Inventory {
public:
    void add_product(const Product& product) {
        products.push_back(product);
    }

    void remove_product(const string& id) {
        for (int i = 0; i < products.size(); i++) {
            if (products[i].get_id() == id) {
                products.erase(products.begin() + i);
                break;
            }
        }
    }

    void update_product(const string& id, int new_quantity, double new_price) {
        for (auto & product : products) {
            if (id == product.get_id()) {
                product.set_quantity(new_quantity);
                product.price = new_price;
                break;
            }
        }
    }

    void display_inventory() const {
        cout << "Inventory:\n";
        for (const auto& product : products) {
            cout << product.get_name() << " - Quantity: " << product.get_quantity() << " - Price: " << product.get_price() << endl;
        }
    }

    void save_inventory_to_file(const string& filename) const;

    void load_inventory_from_file(const string& filename);

    vector<Product> products;
};

void Inventory::save_inventory_to_file(const string &filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& product : products) {
            file << product.get_id() << "," << product.get_name() << "," << product.get_quantity() << "," << product.get_price() << endl;
        }
        file.close();
    }
}

void Inventory::load_inventory_from_file(const string &filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            string id, name;
            int quantity;
            double price;
            stringstream ss(line);
            getline(ss, id, ',');
            getline(ss, name, ',');
            ss >> quantity >> price;
            Product product(id, name, quantity, price);
            add_product(product);
        }
        file.close();
    }
}

class Customer {
public:
    Customer(string name, string email);

    string get_name() const {
        return name;
    }

     string get_email() const {
        return email;
    }



private:
    string name;
    string email;
};

Customer::Customer(string name, string email) : name(std::move(name)), email(std::move(email)) {}

class Invoice {
public:
    explicit Invoice(Customer customer);

    void add_product(const Product& product) {
        products.push_back(product);
    }

    [[nodiscard]] double get_total_price() const;

    void issue_invoice() const {
        cout << "Invoice:\n";
        for (const auto& product : products) {
            cout << product.get_name() << " - Price: " << product.get_price() << endl;
        }
        cout << "Total price: " << get_total_price() << endl;
    }

    void save_invoice_to_file(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "Customer name:," << customer.get_name() << "\n";
            file << "Customer email:," << customer.get_email() << "\n";
            file << "Product ID,Product Name,Quantity,Price\n";
            for (const auto& product : products) {
                file << product.get_id() << "," << product.get_name() << "," << product.get_quantity() << "," << product.get_price() << endl;
            }
            file << "Total price:," << get_total_price() << endl;
            file.close();
        }
    }

private:
     Customer customer;
    vector<Product> products;
};

Invoice::Invoice(Customer customer) : customer(std::move(customer)) {}

double Invoice::get_total_price() const {
    double total_price = 0;
    for (const auto& product : products) {
        total_price += product.get_price();
    }
    return total_price;
}

int main() {
    Inventory inventory;
    inventory.load_inventory_from_file("inventory.txt");

    int option = 0;
    while (option != 6) {
        cout << "1. Add product\n";
        cout << "2. Remove product\n";
        cout << "3. Update product\n";
        cout << "4. Display inventory\n";
        cout << "5. Issue invoice\n";
        cout << "6. Exit\n";
        cout << "Enter your option: ";
        cin >> option;
        if (option == 1) {
            string id, name;
            int quantity;
            double price;
            cout << "Enter product ID: ";
            cin >> id;
            cout << "Enter product name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter product quantity: ";
            cin >> quantity;
            cout << "Enter product price: ";
            cin >> price;
            Product product(id, name, quantity, price);
            inventory.add_product(product);
            cout << "Product added to inventory.\n";
        } else if (option == 2) {
            string id;
            cout << "Enter product ID: ";
            cin >> id;
            inventory.remove_product(id);
            cout << "Product removed from inventory.\n";
        } else if (option == 3) {
            string id;
            int quantity;
            double price;
            cout << "Enter product ID: ";
            cin >> id;
            cout << "Enter new quantity: ";
            cin >> quantity;
            cout << "Enter new price: ";
            cin >> price;
            inventory.update_product(id, quantity, price);
            cout << "Product updated in inventory.\n";
        } else if (option == 4) {
            inventory.display_inventory();
        } else if (option == 5) {
            string customer_name, customer_email;
            int num_products;
            cout << "Enter customer name: ";
            cin.ignore();
            getline(cin, customer_name);
            cout << "Enter customer email: ";
            getline(cin, customer_email);
            cout << "Enter number of products: ";
            cin >> num_products;
            Customer customer(customer_name, customer_email);
            Invoice invoice(customer);
            for (int i = 0; i < num_products; i++) {
                string id;
                cout << "Enter product ID: ";
                cin >> id;
                int quantity;
                cout << "Enter quantity: ";
                cin >> quantity;
                for (const auto& product : inventory.products) {
                    if (product.get_id() == id) {
                        Product invoice_product(product.get_id(), product.get_name(), quantity, product.get_price());
                        invoice.add_product(invoice_product);
                        inventory.update_product(id, product.get_quantity() - quantity, product.get_price());
                        break;
                    }
                }
            }
            invoice.issue_invoice();
            invoice.save_invoice_to_file("invoice.csv");
            inventory.save_inventory_to_file("inventory.txt");
        } else if (option == 6) {
            cout << "Exiting program.\n";
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
