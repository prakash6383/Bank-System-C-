#include <iostream>
#include <fstream>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ios;

// Class Definition
class account_query {
private:
    char account_number[20];
    char firstName[10];
    char lastName[10];
    float total_Balance;

public:
    void read_data();
    void show_data();
    void write_rec();
    void read_rec();
    void search_rec();
    void edit_rec();
    void delete_rec();
};

// Member Function Definitions
void account_query::read_data() {
    cout << "Enter Account Number: ";
    cin >> account_number;
    cout << "Enter First Name: ";
    cin >> firstName;
    cout << "Enter Last Name: ";
    cin >> lastName;
    cout << "Enter Balance: ";
    cin >> total_Balance;
    cout << endl;
}

void account_query::show_data() {
    cout << "Account Number: " << account_number << endl;
    cout << "First Name: " << firstName << endl;
    cout << "Last Name: " << lastName << endl;
    cout << "Current Balance: Rs. " << total_Balance << endl;
    cout << "------------------------------------" << endl;
}

void account_query::write_rec() {
    ofstream outfile("record.bank", ios::binary | ios::app);
    if (!outfile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    read_data();
    outfile.write(reinterpret_cast<char *>(this), sizeof(*this));
    outfile.close();
    cout << "Record added successfully!" << endl;
}

void account_query::read_rec() {
    ifstream infile("record.bank", ios::binary);
    if (!infile) {
        cout << "Error opening file! File not found!" << endl;
        return;
    }
    cout << "\n***** Data from File *****\n";
    while (infile.read(reinterpret_cast<char *>(this), sizeof(*this))) {
        show_data();
    }
    infile.close();
}

void account_query::search_rec() {
    int n, total_records;
    ifstream infile("record.bank", ios::binary);
    if (!infile) {
        cout << "Error opening file! File not found!" << endl;
        return;
    }

    infile.seekg(0, ios::end);
    total_records = infile.tellg() / sizeof(*this);
    if (total_records == 0) {
        cout << "No records found!" << endl;
        infile.close();
        return;
    }

    cout << "\nThere are " << total_records << " records in the file.";
    cout << "\nEnter Record Number to Search: ";
    cin >> n;

    if (n < 1 || n > total_records) {
        cout << "Invalid record number!" << endl;
        infile.close();
        return;
    }

    infile.seekg((n - 1) * sizeof(*this));
    infile.read(reinterpret_cast<char *>(this), sizeof(*this));
    cout << "\nRecord " << n << " details:\n";
    show_data();
    infile.close();
}

void account_query::edit_rec() {
    int n, total_records;
    fstream iofile("record.bank", ios::in | ios::out | ios::binary);
    if (!iofile) {
        cout << "Error opening file! File not found!" << endl;
        return;
    }

    iofile.seekg(0, ios::end);
    total_records = iofile.tellg() / sizeof(*this);
    if (total_records == 0) {
        cout << "No records found!" << endl;
        iofile.close();
        return;
    }

    cout << "\nThere are " << total_records << " records in the file.";
    cout << "\nEnter Record Number to Edit: ";
    cin >> n;

    if (n < 1 || n > total_records) {
        cout << "Invalid record number!" << endl;
        iofile.close();
        return;
    }

    iofile.seekg((n - 1) * sizeof(*this));
    iofile.read(reinterpret_cast<char *>(this), sizeof(*this));
    cout << "Record " << n << " has the following data:\n";
    show_data();

    cout << "Enter new data for the record:\n";
    read_data();

    iofile.seekp((n - 1) * sizeof(*this));
    iofile.write(reinterpret_cast<char *>(this), sizeof(*this));
    iofile.close();
    cout << "Record updated successfully!" << endl;
}

void account_query::delete_rec() {
    int n, total_records;
    ifstream infile("record.bank", ios::binary);
    if (!infile) {
        cout << "Error opening file! File not found!" << endl;
        return;
    }

    infile.seekg(0, ios::end);
    total_records = infile.tellg() / sizeof(*this);
    if (total_records == 0) {
        cout << "No records found!" << endl;
        infile.close();
        return;
    }

    cout << "\nThere are " << total_records << " records in the file.";
    cout << "\nEnter Record Number to Delete: ";
    cin >> n;

    if (n < 1 || n > total_records) {
        cout << "Invalid record number!" << endl;
        infile.close();
        return;
    }

    ofstream tmpfile("tmpfile.bank", ios::binary);
    infile.seekg(0);
    for (int i = 0; i < total_records; ++i) {
        infile.read(reinterpret_cast<char *>(this), sizeof(*this));
        if (i == (n - 1))
            continue; // Skip the record to delete
        tmpfile.write(reinterpret_cast<char *>(this), sizeof(*this));
    }

    infile.close();
    tmpfile.close();
    remove("record.bank");
    rename("tmpfile.bank", "record.bank");
    cout << "Record deleted successfully!" << endl;
}

// Main Function
int main() {
    account_query A;
    int choice;

    while (true) {
        cout << "\n***** Account Information System *****\n";
        cout << "1 -> Add Record\n";
        cout << "2 -> Show All Records\n";
        cout << "3 -> Search Record\n";
        cout << "4 -> Update Record\n";
        cout << "5 -> Delete Record\n";
        cout << "6 -> Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            A.write_rec();
            break;
        case 2:
            A.read_rec();
            break;
        case 3:
            A.search_rec();
            break;
        case 4:
            A.edit_rec();
            break;
        case 5:
            A.delete_rec();
            break;
        case 6:
            cout << "Exiting... Goodbye!" << endl;
            exit(0);
        default:
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
