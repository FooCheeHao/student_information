#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

// Student Information System
// Function declarations for better code organization
bool adminLogin();
void studentLogin();
void adminMenu(sqlite3* db);
void displayStudentData(sqlite3* db);
void displayStudentAttendance(sqlite3* db);
int callback(void *NotUsed, int argc, char **argv, char **azColName);  // Remove static from declaration

// Node structure for double-linked list
struct Node {
    int student_id;
    int attendance[14];
    Node* prev;
    Node* next;
};

// Function to add a node to the double-linked list
Node* addNode(Node* head, int student_id, int* attendance) {
    Node* newNode = new Node;
    newNode->student_id = student_id;
    for (int i = 0; i < 14; ++i) {
        newNode->attendance[i] = attendance[i];
    }
    newNode->next = nullptr;
    newNode->prev = nullptr;
    
    if (head == nullptr) {
        return newNode;
    }

    Node* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;

    return head;
}

// Function to display the double-linked list
void displayList(Node* head) {
    Node* temp = head;
    while (temp != nullptr) {
        cout << "Student ID: " << temp->student_id << " | Attendance: ";
        for (int i = 0; i < 14; ++i) {
            cout << temp->attendance[i] << " ";
        }
        cout << endl;
        temp = temp->next;
    }
}

// Function to free the double-linked list
void freeList(Node* head) {
    Node* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    int choice;
    sqlite3 *db;
    
    // Open database
    if (sqlite3_open("example.db", &db)) {
        cerr << "\nCan't open database: " << sqlite3_errmsg(db) << endl;
        return 1;
    } else {
        cout << "\nOpened database successfully" << endl;
    }

    cout << "Welcome to the NEUC Student Information System!" << endl;
    
    while (true) {
        cout << "\n###### Login Menu ######" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Student Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice (1/2/3): ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (adminLogin()) {
                    adminMenu(db);
                }
                break;
            case 2:
                studentLogin();
                break;
            case 3:
                cout << "\nYou are exiting the system. Goodbye!" << endl;
                sqlite3_close(db);
                return 0;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    }
    return 0;
}

// Admin login function
bool adminLogin() {
    const string adminID = "admin";
    const string adminPassword = "123456";
    string enteredID, enteredPassword;

    cout << "Enter Admin ID: ";
    cin >> enteredID;
    cout << "Enter Admin Password: ";
    cin >> enteredPassword;

    if (enteredID == adminID && enteredPassword == adminPassword) {
        cout << "Admin Login Successful!" << endl;
        return true;
    } else {
        cout << "Invalid Admin ID or Password! Try again." << endl;
        return adminLogin();  // Recursive call for retry
    }
}

