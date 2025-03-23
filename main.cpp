// Garrett, Brunsch
// Lab #5 Pointers & Sort S25
// CS_136_14728SP25P
// Due 3/23/25

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
using namespace std;

enum class MainMenuChoice
{
    PRINT_ORIGINAL_INVENTORY = 1,
    SEARCH_CAR,
    SORT_INVENTORY,
    PRINT_INVALID_RECORDS,
    QUIT
};

enum class SearchMenuChoice
{
    SEARCH_BY_MODEL_MANUFACTURER = 1,
    SEARCH_BY_PRICE_RANGE,
    BACK_TO_MAIN_MENU
};

enum class SortMenuChoice
{
    SORT_BY_CAR_ID = 1,
    SORT_BY_MODEL,
    SORT_BY_MANUFACTURER,
    SORT_BY_QUANTITY,
    SORT_BY_PRICE,
    PRINT_CURRENT_SORT,
    BACK_TO_MAIN_MENU_FROM_SORT
};

enum class PrintOrder
{
    ORIGINAL = 1,
    SORTED
};

void displayMainMenu();
void displaySearchMenu();
void displaySortMenu();
void clearInputError(const string& errorMessage);
string toLower(const string& str);
class Inventory; // Forward declaration needed here so handleSearchMenu can use it
void handleSearchMenu(const Inventory& inventory);

class Car
{
private:
    string carId = "N/A";
    string carModel = "N/A";
    string carManufacturer = "N/A";
    int carQuantity = 0;
    double carPrice = 0.0;

    bool validationStatus = true;
    string errorMessage = "n/a";

    static const int MAX_ID_LENGTH = 8;
    static const int MIN_MODEL_LENGTH = 3;
    static const double MIN_PRICE;

    bool isValidId(const string& carId, string& errorMessage);
    bool isValidCarModel(const string& carModel, string& errorMessage);
    bool isValidCarPrice(const double& carPrice, string& errorMessage);
    bool isValidQuantity(const int& carQuantity, string& errorMessage);

public:
    Car();
    Car(const string& carId, const string& carModel, const string& carManufacturer, int carQuantity, double carPrice);

    void validateRecord();
    string toString() const;
    bool isValid() const { return validationStatus; }
    string getErrorMessage() const { return errorMessage; }

    string getId() const { return carId; }
    string getModel() const { return carModel; }
    string getManufacturer() const { return carManufacturer; }
    int getQuantity() const { return carQuantity; }
    double getPrice() const { return carPrice; }

    bool isLowInventory() const { return carQuantity < 5; }
};

class Inventory
{
private:
    static const int MAX_RECORDS = 25;
    Car carStorage[MAX_RECORDS];
    Car* cars[MAX_RECORDS]; // Pointers used to make sorting more efficient later. This implementation makes it so that only the pointers need to be swapped for sorting rather than the entire data set
    int recordCount = 0;
    static const string INPUT_FILENAME;
    static const string ERROR_FILENAME;
    static const string LOW_INVENTORY_FILENAME;

    bool checkAndOpenFiles(ifstream& inFile, ofstream& errorFile) const;
    void processInputFile(ifstream& inFile, ofstream& errorFile);
    void processLine(const string& line, ofstream& errorFile, bool& continueProcessing);

public:
    Inventory();

    bool readInventory();
    void printInventory(PrintOrder order = PrintOrder::SORTED) const;
    void printSortFieldMessage(SortMenuChoice choice) const;
    void printErrorFile() const;
    void printLowInventoryToFile() const;
    static void printHeader(bool includeErrorColumn = false);

    void searchByModelOrManufacturer() const;
    void searchByPriceRange() const;

    void sortMenu();
    void sortInventory(int sortField);
};

