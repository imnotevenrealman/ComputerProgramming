#include <iostream>
#include <string>
#include <ctime>
#include <limits>
#include <sstream>
#include <algorithm> // Include the algorithm header for 'transform'

using namespace std;

struct BankAccount {
    string firstName, lastName, middleName, address, birthday, gender, accounttype, accNumber;
    long balance, Totalbalance; 
    double deposit, withdraw;
    int pin, testpin;
};

static int balanceamount = 0;
static double initialdeposit = 0;
BankAccount userAccount;

void initializeRandomSeed() {
    srand(static_cast<unsigned int>(time(0)));
}

string random16DigitAccNo() {
    string accNumber;
    for (int i = 0; i < 16; ++i) {
        if (i > 0 && i % 4 == 0) {
            accNumber += '-';
        }
        accNumber += to_string(rand() % 10); // Generate a random digit (0-9)
    }
    return accNumber;
}

// Function to calculate age based on birthdate
int calculateAge(const string& birthDate) {
    // Get current time
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;
    int currentDay = currentTime->tm_mday;

    // Parse birthDate string to extract year, month, and day
    std::istringstream ss(birthDate);
    int birthYear, birthMonth, birthDay;
    char discard;

    if (!(ss >> birthDay >> discard >> birthMonth >> discard >> birthYear)) {
        cout << "Invalid date format." << endl;
        return -1; // Return -1 for invalid date
    }

    int age = currentYear - birthYear;

    // Check if the birthday has passed this year
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }

    return age;
}

// Function to validate the date format (DD/MM/YYYY)
bool isValidDateFormat(const string& date) {
    if (date.length() != 10) {
        return false;
    }
    
    if (date[2] != '/' || date[5] != '/') {
        return false;
    }
    
    for (int i = 0; i < 10; ++i) {
        if (i != 2 && i != 5 && !isdigit(date[i])) {
            return false;
        }
    }
    
    return true;
}

BankAccount createAccount() {
    BankAccount account;
    account.accNumber = random16DigitAccNo();

    cout << "\nEnter your First Name: ";
    cin.ignore();
    getline(cin, account.firstName);

    cout << "Enter your Last Name: ";
    getline(cin, account.lastName);

    cout << "Enter your Middle Name: ";
    getline (cin, account.middleName);

    cout << "\nEnter your Address: ";
    getline (cin, account.address);

    bool validDate = false;
    while (!validDate) {
        cout << "\nEnter Birthday (DD/MM/YYYY): ";
        getline(cin, account.birthday);

        if (!isValidDateFormat(account.birthday)) {
            cout << "Invalid date format. Please enter the date in DD/MM/YYYY format." << endl;
        } else {
            validDate = true;
        }
    }

    // Calculate age based on birthday
    int age = calculateAge(account.birthday);
    cout << "Your age: " << age << " years old." << endl;

    // Check if the user is underage
    if (age < 18) {
        cout << "\nSorry, you must be at least 18 years old to create an account." << endl;
        cout << "Account creation process aborted." << endl;
        
        char response;
        do {
            cout << "\nWould you like to go back to the menu? (Y to return, any other key to exit): ";
            cin >> response;

            if (toupper(response) == 'Y') {
                // User wants to return to the menu
                return account;
            } else {
                // User wants to exit the program
                cout << "\nGoodbye.\n";
                exit(0); // Exit the program
            }
        } while (true);
    }

    bool validInputGender = false;

    while (!validInputGender) {
        cout << "\nEnter your sex (M/F): ";
        getline(cin, account.gender);

        // Convert input to uppercase
        transform(account.gender.begin(), account.gender.end(), account.gender.begin(), ::toupper);

        if (account.gender == "M" || account.gender == "F") {
            validInputGender = true;
        } 
        else {
            cout << "\nInvalid input. Please enter 'M' for Male or 'F' for Female: ";
        }
    }


    bool validInput = false;

    while (!validInput) {
        cout << "\nEnter Account Type:\nSA - Savings Account\nCA - Current Account\n\nEnter Account Type: ";
        string accountTypeInput;
        getline(cin, accountTypeInput);

        // Convert input to uppercase
        transform(accountTypeInput.begin(), accountTypeInput.end(), accountTypeInput.begin(), ::toupper);

        if (accountTypeInput == "SA" || accountTypeInput == "CA") {
            account.accounttype = accountTypeInput;
            validInput = true;

            int minimumDeposit = (accountTypeInput == "SA") ? 5000 : 10000;
            cout << "\nMinimum Initial Deposit: " << minimumDeposit << " - " << accountTypeInput << endl;

            bool validInitialDeposit = false;
            while (!validInitialDeposit) {
                cout << "Enter initial deposit: ";
                cin >> initialdeposit;

                if (initialdeposit >= minimumDeposit) {
                    validInitialDeposit = true;
                } else {
                    cout << "\nInvalid deposit amount. Please enter a deposit amount greater than or equal to the minimum required." << endl;
                }
            }
        } 
    else {
        cout << "\nInvalid input. Please enter 'SA' for Savings Account or 'CA' for Current Account: ";
    }
    }


    // PIN validation
    bool validPIN = false;
    while (!validPIN) {
        cout << "\nEnter 6-Digit PIN: ";
        cin >> account.pin;

        if (cin.fail() || to_string(account.pin).length() != 6) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input buffer
            cout << "Invalid PIN format. Please enter a 6-digit PIN." << endl;
        } 
        else
            validPIN = true;
    }

    // Re-enter PIN and verify it matches the first entry
    validPIN = false;
    while (!validPIN) {
        cout << "Re-enter 6-Digit PIN: ";
        cin >> account.testpin;

        if (cin.fail() || to_string(account.testpin).length() != 6) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input buffer
            cout << "Invalid PIN format. Please enter a 6-digit PIN." << endl;
        } else if (account.testpin != account.pin) {
            cout << "PIN does not match. Please re-enter the same 6-Digit PIN." << endl;
        } 
        else
            validPIN = true;
    }
    account.balance = initialdeposit;

    cout << "\nAccount created." << endl;
    cout << "Account Name: " << account.lastName << ", " << account.firstName << ", " << account.middleName << endl;
    cout << "Account Number: " << account.accNumber << endl;
    cout << "PIN: " << account.pin << endl;
    cout << "Balance: " << account.balance << endl;

    char response;
    do {
        cout << "\nWould you like to go back to the menu? (Y to return, any other key to exit): ";
        cin >> response;

        if (toupper(response) == 'Y') {
            // User wants to return to the menu
            return account;
        } else {
            // User wants to exit the program
            cout << "\nGoodbye.\n";
            exit(0); // Exit the program
        }
    } while (true);
}

