#include <EEPROM.h>

int Var;
int ram[128];
/*

Add the all code line ended a ' ' (space) . Exp : 'EEPROM WRITE 20 , 1 '

PAINT Hello, World!         // Print "Hello, World!"

VAR B = 15 , ADDRESS : 20   // Set variable B to 15 at address 20
ARG ADDRESS : 10 , ADDRESS : 20 , ADD   // Add values at addresses 10 and 20 (42 + 15), should print 57
ARG ADDRESS : 10 , ADDRESS : 20 , SUB   // Subtract values at addresses 10 and 20 (42 - 15), should print 27
ARG ADDRESS : 10 , ADDRESS : 20 , MUL   // Multiply values at addresses 10 and 20 (42 * 15), should print 630
ARG ADDRESS : 10 , ADDRESS : 20 , DIV   // Divide values at addresses 10 and 20 (42 / 15), should print 2 (integer division)

EEPROM WRITE 20 , 123       // Write value 123 to EEPROM address 20
EEPROM READ 20               // Read from EEPROM address 20 and print the value

VAR A = 42 , ADDRESS : 10   // Set variable A to 42 at address 10
PRINT 10                    // Print value at address 10 (should print 42)

*/
void Compiler(char *code) {
    if (strncmp(code, "VAR", 3) == 0) {
        int add, _Var;
        char ch;
        if (sscanf(code, "VAR %c = %d , ADDRESS : %d", &ch, &_Var, &add) == 3) {
            Var = _Var;
            if (add >= 0 && add < 128) {
                ram[add] = Var;
            } else {
                Serial.println("ADDRESS OUT OF RANGE");
            }
        } else {
            Serial.println("SYNTAX ERROR");
        }
    }
    else if (strncmp(code, "EEPROM WRITE", 12) == 0) {
        int add, data;
        if (sscanf(code, "EEPROM WRITE %d , %d", &add, &data) == 2) {
            if (add >= 0 && add < EEPROM.length()) {
                EEPROM.write(add, data);
            } else {
                Serial.println("EEPROM ADDRESS OUT OF RANGE");
            }
        } else {
            Serial.println("SYNTAX ERROR");
        }
    }
    else if (strncmp(code, "EEPROM READ", 11) == 0) {
        int add;
        if (sscanf(code, "EEPROM READ %d", &add) == 1) {
            if (add >= 0 && add < EEPROM.length()) {
                int data = EEPROM.read(add);
                Serial.println(data);
            } else {
                Serial.println("EEPROM ADDRESS OUT OF RANGE");
            }
        } else {
            Serial.println("SYNTAX ERROR");
        }
    }
    else if (strncmp(code, "PAINT", 5) == 0) {
        char m[256];
        if (sscanf(code, "PAINT %s", m) == 1) {
            Serial.println(m);
        } else {
            Serial.println("SYNTAX ERROR");
        }
    }
    else if (strncmp(code, "PRINT", 5) == 0) {
        int add;
        if (sscanf(code, "PRINT %d", &add) == 1) {
            if (add >= 0 && add < 128) {
                Serial.println(ram[add]);
            } else {
                Serial.println("ADDRESS OUT OF RANGE");
            }
        } else {
            Serial.println("SYNTAX ERROR");
        }
    }
    else if (strncmp(code, "ARG", 3) == 0) {
        int add1, add2;
        char op[4];
        if (sscanf(code, "ARG ADDRESS : %d , ADDRESS : %d , %3s", &add1, &add2, op) == 3) {
            int s1 = (add1 >= 0 && add1 < 128) ? ram[add1] : 0;
            int s2 = (add2 >= 0 && add2 < 128) ? ram[add2] : 0;
            if (strcmp(op, "ADD") == 0) {
                Serial.println(s1 + s2);
            } else if (strcmp(op, "SUB") == 0) {
                Serial.println(s1 - s2);
            } else if (strcmp(op, "MUL") == 0) {
                Serial.println(s1 * s2);
            } else if (strcmp(op, "DIV") == 0) {
                if (s2 != 0) {
                    Serial.println(s1 / s2);
                } else {
                    Serial.println("DIVISION BY ZERO");
                }
            } else {
                Serial.println("INVALID OPERATOR");
            }
        } else {
            Serial.println("SYNTAX ERROR");
        }
    }
    else {
        Serial.println("INVALID COMMAND");
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("\n Welcome the SerialOS \n");
}

void loop() {
    if (Serial.available() > 0) {
        char receivedString[100];
        Serial.readBytesUntil('\n', receivedString, sizeof(receivedString) - 1);
        receivedString[sizeof(receivedString) - 1] = '\0'; // Ensure null-termination
        Compiler(receivedString);
    }
}