int main()
{
    int mainChoiceInt = 0;
    Inventory inventory;
    bool initSuccess = inventory.readInventory();

    if (initSuccess)
    {
        do
        {
            displayMainMenu();
            cin >> mainChoiceInt;
            MainMenuChoice mainChoice = static_cast<MainMenuChoice>(mainChoiceInt);

            switch (mainChoice)
            {
            case MainMenuChoice::PRINT_ORIGINAL_INVENTORY:
                inventory.printInventory(PrintOrder::ORIGINAL);
                break;
            case MainMenuChoice::SEARCH_CAR:
                handleSearchMenu(inventory);
                break;
            case MainMenuChoice::SORT_INVENTORY:
                inventory.sortMenu();
                break;
            case MainMenuChoice::PRINT_INVALID_RECORDS:
                inventory.printErrorFile();
                break;
            case MainMenuChoice::QUIT:
                inventory.printLowInventoryToFile();
                cout << "Now exiting program...\n\n";
                break;
            default:
                clearInputError("Invalid menu choice. Please enter a number between 1 and " + to_string(static_cast<int>(MainMenuChoice::QUIT)));
                break;
            }
        } while (mainChoiceInt != static_cast<int>(MainMenuChoice::QUIT));
    }

    return initSuccess ? 0 : 1;
}

void displayMainMenu()
{
    cout << "\n\n --- Main Menu ---\n"
        "1. Print Original Inventory\n"
        "2. Search Card Records\n"
        "3. Sort Inventory\n"
        "4. Print Invalid Records\n"
        "5. Quit\n"
        "Enter choice: ";
}

void displaySearchMenu()
{
    cout << "\n --- Search Options ---\n"
        "1. Model or Manufacturer\n"
        "2. Price\n"
        "3. Return to Main Menu\n"
        "Enter choice: ";
}

void displaySortMenu()
{
    cout << "\n --- Sort Options ---\n"
        "1. Car ID\n"
        "2. Model\n"
        "3. Manufacturer\n"
        "4. Quantity\n"
        "5. Price\n"
        "6. Print Sorted Inventory\n" // I'm not sure if this was necessary, but I separated my print option for better encapsulation. The previous version still utilized different function calls, but now it's entirely separate
        "7. Return to Main Menu\n"
        "Enter choice: ";
}

string toLower(const string& str)
{
    string result = str;
    for (int i = 0; i < result.length(); i++)
    {
        result[i] = tolower(result[i]);
    }
    return result;
}

void clearInputError(const string& errorMessage)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << errorMessage << "\n";
}

void handleSearchMenu(const Inventory& inventory)
{
    int searchChoiceInt = 0;

    do
    {
        displaySearchMenu();
        cin >> searchChoiceInt;
        SearchMenuChoice searchChoice = static_cast<SearchMenuChoice>(searchChoiceInt); 

        switch (searchChoice)
        {
        case SearchMenuChoice::SEARCH_BY_MODEL_MANUFACTURER:
            inventory.searchByModelOrManufacturer();
            break;
        case SearchMenuChoice::SEARCH_BY_PRICE_RANGE:
            inventory.searchByPriceRange();
            break;
        case SearchMenuChoice::BACK_TO_MAIN_MENU:
            cout << "Returning to main menu...\n";
            break;
        default:
            clearInputError("Invalid search choice. Please enter a number between 1 and " + to_string(static_cast<int>(SearchMenuChoice::BACK_TO_MAIN_MENU)));
            break;
        }
    } while (searchChoiceInt != static_cast<int>(SearchMenuChoice::BACK_TO_MAIN_MENU));
}

const string Inventory::INPUT_FILENAME = "inventory.txt";
const string Inventory::ERROR_FILENAME = "errors.txt";
const string Inventory::LOW_INVENTORY_FILENAME = "low_inventory.txt";
const double Car::MIN_PRICE = 12999.00;

Car::Car() : carId("N/A"), carModel("N/A"), carManufacturer("N/A"), carQuantity(0), carPrice(0.0), validationStatus(true), errorMessage("n/a") {}

Car::Car(const string& id, const string& model, const string& manufacturer, int quantity, double price)
    :carId(id), carModel(model), carManufacturer(manufacturer), carQuantity(quantity), carPrice(price), validationStatus(true), errorMessage("n/a") {}