void balanceInquiry(const BankAccount& account) {
    if (account.accNumber.empty()) {
        cout << "\nYou do not have an existing account. Please create an account first." << endl;
        return; // Return if there's no existing account
    }

    int enteredPIN;
    char response;

    do {
        cout << "\nEnter your 6-Digit PIN to view the balance: ";

        // Check if the entered input is an integer
        if (!(cin >> enteredPIN)) {
            cout << "Invalid input. Please enter a numerical PIN." << endl;
            cin.clear(); // Clear error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Prompt the user again for input
        }

        if (enteredPIN == account.pin) {
            cout << "\nAccount Number: " << account.accNumber << endl;
            cout << "Balance: " << account.balance << endl;

            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

            do {
                cout << "\nWould you like to go back to the menu? (Y to return, any other key to exit): ";
                cin >> response;

                if (toupper(response) == 'Y') {
                    // User wants to return to the menu
                    return;
                } else {
                    // User wants to exit the program
                    cout << "\nGoodbye.\n";
                    exit(0); // Exit the program
                }
            } while (true);
        } else {
            cout << "Invalid PIN. Access denied." << endl;
        }
    } while (true);
}


void deposit(BankAccount& account) {
    if (account.accNumber.empty()) {
        cout << "\nYou do not have an existing account. Please create an account first." << endl;
        return; // Return if there's no existing account
    }

    int enteredPIN;
    char response;
    bool validPIN = false;

    do {
        cout << "\nEnter your 6-Digit PIN to deposit: ";
        if (!(cin >> enteredPIN)) {
            cout << "Invalid input. Please enter a numeric PIN." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; // Ask for PIN again if non-numeric input is entered
        }

        if (enteredPIN != account.pin) {
            cout << "Invalid PIN. Access denied." << endl;
            cout << "Would you like to try again? (Y/N): ";
            cin >> response;
            response = toupper(response); // Convert response to uppercase

            if (response == 'N') {
                return; // Return to the menu if the user chooses not to try again
            }
        } else {
            validPIN = true;
        }
    } while (!validPIN);

    cout << "\nAccount Number: " << account.accNumber << endl;
    cout << "Minimum deposit is " << ((account.accounttype == "SA") ? "Php 300." : "Php 500.") << endl;
    cout << "\nEnter deposit: ";
    if (!(cin >> account.deposit)) {
        cout << "Invalid input. Please enter a numeric amount." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return; // Return if non-numeric input is entered for deposit
    }

    if ((account.accounttype == "SA" && account.deposit < 300) ||
        (account.accounttype == "CA" && account.deposit < 500)) {
        cout << "Insufficient deposit. Please try again." << endl;
        return; // Return if deposit is insufficient
    }

    bool validResponse = false;
    while (!validResponse) {
        cout << "Please confirm deposit (Y/N): ";
        cin >> response;
        response = toupper(response); // Convert response to uppercase

        if (response == 'Y') {
            cout << "Successfully deposited!" << endl;
            account.Totalbalance = account.deposit + account.balance;
            account.balance = account.Totalbalance;
            validResponse = true;
        } else if (response == 'N') {
            cout << "Invalid input. Please try again." << endl;
            validResponse = true;
        } else {
            cout << "Invalid input. Please enter 'Y' or 'N'." << endl;
        }
    }
}

