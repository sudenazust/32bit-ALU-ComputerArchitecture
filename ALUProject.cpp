#include <iostream>
#include <bitset>
using namespace std;

//Status Flags
bool ZF = false;  // ZF: Set to 1 if result is zero
bool CF = false;  // CF: Set to 1 if unsigned overflow (carry out) occurred
bool OF = false;  // OF: Set to 1 if signed overflow occurred
bool NF = false;  // NF: Set to 1 if result is negative (sign bit is 1)

//Flag Update
void updateFlags(int32_t result, uint32_t a, uint32_t b, char op) {
    ZF = (result == 0);
    NF = (result < 0);
    CF = false;
    OF = false;

    if (op == '+') {
        // Use 64 bit addition to detect unsigned overflow (carry)
        uint64_t wide = (uint64_t)a + (uint64_t)b;
        CF = (wide >> 32) != 0;
        // Overflow flag: occurs if signed addition overflows
        if (((int32_t)a > 0 && (int32_t)b > 0 && result < 0) ||
            ((int32_t)a < 0 && (int32_t)b < 0 && result > 0)) {
            OF = true;
        }
    }
    else if (op == '-') {
        CF = (a < b);  // Carry flag: if unsigned borrow occurs
        // Overflow flag: occurs if signed subtraction overflows
        if (((int32_t)a > 0 && (int32_t)b < 0 && result < 0) ||
            ((int32_t)a < 0 && (int32_t)b > 0 && result > 0)) {
            OF = true;
        }
    }
}

//Flag Display:Prints the current status of ALU flags to the console after each operation.
void displayFlags() {
    cout << "\nStatus Flags\n";
    cout << "ZF (Zero Flag): " << ZF << endl;
    cout << "CF (Carry Flag): " << CF << endl;
    cout << "OF (Overflow Flag): " << OF << endl;
    cout << "NF (Negative Flag): " << NF << endl;
    cout << "\n";
}
void showMenu() {  // Displays the list of available ALU operations for user selection.
    cout << "\n****** 32-Bit ALU MENU ******\n";
    cout << "1.  ADD (a + b)\n";
    cout << "2.  SUB (a - b)\n";
    cout << "3.  AND (a & b)\n";
    cout << "4.  OR  (a | b)\n";
    cout << "5.  INC (a + 1)\n";
    cout << "6.  DEC (a - 1)\n";
    cout << "7.  XOR (a ^ b)\n";
    cout << "8.  NOT (!a)\n";
    cout << "9.  SHIFT LEFT (a << n)\n";
    cout << "10. SHIFT RIGHT (a >> n)\n";
    cout << "11. CMP (a ? b)\n";
    cout << "12. EXIT\n";
    cout << "Select an operation (1-12): ";
}

int main()
{
    uint32_t a = 0, b = 0;
    int32_t result = 0;
    int choice = 0;

    while (true) {
         showMenu();    // Display ALU operation menu
        cin >> choice;   // Read user selection
        // Input validation: handle non-numeric inputs 
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number between 1–12.\n";
            continue;
        }
        // Read inputs for two-operand operations
        if (choice >= 1 && choice <= 4 || choice == 7 || choice == 11) {
            cout << "Enter first number: ";
            cin >> a;
            cout << "Enter second number: ";
            cin >> b;
        }
        // Read input for single-operand operations (INC, DEC, NOT)
        else if (choice == 5 || choice == 6 || choice == 8) {
            cout << "Enter number: ";
            cin >> a;
        }
        // Read input and shift amount for shift operations
        else if (choice == 9 || choice == 10) {
            cout << "Enter number: ";
            cin >> a;
            cout << "Enter shift amount (0-31): ";  // Prompt user to enter valid shift amount for 32-bit value
            cin >> b;
        }


        switch (choice) {
        case 1: result = a + b; updateFlags(result, a, b, '+'); break;
        case 2: result = a - b; updateFlags(result, a, b, '-'); break;
        case 3: result = a & b; updateFlags(result, a, b, '&'); break;
        case 4: result = a | b; updateFlags(result, a, b, '|'); break;
        case 5: result = a + 1; updateFlags(result, a, 1, '+'); break;
        case 6: result = a - 1; updateFlags(result, a, 1, '-'); break;
        case 7: result = a ^ b; updateFlags(result, a, b, '^'); break;
        case 8: result = ~a;    updateFlags(result, a, 0, '~'); break;
        case 9: result = a << b; updateFlags(result, a, b, '<'); break;
        case 10: result = a >> b; updateFlags(result, a, b, '>'); break;
        case 11:    // CMP only updates flags; no result is displayed
            result = a - b;
            updateFlags(result, a, b, '-');
            cout << "Completed. No result shown.\n";
            break;
        case 12:
            cout << "Exiting program...\n";
            return 0;
        default:
            cout << "Invalid selection.\n";
            continue;
        }
        // Display result
        if (choice != 11 && choice >= 1 && choice <= 10) {
            cout << "\nResult (Decimal): " << result << endl;
            cout << "Result (Binary) : " << bitset<32>(result) << endl;
        }
        displayFlags(); // Display current status flag values

        
    }
}