string Car::toString() const
{
    //All numerical values must be right aligned to adhere to assignment instructions
    stringstream ss;
    ss << left << setw(10) << carId
        << left << setw(20) << carModel
        << left << setw(20) << carManufacturer
        << right << setw(8) << carQuantity
        << right << setw(12) << fixed << setprecision(2) << carPrice;

    if (isLowInventory())
    {
        ss << "   *LOW INVENTORY*";
    }

    return ss.str();
}

void Car::validateRecord()
{
    validationStatus = true;
    errorMessage.clear();

    isValidId(carId, errorMessage);
    isValidCarModel(carModel, errorMessage);
    isValidCarPrice(carPrice, errorMessage);
    isValidQuantity(carQuantity, errorMessage);
}

bool Car::isValidId(const string& carId, string& errorMessage)
{
    if (carId.length() != MAX_ID_LENGTH)
    {
        errorMessage += "ID must be 8 characters; ";
        validationStatus = false;
    }

    for (int i = 0; i < (MAX_ID_LENGTH / 2); i++)
    {
        if (!isalpha(carId[i]) || carId[i] == 'O' || carId[i] == 'o')
        {
            errorMessage += "First 4 characters must be alpha only (excluding O); ";
            validationStatus = false;
        }
    }

    for (int i = (MAX_ID_LENGTH / 2); i < MAX_ID_LENGTH; i++)
    {
        if (carId[i] < '1' || carId[i] > '9')
        {
            errorMessage += "Second set of 4 characters must be 1-9; ";
            validationStatus = false;
        }
    }
    return validationStatus;
}

bool Car::isValidCarModel(const string& carModel, string& errorMessage)
{
    if (carModel.length() < MIN_MODEL_LENGTH)
    {
        errorMessage += "Model contains less than minimum required characters; ";
        validationStatus = false;
    }
    if (!isalpha(carModel[0]))
    {
        errorMessage += "Model does not start with an alpha character; ";
        validationStatus = false;
    }
    for (int i = 0; i < carModel.length(); i++)
    {
        if (!isalnum(carModel[i]) && carModel[i] != '_')
        {
            errorMessage += "Model contains non-alphanumeric character; ";
            validationStatus = false;
        }
    }
    return validationStatus;
}

bool Car::isValidCarPrice(const double& carPrice, string& errorMessage)
{
    if (carPrice < MIN_PRICE)
    {
        errorMessage += "Price must be above minimum price; ";
        validationStatus = false;
    }
    return validationStatus;
}

bool Car::isValidQuantity(const int& carQuantity, string& errorMessage)
{
    if (carQuantity < 0)
    {
        errorMessage += "Quantity must be at least 0; ";
        validationStatus = false;
    }
    return validationStatus;
}

void Inventory::printHeader(bool includeErrorColumn)
{
    cout << left << setw(10) << "Car ID"
        << left << setw(20) << "Model"
        << left << setw(20) << "Manufacturer"
        << right << setw(8) << "Qty"
        << right << setw(12) << "Price";

    if (includeErrorColumn)
    {
        cout << left << setw(30) << "   Error Message" // Three spaces added here to compensate for " - " format in table rows
            << "\n" << string(100, '-') << "\n";
    }
    else
    {
        cout << "\n" << string(70, '-') << "\n";
    }
}

bool Inventory::checkAndOpenFiles(ifstream& inFile, ofstream& errorFile) const
{
    bool filesOpen = true;

    inFile.open(INPUT_FILENAME);
    if (!inFile)
    {
        cout << "Error opening " << INPUT_FILENAME <<
            "\nDouble check the file exists in the proper directory and try again\n"
            "Program now exiting...\n";
        filesOpen = false;
    }

    if (filesOpen)
    {
        errorFile.open(ERROR_FILENAME);
        if (!errorFile)
        {
            inFile.close();
            cout << "Error opening " << ERROR_FILENAME <<
                "\nDouble check the file exists in the proper directory and try again\n"
                "Program now exiting...\n";
            filesOpen = false;
        }
    }

    return filesOpen;
}