void withdraw(BankAccount& account) {
    if (account.accNumber.empty()) {
        cout << "\nYou do not have an existing account. Please create an account first." << endl;
        return;
    }

    int enteredPIN;
    char response;

    do {
        cout << "\nEnter your 6-Digit PIN to withdraw: ";

        if (!(cin >> enteredPIN)) {
            cout << "Invalid input. Please enter a numeric PIN." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nWould you like to try again? (Y to retry, any other key to return to menu): ";
            cin >> response;

            if (toupper(response) != 'Y') {
                cout << "\nGoodbye.\n";
                return;
            }
        } else {
            if (enteredPIN == account.pin) {
                cout << "Account Number: " << account.accNumber << endl;

                int minWithdraw = (account.accounttype == "SA") ? 300 : 500;

                cout << "Minimum withdraw is Php " << minWithdraw << endl;
                cout << "\nEnter withdrawal amount: ";
                long withdrawalAmount;
                cin >> withdrawalAmount;

                if (withdrawalAmount < minWithdraw) {
                    cout << "Insufficient withdrawal amount. The minimum withdrawal amount is "
                         << "Php " << minWithdraw << "." << endl;
                    cout << "\nWould you like to try again? (Y to retry, any other key to return to menu): ";
                    cin >> response;

                    if (toupper(response) != 'Y') {
                        cout << "\nGoodbye.\n";
                        return;
                    }
                } else if (withdrawalAmount > account.balance) {
                    cout << "Insufficient funds. Your account balance is not enough for this withdrawal." << endl;
                    cout << "\nWould you like to try again? (Y to retry, any other key to return to menu): ";
                    cin >> response;

                    if (toupper(response) != 'Y') {
                        cout << "\nGoodbye.\n";
                        return;
                    }
                } else {
                    cout << "Please confirm withdrawal of Php " << withdrawalAmount << " (Y/N): ";
                    cin >> response;

                    if (toupper(response) == 'Y') {
                        cout << "Successfully withdrew Php " << withdrawalAmount << "!" << endl;
                        account.balance -= withdrawalAmount;

                        cout << "\nWould you like to return to the menu? (Y to return, any other key to close the program): ";
                        cin >> response;

                        if (toupper(response) != 'Y') {
                            cout << "\nGoodbye.\n";
                            return;
                        } else {
                            // Returning to the menu
                            return;
                        }
                    } else {
                        cout << "Withdrawal canceled." << endl;
                        cout << "\nWould you like to return to the menu? (Y to return, any other key to close the program): ";
                        cin >> response;

                        if (toupper(response) != 'Y') {
                            cout << "\nGoodbye.\n";
                            return;
                        } else {
                            // Returning to the menu
                            return;
                        }
                    }
                }
            } else {
                cout << "Invalid PIN. Access denied." << endl;
                cout << "\nWould you like to try again? (Y to retry, any other key to return to menu): ";
                cin >> response;

                if (toupper(response) != 'Y') {
                    cout << "\nGoodbye.\n";
                    return;
                }
            }
        }
    } while (true);
}

void viewAccount(const BankAccount& account) {
    if (account.accNumber.empty()) {
        cout << "\nYou do not have an existing account. Please create an account first." << endl;
        return;
    }

    int enteredPIN;
    char response;
    bool validPIN = false;

    do {
        cout << "\nEnter your 6-Digit PIN to view account information: ";
        cin >> enteredPIN;

        if (enteredPIN != account.pin) {
            cout << "Invalid PIN. Access denied." << endl;
            cout << "Would you like to try again? (Y/N): ";
            cin >> response;
            response = toupper(response);

            if (response == 'N') {
                return;
            }
        } else {
            validPIN = true;
        }
    } while (!validPIN);

    cout << "\nAccount number: " << account.accNumber << endl;
    cout << "Full Name: " << account.lastName << ", " << account.firstName << ", " << account.middleName << endl;
    cout << "Address: " << account.address << endl;
    cout << "Birthday: " << account.birthday << endl;
    cout << "Gender: " << account.gender << endl;
    cout << "Account Type: " << account.accounttype << endl;
    cout << "Initial Deposit: " << initialdeposit << endl;  // Note: Use 'initialdeposit' instead of 'account.initialdeposit'
    cout << "Current Balance: " << account.balance << endl;

    // Ask if the user wants to return to the menu or exit the program
    cout << "\nWould you like to return to the menu? (Y to return, any other key to exit the program): ";
    cin >> response;

    if (toupper(response) != 'Y') {
        cout << "\nGoodbye.\n";
        exit(0);
    }
}