// Admin menu function
void adminMenu(sqlite3* db) {
    int adminChoice, rc, student_ID, ID, Student_Password;
    sqlite3_stmt* stmt;
    string sql, Name, Email_Address, Course, Subject;

    if (sqlite3_open("example.db", &db)) {
        cerr << "Error opening DB: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (true) {
        cout << "\n###### Admin Menu ######" << endl;
        cout << "1. Check Student Database" << endl;
        cout << "2. Check Student Attendance" << endl;
        cout << "########################" << endl;
        cout << "3. Add student to database" << endl;
        cout << "4. Remove student from database" << endl;
        cout << "########################" <<endl;
        cout << "5. Change Student Email Address" << endl;
        cout << "6. Change Student Password" << endl;
        cout << "7. Log Out" <<endl;
        cout << "Enter your choice with number: ";
        cin >> adminChoice;

        switch (adminChoice) {
            case 1:
                displayStudentData(db);
                break;
            case 2:
                displayStudentAttendance(db);
                break;
            case 3:
                cout << "\n###### Add Student to Database ######" << endl;//add student
                char confirm;
                cout << "Are you sure want to add student on database? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {

                cout << "Enter new ID: ";
                cin >> ID;
                cout << "Enter new student ID: ";
                cin >> student_ID;
                cout << "Enter new student pasword: ";
                cin >> Student_Password;
                cout << "Enter new student name: ";
                cin >> Name;
                cout << "Enter new student email address: ";
                cin >> Email_Address;
                cout << "Enter new student course: ";
                cin >> Course;
                cout << "Enter new student subject: ";
                cin >> Subject;

                sql = "INSERT INTO Student_Database (ID, Student_ID, Student_Password, Name, Email_Address, Course, Homework, Assignment, Comment, Subject, Result) VALUES (?,?,?,?,?,?, 'Not Done', 'Not Done', 'New Hopeness.', ?, 'NA')";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, ID);
                sqlite3_bind_int(stmt, 2, student_ID);
                sqlite3_bind_int(stmt, 3, Student_Password);
                sqlite3_bind_text(stmt, 4, Name.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 5, Email_Address.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 6, Course.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 7, Subject.c_str(), -1, SQLITE_STATIC);
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Add Student to database process successfully." << endl;
                } else {
                    cout << "Failed to process. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sql = "INSERT INTO Student_Attendance (Student_ID, Week1, Week2, Week3, Week4, Week5, Week6, Week7, Week8, Week9, Week10, Week11, Week12, Week13, Week14) VALUES (?, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)";//attandance
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, student_ID);
                cout << "Attendance Record..."<<endl;
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Attendance record complete." << endl;
                } else {
                    cout << "Failed to process. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
                } else {
                    cout << "\nAdd student to database process canceled." << endl;
                }
                break;
            case 4:
                cout << "\n###### Remove Student from Database ######" << endl;//remove student
                cout << "Are you sure want to remove student from database? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {

                cout << "Enter student ID to remove: ";
                cin >> student_ID;

                sql = "DELETE FROM Student_Database WHERE Student_ID = ?;";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, student_ID);
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Remove student from database successfully." << endl;
                } else {
                    cout << "Failed to process. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sql = "DELETE FROM Student_Attendance WHERE Student_ID = ?;";//attendance
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, student_ID);
                cout << "Attendance Record Removing..."<<endl;
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Attendance remove complete." << endl;
                } else {
                    cout << "Failed to process. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
                } else {
                    cout << "\nRemove student from database process canceled." << endl;
                }
                break;
            case 5:
    cout << "\n###### Change Student Email Address ######" << endl;
    cout << "Are you sure you want to change student email address? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        string oldEmail, newEmail;

        cout << "Enter student id to locate: ";
        cin >> student_ID;
        cout << "Enter student old email address: ";
        cin >> oldEmail;
        cout << "Enter student new email address: ";
        cin >> newEmail;

        // Query to check if the old email address matches the one in the database
        string sql = "SELECT Email_Address FROM Student_Database WHERE Student_ID = ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Failed to prepare SQL statement." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            break;
        }

        sqlite3_bind_int(stmt, 1, student_ID);

        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);

            if (email == oldEmail) {
                // Update the email address
                sql = "UPDATE Student_Database SET Email_Address = ? WHERE Student_ID = ? AND Email_Address = ?";
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                    cout << "Failed to prepare SQL statement." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                    break;
                }

                sqlite3_bind_text(stmt, 1, newEmail.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, student_ID);
                sqlite3_bind_text(stmt, 3, oldEmail.c_str(), -1, SQLITE_STATIC);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Email address changed successfully." << endl;
                } else {
                    cout << "Failed to change email address. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
            } else {
                cout << "The email address entered does not match our records." << endl;
            }
        } else {
            cout << "Failed to retrieve the email address. Please try again." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
        }
    } else {
        cout << "\nEmail address change canceled." << endl;
    }
                break;
            case 6:
    cout << "\n###### Change Password ######" << endl;
    confirm;
    cout << "Are you sure you want to change your password? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        string oldPassword, newPassword;

        cout << "Enter student id to locate: ";
        cin >> student_ID;
        cout << "Enter your old password: ";
        cin >> oldPassword;
        cout << "Enter your new password: ";
        cin >> newPassword;

        // Query to check if the old email address matches the one in the database
        sql = "SELECT Student_Password FROM Student_Database WHERE Student_ID = ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Failed to prepare SQL statement." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            break;
        }

        sqlite3_bind_int(stmt, 1, student_ID);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            string Password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);

            if (Password == oldPassword) {
                // Update the email address
                sql = "UPDATE Student_Database SET Student_Password = ? WHERE Student_ID = ? AND Student_Password = ?";
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                    cout << "Failed to prepare SQL statement." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                    break;
                }

                sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, student_ID);
                sqlite3_bind_text(stmt, 3, oldPassword.c_str(), -1, SQLITE_STATIC);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Password changed successfully." << endl;
                } else {
                    cout << "Failed to change password. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
            } else {
                cout << "The password entered does not match our records." << endl;
            }
        } else {
            cout << "Failed to retrieve the email address. Please try again." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
        }
    } else {
        cout << "\nPassword change canceled." << endl;
    }      
                break;
            case 7:
                cout << "\nLogging out..." << endl;
                sqlite3_close(db);
                return;
            default:
                cout << "\nInvalid choice! Please enter invalid number." << endl;
                break;
        }
    }
}