void Inventory::processLine(const string& line, ofstream& errorFile, bool& continueProcessing)
{

    if (recordCount < MAX_RECORDS && !line.empty())
    {
        istringstream iss(line);
        string tempId{}, tempModel{}, tempManufacturer{}, quantityStr{}, priceStr{};

        if (iss >> tempId >> tempModel >> tempManufacturer >> quantityStr >> priceStr)
        {
            int tempQuantity = 0;
            double tempPrice = 0.0;
            bool validQuantity = true;
            bool validPrice = true;
            string additionalError = "";

            try
            {
                tempQuantity = stoi(quantityStr);
            }
            catch (...)
            {
                validQuantity = false;
                additionalError += "Invalid quantity format; ";
            }

            try
            {
                tempPrice = stod(priceStr);
            }
            catch (...)
            {
                validPrice = false;
                additionalError += "Invalid price format; ";
            }

            Car tempCar(tempId, tempModel, tempManufacturer, tempQuantity, tempPrice);
            tempCar.validateRecord();

            if (validQuantity && validPrice && tempCar.isValid())
            {
                carStorage[recordCount] = tempCar;
                cars[recordCount] = &carStorage[recordCount];
                recordCount++;
            }
            else
            {
                string errorMsg = tempCar.getErrorMessage() + additionalError;
                errorFile << tempCar.toString() << " - " << errorMsg << "\n";
            }
        }
        else
        {
            cout << "Warning: Invalid format in line: " << line << "\n";
        }
    }
    else if (recordCount >= MAX_RECORDS)
    {
        continueProcessing = false;
    }
}

void Inventory::processInputFile(ifstream& inFile, ofstream& errorFile)
{
    recordCount = 0;
    string line;
    bool continueProcessing = true;

    while (continueProcessing && getline(inFile, line))
    {
        processLine(line, errorFile, continueProcessing);
    }

    if (recordCount >= MAX_RECORDS && inFile.peek() != EOF)
    {
        cout << "Max record limit of " << MAX_RECORDS << " has been reached\n";
    }
}

Inventory::Inventory() : recordCount(0)
{
    for (int i = 0; i < MAX_RECORDS; i++)
    {
        cars[i] = nullptr;
    }
}

bool Inventory::readInventory()
{
    ifstream inFile;
    ofstream errorFile;
    bool success = true;

    if (checkAndOpenFiles(inFile, errorFile))
    {
        if (inFile.peek() == EOF)
        {
            cout << "Input file is empty\n";
            success = false;
        }
        else
        {
            processInputFile(inFile, errorFile);
        }

        inFile.close();
        errorFile.close();
    }
    else
    {
        success = false;
    }

    return success;
}

void Inventory::printInventory(PrintOrder order) const
{
    cout << "\n" << (order == PrintOrder::ORIGINAL ? "Original" :"Current Inventory") << " Inventory\n";
    printHeader();

    if (order == PrintOrder::ORIGINAL)
    {
        for (int i = 0; i < recordCount; i++)
        {
            cout << carStorage[i].toString() << "\n";
        }
    }
    else
    {
        for (int i = 0; i < recordCount; i++)
        {
            cout << cars[i]->toString() << "\n";
        }
    }

}

void Inventory::printSortFieldMessage(SortMenuChoice choice) const
{
    cout << "Inventory successfully sorted by ";
    string searchCriteria = "n/a";

    switch (choice)
    {
    case SortMenuChoice::SORT_BY_CAR_ID:
        searchCriteria = "Car ID";
        break;
    case SortMenuChoice::SORT_BY_MODEL:
        searchCriteria = "Model";
        break;
    case SortMenuChoice::SORT_BY_MANUFACTURER:
        searchCriteria = "Manufacturer";
        break;
    case SortMenuChoice::SORT_BY_QUANTITY:
        searchCriteria = "Quantity";
        break;
    case SortMenuChoice::SORT_BY_PRICE:
        searchCriteria = "Price";
        break;
    }
    cout << searchCriteria << "\n";
}

void Inventory::printErrorFile() const
{
    bool hasErrors = false;
    ifstream errorFile(ERROR_FILENAME);
    string line;

    cout << "\nInvalid Records:\n";
    printHeader(true);

    while (getline(errorFile, line))
    {
        cout << line << "\n";
        hasErrors = true;
    }

    if (!hasErrors)
    {
        cout << "No invalid records found\n";
    }
}