void closeAccount(BankAccount& account) {
    if (account.accNumber.empty()) {
        cout << "\nYou do not have an existing account. Please create an account first." << endl;
        return;
    }

    int enteredPIN;
    char response;
    bool validPIN = false;
    bool accountClosed = false; // Flag to indicate if the account is closed

    do {
        cout << "\nEnter your 6-Digit PIN to close account: ";
        if (!(cin >> enteredPIN)) {
            cout << "Invalid input. Please enter a numeric PIN." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (enteredPIN != account.pin) {
            cout << "Invalid PIN. Access denied." << endl;
            cout << "Would you like to try again? (Y/N): ";
            cin >> response;
            response = toupper(response);

            if (response == 'N') {
                return;
            }
        } else {
            validPIN = true;
        }
    } while (!validPIN);

    cout << "Confirm to close account? (Y/N): ";
    cin >> response;
    response = toupper(response);

    if (response == 'N') {
        cout << "Account closure canceled." << endl;
        // Ask if they want to return to the menu or exit the program
        cout << "\nWould you like to go back to the menu? (Y to return, any other key to exit): ";
        cin >> response;

        if (toupper(response) == 'Y') {
            // User wants to return to the menu
            return;
        } else {
            // User wants to exit the program
            cout << "\nGoodbye.\n";
            exit(0); // Exit the program
        }
    } 
    
    else {
        accountClosed = true; // For demonstration purposes

        if (accountClosed) {
            cout << "Account closed successfully." << endl;
            cout << "----------------------------------" << endl;
            cout << "Account Closed Receipt" << endl;
            cout << "----------------------------------" << endl;
            cout << "Account Number: " << account.accNumber << endl;
            cout << "Account Holder: " << account.lastName << ", " << account.firstName << ", " << account.middleName << endl;
            cout << "\nThank you for choosing Communist Frogs' bank!\nFor inquiries, please contact Froggy Services: 9-987-765-6543\nVisit our website: www.communistfrogs.com" << endl;

            // After closing the account, ask the user if they want to return to the menu or exit the program
            cout << "\nWould you like to go back to the menu? (Y to return, any other key to exit): ";
            cin >> response;

            if (toupper(response) == 'Y') {
                // User wants to return to the menu
                return;
            } else {
                // User wants to exit the program
                cout << "\nGoodbye.\n";
                exit(0); // Exit the program
            }
        } else {
            cout << "Failed to close the account. Please try again later." << endl;
        }
    }
}



void Menu(){
    bool accountCreated = false;
    int choice, new_choice;

    do {
        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━✁┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈" << endl;
        cout << "\nWelcome to The Communist Frogs' bank. да здравствует лягушка" << endl;
        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━✁┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n" << endl;

        cout << "Menu" << endl;
        cout << "1) Open a New Account" << endl;
        cout << "2) Balance Inquiry" << endl;
        cout << "3) Deposit" << endl;
        cout << "4) Withdraw" << endl;
        cout << "5) View Account Information" << endl;
        cout << "6) Close Account" << endl;
        cout << "7) Exit.\n" << endl;
        cout << "Choose an option (1-7): ";
        cin >> choice;

        switch (choice) {
            case 1:
                userAccount = createAccount();
                accountCreated = true;
                break;

            case 2:
                balanceInquiry(userAccount);
                break;
                
            case 3: 
                deposit(userAccount);
                break;
                
            case 4:
                withdraw(userAccount);
                break; 
                
            case 5:
                viewAccount(userAccount);
                break;
                
            case 6:
                closeAccount(userAccount);
                break; 
                
            case 7:
                cout << "\nThank you for choosing Communist Frogs' bank!" << endl;
                cout << "Members: " << endl;
                cout << "Alintajan, Ryan Robeck";
                cout << "\nDugan, Andrea";
                cout << "\nDe Jose, Wim Robert";
                cout << "\nJapon, Althaea";
                break;
                
            default:
                cout << "Invalid input. Please enter your choice.";
                cin >> new_choice;
                choice = new_choice;
                break;
        }
    } 
    while (choice !=7);
}

int main() {
    initializeRandomSeed();
    Menu();
    return 0;
}