// Display student data function
void displayStudentData(sqlite3* db) {
    const char* sql = "SELECT * FROM Student_Database";
    char* zErrMsg = nullptr;
    string skip;

    cout << "\n####################################" << endl;
    if (sqlite3_exec(db, sql, callback, nullptr, &zErrMsg) != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    } else {
        cout << "####################################" << endl;
        cout << "Student Database Listed Successfully" << endl;
    }

    cout << "\nPress any button to continue: ";
    cin >> skip;
}

// Display student attendance function
void displayStudentAttendance(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Student_Attendance";
    char* zErrMsg = nullptr;
    int rc;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
        return;
    }

    Node* head = nullptr;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int student_id = sqlite3_column_int(stmt, 0);
        int attendance[14];
        for (int i = 0; i < 14; ++i) {
            attendance[i] = sqlite3_column_int(stmt, i + 1);
        }
        head = addNode(head, student_id, attendance);
    }

    if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "\n#### Student Attendance ####" << endl;
        displayList(head);
    }

    sqlite3_finalize(stmt);
    freeList(head);
}

// Callback function for SQLite execution
int callback(void *NotUsed, int argc, char **argv, char **azColName) {  // Remove static from definition
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
    return 0;
}

// Student login function
void studentLogin() {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int student_id, student_password;
    int choiceB;

    if (sqlite3_open("example.db", &db)) {
        cerr << "Error opening DB: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Enter Student ID: ";
    cin >> student_id;
    cout << "Enter Student Password: ";
    cin >> student_password;

    string sql = "SELECT Name, Email_Address, Course FROM Student_Database WHERE Student_ID = ? AND Student_Password = ?";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, student_id);
    sqlite3_bind_int(stmt, 2, student_password);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string course = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        cout << "\n###### Welcome " << name << "! ######" << endl;
        cout << "Email: " << email << endl;
        cout << "Course: " << course << endl;
        
    } else {
        cout << "Invalid Student ID or Password. Try again." << endl;
        studentLogin();
    }

    int choice, rc, studentID;
    string skipAboutSchool, result, homeworkStatusUpdate, assignmentStatusUpdate, commentUpdate, skipHoliday;

    while (true) {
        cout << "\n###### Student Portal ######" << endl;
        cout << "1. View Class Material" << endl;
        cout << "2. View Course Announcements" << endl;
        cout << "3. View Homework Submission" << endl;
        cout << "4. View Assignment Submission" << endl;
        cout << "5. View Course Comments" << endl;
        cout << "6. View Result" << endl;
        cout << "##############################" << endl;
        cout << "7. Update Homework Status" << endl;
        cout << "8. Update Assignment Status" << endl;
        cout << "9. Update Course Comment" << endl;
        cout << "##############################" << endl;
        cout << "10. About School" << endl;
        cout << "11. 2024 public holiday" << endl;
        cout << "##############################" << endl;
        cout << "12. Change Email Address" << endl;
        cout << "13. Change Password" << endl;
        cout << "14. Log Out" << endl;
        cout << "Enter your choice with number: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n###### Class Material ######" << endl;
                cout << "1. sem3_material1~14.pdf" << endl;
                cout << "\nNo more information already" <<endl;
                break;
            case 2:
                cout << "\n###### Course Announcements ######" << endl;
                cout << "1. sem3_week14_presentation_assignment" << endl;
                cout << "\nNo more information already" <<endl;
                break;
            case 3:
                cout << "\n###### View Homework Submission ######" << endl;
                
                sql = "SELECT Homework FROM Student_Database WHERE Student_ID = ?;";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, student_id);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) {
                    result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    cout << "Homework Status: " << result << endl;
                } else {
                    cout << "Failed to showout result." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
                break;
            case 4:
                cout << "\n###### View Assignment Submission ######" << endl;
                
                if (sqlite3_open("example.db", &db) != SQLITE_OK) {
                    cerr << "Cannot open db: " << sqlite3_errmsg(db) << endl;
                    return;
                }
                sql = "SELECT Assignment FROM Student_Database WHERE Student_ID = ?;";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, student_id);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) {
                    result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    cout << "Assignment Status: " << result << endl;
                } else {
                    cout << "Failed to showout result." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
                break;
            case 5:
                cout << "\n###### View Course Comments ######" << endl;
                
                if (sqlite3_open("example.db", &db) != SQLITE_OK) {
                    cerr << "Cannot open db: " << sqlite3_errmsg(db) << endl;
                    return;
                }
                sql = "SELECT Comment FROM Student_Database WHERE Student_ID = ?;";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, student_id);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) {
                    result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    cout << "Comment: " << result << endl;
                } else {
                    cout << "Failed to showout result." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
                break;
            case 6:
                cout << "\n###### View Result ######" << endl;
                
                sql = "SELECT Result FROM Student_Database WHERE Student_ID = ?;";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_int(stmt, 1, student_id);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) {
                    result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    cout << "Result: " << result << endl;
                } else {
                    cout << "Failed to showout result." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
                break;
            case 7:
                cout << "\n###### Update Homework Status ######" << endl;
                
                cout << "Update your homework status here: ";
                cin >> homeworkStatusUpdate;

                sql = "UPDATE Student_Database SET Homework = ? WHERE Student_ID = ?";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_text(stmt, 1, homeworkStatusUpdate.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, student_id);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Update Homework Status successfully." << endl;
                } else {
                    cout << "Failed to process. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
                break;
            case 8:
                cout << "\n###### Update Assignment Status ######" << endl;

                cout << "Update your assignment status here: ";
                cin >> assignmentStatusUpdate;

                sql = "UPDATE Student_Database SET Assignment = ? WHERE Student_ID = ?";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_text(stmt, 1, assignmentStatusUpdate.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, student_id);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Update Assignment Status successfully." << endl;
                } else {
                    cout << "Failed to process. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
                break;
            case 9:
                cout << "\n###### Update Course Comment ######" << endl;
                if (sqlite3_open("example.db", &db)) {
                    cerr << "Error opening DB: " << sqlite3_errmsg(db) << endl;
                    return;
                }

                cout << "Update your course comment here: ";
                cin >> commentUpdate;

                sql = "UPDATE Student_Database SET Comment = ? WHERE Student_ID = ?";
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                sqlite3_bind_text(stmt, 1, commentUpdate.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, student_id);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Update Course Comment successfully." << endl;
                } else {
                    cout << "Failed to process. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
                break;
            case 10:
                cout << "\n###### About School ######" <<endl;
                cout << "New Era University College (NEUC) is a non-profit, private university college in Malaysia that was established in 1997, following the closure of Nantah University in Singapore in 1980. " <<endl;
                cout << "Its establishment was made possible through extensive efforts and steadfast support from the Chinese community."<<endl;
                cout << "\nNEUC currently has nine(9) faculties including: "<<endl;
                cout << "1. Institute of International Education"<<endl;
                cout << "2. Graduate School" <<endl;
                cout << "3. Arts and Social Sciences" <<endl;
                cout << "4. Art and Design" <<endl;
                cout << "5. Communication, Cinematic and Performing Arts"<<endl;
                cout << "6. Accountancy, Management and Economics"<<endl;
                cout << "7. Computer Science and Information Computing Technology"<<endl;
                cout << "8. the Institute of Ageing and Professional Care"<<endl;
                cout << "9. Health, Safety, Security and Environment"<<endl;
                cout << "\nMore Information, pls visit: https://www.newera.edu.my/"<<endl;
                cout << "\nPress any button to continue: ";
                cin >> skipAboutSchool;
                break;
            case 11: {
                cout << "\n###### 2024 public holiday ######" << endl;

                if (sqlite3_open("example.db", &db)) {
                    cerr << "Error opening DB: " << sqlite3_errmsg(db) << std::endl;
                    return;
                }

                const char* sqlb = "SELECT Holiday_Name, Date, Day, Special_Remarks FROM Public_Holidays;";
                rc = sqlite3_prepare_v2(db, sqlb, -1, &stmt, nullptr);
                if (rc != SQLITE_OK) {
                    cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_close(db);
                    return;
                }
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                    string holidayName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                    string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                    string day = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                    const char* specialRemarks = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

                    cout << holidayName << " - ";
                    cout << date << " - ";
                    cout << day << " - " ;
                    cout << (specialRemarks ? specialRemarks : "None")<<endl;
                }

                if (rc != SQLITE_DONE) {
                    cerr << "Failed to step through results: " << sqlite3_errmsg(db) << endl;
                }

                cout << "\n(*) Chinese New Years Eve and Mid-Autumn Festival are special holidays declared by NEUC. No replacement leaves shall be given when it falls on a public holiday." << endl;
                cout << "(**) Subject to change:" << endl;
                cout << "If a holiday falls on a Sunday, the following day shall be declared a public holiday, and if the following day is also a public holiday, the subsequent day shall be declared a public holiday" << endl;
                cout << "\nPress any button to continue: ";
                cin >> skipHoliday;

                sqlite3_finalize(stmt);
                break;
            }
            case 12:
    cout << "\n###### Change Email Address ######" << endl;
    char confirm;
    cout << "Are you sure you want to change your email address? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        string oldEmail, newEmail;

        cout << "Enter your old email address: ";
        cin >> oldEmail;
        cout << "Enter your new email address: ";
        cin >> newEmail;

        // Query to check if the old email address matches the one in the database
        sql = "SELECT Email_Address FROM Student_Database WHERE Student_ID = ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Failed to prepare SQL statement." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            break;
        }

        sqlite3_bind_int(stmt, 1, student_id);

        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);

            if (email == oldEmail) {
                // Update the email address
                sql = "UPDATE Student_Database SET Email_Address = ? WHERE Student_ID = ? AND Email_Address = ?";
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                    cout << "Failed to prepare SQL statement." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                    break;
                }

                sqlite3_bind_text(stmt, 1, newEmail.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, student_id);
                sqlite3_bind_text(stmt, 3, oldEmail.c_str(), -1, SQLITE_STATIC);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Email address changed successfully." << endl;
                } else {
                    cout << "Failed to change email address. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
            } else {
                cout << "The old email address you entered does not match our records." << endl;
            }
        } else {
            cout << "Failed to retrieve the email address. Please try again." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
        }
    } else {
        cout << "\nEmail address change canceled." << endl;
    }
    break;

            case 13:
    cout << "\n###### Change Password ######" << endl;
    confirm;
    cout << "Are you sure you want to change your password? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        string oldPassword, newPassword;

        cout << "Enter your old password: ";
        cin >> oldPassword;
        cout << "Enter your new password: ";
        cin >> newPassword;

        // Query to check if the old email address matches the one in the database
        sql = "SELECT Student_Password FROM Student_Database WHERE Student_ID = ?;";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Failed to prepare SQL statement." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            break;
        }

        sqlite3_bind_int(stmt, 1, student_id);

        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            string Password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);

            if (Password == oldPassword) {
                // Update the email address
                sql = "UPDATE Student_Database SET Student_Password = ? WHERE Student_ID = ? AND Student_Password = ?";
                if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                    cout << "Failed to prepare SQL statement." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                    break;
                }

                sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 2, student_id);
                sqlite3_bind_text(stmt, 3, oldPassword.c_str(), -1, SQLITE_STATIC);

                rc = sqlite3_step(stmt);
                if (rc == SQLITE_DONE) {
                    cout << "Password changed successfully." << endl;
                } else {
                    cout << "Failed to change password. Please try again." << endl;
                    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
            } else {
                cout << "The password you entered does not match our records." << endl;
            }
        } else {
            cout << "Failed to retrieve the email address. Please try again." << endl;
            cout << "SQL error: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
        }
    } else {
        cout << "\nPassword change canceled." << endl;
    }
    break;

            case 14:
                cout << "\nLogging out..." << endl;
                sqlite3_close(db);
                return;  // Return to main menu
            default:
                cout << "\nInvalid choice! Please enter invalid number." << endl;
                break;
        }
    }
}