void Inventory::printLowInventoryToFile() const
{
    ofstream outFile(LOW_INVENTORY_FILENAME);
    bool hasLowInventory = false;

    if (!outFile)
    {
        cout << "Error opening " << LOW_INVENTORY_FILENAME << " for writing\n";
    }
    else
    {
        cout << "\nLow Inventory Items:\n";
        printHeader();

        outFile << left << setw(10) << "Car ID"
            << left << setw(20) << "Model"
            << left << setw(20) << "Manufacturer"
            << right << setw(8) << "Qty"
            << right << setw(12) << "Price\n";
        outFile << string(70, '-') << "\n";

        for (int i = 0; i < recordCount; i++)
        {
            if (cars[i]->isLowInventory())
            {
                cout << cars[i]->toString() << "\n";
                outFile << cars[i]->toString() << "\n";
                hasLowInventory = true;
            }
        }

        if (!hasLowInventory)
        {
            cout << "No low inventory items found\n";
            outFile << "No low inventory items found\n";
        }

        outFile.close();
    }
}

void Inventory::searchByModelOrManufacturer() const
{
    string searchTerm;
    bool found = false;

    cin.ignore();
    cout << "Enter model or manufacturer to search: ";
    getline(cin, searchTerm);

    // Convert search term to lowercase for case insensitive search
    string searchTermLower = toLower(searchTerm);

    cout << "\nSearch results for '" << searchTerm << "':\n";
    printHeader();

    for (int i = 0; i < recordCount; i++)
    {
        string modelLower = toLower(cars[i]->getModel());
        string manufacturerLower = toLower(cars[i]->getManufacturer());

        // Check if search term is a substring of model or manufacturer
        bool foundInModel = modelLower.find(searchTermLower) != string::npos;
        bool foundInManufacturer = manufacturerLower.find(searchTermLower) != string::npos;

        if (foundInModel || foundInManufacturer)
        {
            cout << cars[i]->toString() << "\n";
            found = true;
        }
    }

    if (!found)
    {
        cout << "No matching records found\n";
    }
}

void Inventory::searchByPriceRange() const
{
    double targetPrice = 0.0;
    const double PRICE_SEARCH_RANGE = 0.12; // +/- 12% range required with project
    bool targetPriceFound = false;
    bool validInput = true;

    cout << "Enter target price: ";
    cin >> targetPrice;

    if (cin.fail())
    {
        clearInputError("Invalid price format. Please enter a numeric value.");
        validInput = false;
    }

    if (validInput)
    {
        double priceDifference = targetPrice * PRICE_SEARCH_RANGE;
        double lowerLimit = targetPrice - priceDifference;
        double upperLimit = targetPrice + priceDifference;

        cout << "\nSearch results for price range $" << fixed << setprecision(2) << lowerLimit
            << " to $" << upperLimit << ":\n";
        printHeader();

        for (int i = 0; i < recordCount; i++)
        {
            if (cars[i]->getPrice() >= lowerLimit && cars[i]->getPrice() <= upperLimit)
            {
                cout << cars[i]->toString() << "\n";
                targetPriceFound = true;
            }
        }

        if (!targetPriceFound)
        {
            cout << "No cars found in the specified price range\n";
        }
    }
}

void Inventory::sortMenu()
{
    int sortChoiceInt = 0;

    do
    {
        displaySortMenu();
        cin >> sortChoiceInt;
        SortMenuChoice sortChoice = static_cast<SortMenuChoice>(sortChoiceInt);

        switch (sortChoice)
        {
        case SortMenuChoice::SORT_BY_CAR_ID:
        case SortMenuChoice::SORT_BY_MODEL:
        case SortMenuChoice::SORT_BY_MANUFACTURER:
        case SortMenuChoice::SORT_BY_QUANTITY:
        case SortMenuChoice::SORT_BY_PRICE:
            sortInventory(static_cast<int>(sortChoice));
            printSortFieldMessage(sortChoice); // Helper/feedback function to provide confirmation that the data has been sorted by X criteria
            break;
        case SortMenuChoice::PRINT_CURRENT_SORT:
            printInventory();
            break;
        case SortMenuChoice::BACK_TO_MAIN_MENU_FROM_SORT:
            cout << "Returning to main menu...\n";
            break;
        default:
            clearInputError("Invalid sort choice. Please enter a number between 1 and " + to_string(static_cast<int>(SortMenuChoice::BACK_TO_MAIN_MENU_FROM_SORT)));
            break;
        }
    } while (sortChoiceInt != static_cast<int>(SortMenuChoice::BACK_TO_MAIN_MENU_FROM_SORT));
}

void Inventory::sortInventory(int sortField)
{
    // Bubble sort implementation required from the class instructions
    for (int i = 0; i < recordCount - 1; i++)
    {
        for (int j = 0; j < recordCount - i - 1; j++)
        {
            bool swapNeeded = false;

            SortMenuChoice sortChoice = static_cast<SortMenuChoice>(sortField);
            // Revised to now sort by descending order
            switch (sortChoice)
            {
            case SortMenuChoice::SORT_BY_CAR_ID:
                swapNeeded = cars[j]->getId() < cars[j + 1]->getId();
                break;
            case SortMenuChoice::SORT_BY_MODEL:
                swapNeeded = cars[j]->getModel() < cars[j + 1]->getModel();
                break;
            case SortMenuChoice::SORT_BY_MANUFACTURER:
                swapNeeded = cars[j]->getManufacturer() < cars[j + 1]->getManufacturer();
                break;
            case SortMenuChoice::SORT_BY_QUANTITY:
                swapNeeded = cars[j]->getQuantity() < cars[j + 1]->getQuantity();
                break;
            case SortMenuChoice::SORT_BY_PRICE:
                swapNeeded = cars[j]->getPrice() < cars[j + 1]->getPrice();
                break;
            }

            if (swapNeeded)
            {
                Car* temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}

/*


 --- Main Menu ---
1. Print Original Inventory
2. Search Card Records
3. Sort Inventory
4. Print Invalid Records
5. Quit
Enter choice: g
Invalid menu choice. Please enter a number between 1 and 5


 --- Main Menu ---
1. Print Original Inventory
2. Search Card Records
3. Sort Inventory
4. Print Invalid Records
5. Quit
Enter choice: 1

Original Inventory
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
ABCD1234  Civic               Honda                     12    19999.99
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*
IJKL9123  Mustang             Ford                       8    34999.99
QRST8912  Camry               Toyota                    15    22999.99
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
YZAB7891  Model3              Tesla                      7    39999.99
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
GHIJ6789  Challenger          Dodge                      6    38999.99
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*
FGHI3456  E350                Mercedes                   6    54999.99
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
VWXY1234  Sorento             Kia                        8    32999.99
ZABC5678  Soul                Kia                        5    19999.99
DEFG9123  Wrangler            Jeep                       7    42999.99
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*


 --- Main Menu ---
1. Print Original Inventory
2. Search Card Records
3. Sort Inventory
4. Print Invalid Records
5. Quit
Enter choice: 2

 --- Search Options ---
1. Model or Manufacturer
2. Price
3. Return to Main Menu
Enter choice: roll
Invalid search choice. Please enter a number between 1 and 3

 --- Search Options ---
1. Model or Manufacturer
2. Price
3. Return to Main Menu
Enter choice: 1
Enter model or manufacturer to search: roll

Search results for 'roll':
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*

 --- Search Options ---
1. Model or Manufacturer
2. Price
3. Return to Main Menu
Enter choice: 1
Enter model or manufacturer to search: Stroller

Search results for 'Stroller':
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*

 --- Search Options ---
1. Model or Manufacturer
2. Price
3. Return to Main Menu
Enter choice: 1
Enter model or manufacturer to search: TheLazyBrownFoxJumpedOver

Search results for 'TheLazyBrownFoxJumpedOver':
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
No matching records found

 --- Search Options ---
1. Model or Manufacturer
2. Price
3. Return to Main Menu
Enter choice: 2
Enter target price: 20000

Search results for price range $17600.00 to $22400.00:
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
ABCD1234  Civic               Honda                     12    19999.99
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
ZABC5678  Soul                Kia                        5    19999.99

 --- Search Options ---
1. Model or Manufacturer
2. Price
3. Return to Main Menu
Enter choice: 2
Enter target price: 2000000000

Search results for price range $1760000000.00 to $2240000000.00:
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
No cars found in the specified price range

 --- Search Options ---
1. Model or Manufacturer
2. Price
3. Return to Main Menu
Enter choice: 3
Returning to main menu...


 --- Main Menu ---
1. Print Original Inventory
2. Search Card Records
3. Sort Inventory
4. Print Invalid Records
5. Quit
Enter choice: 3

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: g
Invalid sort choice. Please enter a number between 1 and 7

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 1
Inventory successfully sorted by Car ID

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 6

Current Inventory Inventory
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
ZABC5678  Soul                Kia                        5    19999.99
YZAB7891  Model3              Tesla                      7    39999.99
VWXY1234  Sorento             Kia                        8    32999.99
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
QRST8912  Camry               Toyota                    15    22999.99
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
IJKL9123  Mustang             Ford                       8    34999.99
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
GHIJ6789  Challenger          Dodge                      6    38999.99
FGHI3456  E350                Mercedes                   6    54999.99
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*
DEFG9123  Wrangler            Jeep                       7    42999.99
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
ABCD1234  Civic               Honda                     12    19999.99

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 2
Inventory successfully sorted by Model

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 6

Current Inventory Inventory
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
DEFG9123  Wrangler            Jeep                       7    42999.99
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
ZABC5678  Soul                Kia                        5    19999.99
VWXY1234  Sorento             Kia                        8    32999.99
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
IJKL9123  Mustang             Ford                       8    34999.99
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
YZAB7891  Model3              Tesla                      7    39999.99
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*
FGHI3456  E350                Mercedes                   6    54999.99
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
ABCD1234  Civic               Honda                     12    19999.99
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
GHIJ6789  Challenger          Dodge                      6    38999.99
QRST8912  Camry               Toyota                    15    22999.99
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 3
Inventory successfully sorted by Manufacturer

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 6

Current Inventory Inventory
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
QRST8912  Camry               Toyota                    15    22999.99
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
YZAB7891  Model3              Tesla                      7    39999.99
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
FGHI3456  E350                Mercedes                   6    54999.99
ZABC5678  Soul                Kia                        5    19999.99
VWXY1234  Sorento             Kia                        8    32999.99
DEFG9123  Wrangler            Jeep                       7    42999.99
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
ABCD1234  Civic               Honda                     12    19999.99
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*
IJKL9123  Mustang             Ford                       8    34999.99
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
GHIJ6789  Challenger          Dodge                      6    38999.99

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 4
Inventory successfully sorted by Quantity

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 6

Current Inventory Inventory
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
QRST8912  Camry               Toyota                    15    22999.99
ABCD1234  Civic               Honda                     12    19999.99
VWXY1234  Sorento             Kia                        8    32999.99
IJKL9123  Mustang             Ford                       8    34999.99
YZAB7891  Model3              Tesla                      7    39999.99
DEFG9123  Wrangler            Jeep                       7    42999.99
FGHI3456  E350                Mercedes                   6    54999.99
GHIJ6789  Challenger          Dodge                      6    38999.99
ZABC5678  Soul                Kia                        5    19999.99
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 5
Inventory successfully sorted by Price

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 6

Current Inventory Inventory
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
FGHI3456  E350                Mercedes                   6    54999.99
DEFG9123  Wrangler            Jeep                       7    42999.99
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
YZAB7891  Model3              Tesla                      7    39999.99
GHIJ6789  Challenger          Dodge                      6    38999.99
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
IJKL9123  Mustang             Ford                       8    34999.99
VWXY1234  Sorento             Kia                        8    32999.99
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*
QRST8912  Camry               Toyota                    15    22999.99
ABCD1234  Civic               Honda                     12    19999.99
ZABC5678  Soul                Kia                        5    19999.99
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*

 --- Sort Options ---
1. Car ID
2. Model
3. Manufacturer
4. Quantity
5. Price
6. Print Sorted Inventory
7. Return to Main Menu
Enter choice: 7
Returning to main menu...


 --- Main Menu ---
1. Print Original Inventory
2. Search Card Records
3. Sort Inventory
4. Print Invalid Records
5. Quit
Enter choice: 1

Original Inventory
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
ABCD1234  Civic               Honda                     12    19999.99
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*
IJKL9123  Mustang             Ford                       8    34999.99
QRST8912  Camry               Toyota                    15    22999.99
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
YZAB7891  Model3              Tesla                      7    39999.99
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
GHIJ6789  Challenger          Dodge                      6    38999.99
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*
FGHI3456  E350                Mercedes                   6    54999.99
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
VWXY1234  Sorento             Kia                        8    32999.99
ZABC5678  Soul                Kia                        5    19999.99
DEFG9123  Wrangler            Jeep                       7    42999.99
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*


 --- Main Menu ---
1. Print Original Inventory
2. Search Card Records
3. Sort Inventory
4. Print Invalid Records
5. Quit
Enter choice: 4

Invalid Records:
Car ID    Model               Manufacturer             Qty       Price   Error Message
----------------------------------------------------------------------------------------------------
MNOP4567  F150                Ford                       2    42999.99   *LOW INVENTORY* - First 4 characters must be alpha only (excluding O);
XYZA4567  A4                  Audi                       9    45999.99 - Model contains less than minimum required characters;
BCDE8912  Q7                  Audi                       3    59999.99   *LOW INVENTORY* - Model contains less than minimum required characters;
NOPQ2345  Rogue               Nissan                    11    26999.99 - First 4 characters must be alpha only (excluding O);
LMNO8912  XC90                Volvo                      2    57999.99   *LOW INVENTORY* - First 4 characters must be alpha only (excluding O);
ABCD1234  GV70                Genesis                   -3    49999.99   *LOW INVENTORY* - Quantity must be at least 0;
WXYZ1234  M3                  BMW                        5     9999.99 - Model contains less than minimum required characters; Price must be above minimum price;
WXYA1234  @Special            BMW                        4    29999.99   *LOW INVENTORY* - Model does not start with an alpha character; Model contains non-alphanumeric character;
ABCD1234  Z4                  BMW                       -2    11000.00   *LOW INVENTORY* - Model contains less than minimum required characters; Price must be above minimum price; Quantity must be at least 0;
WXYZ1234  Z8                  BMW                       -3     9999.99   *LOW INVENTORY* - Model contains less than minimum required characters; Price must be above minimum price; Quantity must be at least 0;


 --- Main Menu ---
1. Print Original Inventory
2. Search Card Records
3. Sort Inventory
4. Print Invalid Records
5. Quit
Enter choice: 5

Low Inventory Items:
Car ID    Model               Manufacturer             Qty       Price
----------------------------------------------------------------------
TUVW9123  Ghost               Rolls_Royce                1   299999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce_Extra          4   199999.99   *LOW INVENTORY*
PQRS5678  Stroller            Rolls_Royce                2   199999.99   *LOW INVENTORY*
JKLM7891  S550                Mercedes                   2    94999.99   *LOW INVENTORY*
CDEF2345  ModelX              Tesla                      1    79999.99   *LOW INVENTORY*
PQRS3456  S60                 Volvo                      4    41999.99   *LOW INVENTORY*
HIJK4567  Cherokee            Jeep                       3    38999.99   *LOW INVENTORY*
TUVW7891  Highlander          Toyota                     1    37999.99   *LOW INVENTORY*
KLMN1234  Charger             Dodge                      3    35999.99   *LOW INVENTORY*
RSTU6789  Altima              Nissan                     4    25999.99   *LOW INVENTORY*
EFGH5678  Accord              Honda                      3    24999.99   *LOW INVENTORY*
UVWX3456  Corolla             Toyota                     4    17999.99   *LOW INVENTORY*
Now exiting program...
*/