#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "employee.txt"
#define WORK_DAYS 30
#define MAX_USER 2
#define MAX_LEN 100
#define LINE_LEN 512
#define USER_FILE "users.txt"

struct Employee
{
    int id;
    char name[50];
    int age;
    char contact[20];
    char email[50];
    char joining_date[20];
    char position[50];
    float base_salary;
    int attendance;
    float bonus;
    float deduction;
};

struct User
{
    char username[MAX_LEN];
    char password[MAX_LEN];
    char role[MAX_LEN];
};

void clearInputBuffer();
void saveUsersToFile();
int loadUsersFromFile();
void inputUserCredentials();
int login(const char *role);
void addEmployee();
int readEmployeeBlock(FILE *fp, struct Employee *emp);
void viewEmployees();
void writeEmployeeToFile(FILE *fp, struct Employee *emp);
void updateEmployee();
void updateAttendance();
void calculateSalary();
void adminMenu();
void hrMenu();

struct User users[MAX_USER];

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void saveUsersToFile()
{
    FILE *fp = fopen(USER_FILE, "w");
    if (!fp)
    {
        printf("Error saving users!\n");
        return;
    }
    for (int i = 0; i < MAX_USER; i++)
    {
        fprintf(fp, "%s,%s,%s\n", users[i].role, users[i].username, users[i].password);
    }
    fclose(fp);
}
int loadUsersFromFile()
{
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp)
        return 0; // file not found

    for (int i = 0; i < MAX_USER; i++)
    {
        if (fscanf(fp, "%[^,],%[^,],%[^\n]\n", users[i].role, users[i].username, users[i].password) != 3)
        {
            fclose(fp);
            return 0; // corrupted file
        }
    }
    fclose(fp);
    return 1;
}
void inputUserCredentials()
{
    if (loadUsersFromFile())
    {
        printf("User credentials loaded successfully!\n");
        return;
    }

    printf("Set credentials for system users:\n");
    for (int i = 0; i < MAX_USER; i++)
    {
        if (i == 0)
        {
            printf("\n--- Admin Credentials ---\n");
            strcpy(users[i].role, "Admin");
        }
        else
        {
            printf("\n--- HR Credentials ---\n");
            strcpy(users[i].role, "HR");
        }
        printf("Set username: ");
        fgets(users[i].username, MAX_LEN, stdin);
        users[i].username[strcspn(users[i].username, "\n")] = '\0';

        printf("Set password: ");
        fgets(users[i].password, MAX_LEN, stdin);
        users[i].password[strcspn(users[i].password, "\n")] = '\0';
    }

    saveUsersToFile();
    printf("\nCredentials saved successfully!\n");
}
int login(const char *role)
{
    char username[MAX_LEN], password[MAX_LEN];

    printf("Enter username: ");
    fgets(username, MAX_LEN, stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    fgets(password, MAX_LEN, stdin);
    password[strcspn(password, "\n")] = '\0';

    for (int i = 0; i < MAX_USER; i++)
    {
        if (strcmp(users[i].role, role) == 0 &&
            strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void writeEmployeeToFile(FILE *fp, struct Employee *emp)
{
    fprintf(fp, "ID: %d\n", emp->id);
    fprintf(fp, "Name: %s\n", emp->name);
    fprintf(fp, "Age: %d\n", emp->age);
    fprintf(fp, "Contact number (+880...): %s\n", emp->contact);
    fprintf(fp, "Email: %s\n", emp->email);
    fprintf(fp, "Joining date (dd-mm-yyyy): %s\n", emp->joining_date);
    fprintf(fp, "Position: %s\n", emp->position);
    fprintf(fp, "Base salary: %.2f\n", emp->base_salary);
    fprintf(fp, "Attendance (out of %d): %d\n", WORK_DAYS, emp->attendance);
    fprintf(fp, "Bonus: %.2f\n", emp->bonus);
    fprintf(fp, "Deduction: %.2f\n", emp->deduction);
    fprintf(fp, "-------------------------\n");
}

void addEmployee()
{
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    struct Employee emp;

    printf("Enter ID: ");
    scanf("%d", &emp.id);
    clearInputBuffer();

    printf("Enter name: ");
    fgets(emp.name, sizeof(emp.name), stdin);
    emp.name[strcspn(emp.name, "\n")] = '\0';

    printf("Enter age: ");
    scanf("%d", &emp.age);
    clearInputBuffer();

    printf("Enter contact number (+880...): ");
    fgets(emp.contact, sizeof(emp.contact), stdin);
    emp.contact[strcspn(emp.contact, "\n")] = '\0';

    printf("Enter email: ");
    fgets(emp.email, sizeof(emp.email), stdin);
    emp.email[strcspn(emp.email, "\n")] = '\0';

    printf("Enter joining date (dd-mm-yyyy): ");
    fgets(emp.joining_date, sizeof(emp.joining_date), stdin);
    emp.joining_date[strcspn(emp.joining_date, "\n")] = '\0';

    printf("Enter position: ");
    fgets(emp.position, sizeof(emp.position), stdin);
    emp.position[strcspn(emp.position, "\n")] = '\0';

    printf("Enter base salary: ");
    scanf("%f", &emp.base_salary);
    clearInputBuffer();

    printf("Enter attendance (out of %d): ", WORK_DAYS);
    scanf("%d", &emp.attendance);
    clearInputBuffer();

    emp.bonus = (emp.attendance >= 25) ? emp.base_salary * 0.10f : 0.0f;
    emp.deduction = (WORK_DAYS - emp.attendance) * (emp.base_salary / WORK_DAYS);

    writeEmployeeToFile(fp, &emp);

    fclose(fp);
    printf("Employee added successfully!\n");
}

int readEmployeeBlock(FILE *fp, struct Employee *emp)
{

    char line[LINE_LEN];

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    if (sscanf(line, "ID: %d", &emp->id) != 1)
        return 0;

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Name: %[^\n]", emp->name);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Age: %d", &emp->age);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Contact number (+880...): %[^\n]", emp->contact);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Email: %[^\n]", emp->email);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Joining date (dd-mm-yyyy): %[^\n]", emp->joining_date);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Position: %[^\n]", emp->position);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Base salary: %f", &emp->base_salary);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Attendance (out of %*d): %d", &emp->attendance);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Bonus: %f", &emp->bonus);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;
    sscanf(line, "Deduction: %f", &emp->deduction);

    if (fgets(line, LINE_LEN, fp) == NULL)
        return 0;

    return 1;
}

void viewEmployees()
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp)
    {
        printf("No employee data found!\n");
        return;
    }

    struct Employee emp;

    printf("\n--- All Employee Records ---\n\n");

    while (readEmployeeBlock(fp, &emp))
    {
        printf("ID: %d\n", emp.id);
        printf("Name: %s\n", emp.name);
        printf("Age: %d\n", emp.age);
        printf("Contact number (+880...): %s\n", emp.contact);
        printf("Email: %s\n", emp.email);
        printf("Joining date (dd-mm-yyyy): %s\n", emp.joining_date);
        printf("Position: %s\n", emp.position);
        printf("Base salary: %.2f\n", emp.base_salary);
        printf("Attendance (out of %d): %d\n", WORK_DAYS, emp.attendance);
        printf("Bonus: %.2f\n", emp.bonus);
        printf("Deduction: %.2f\n", emp.deduction);
        printf("-------------------------\n");
    }

    fclose(fp);
}

void updateEmployee()
{
    FILE *fptr = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fptr || !temp)
    {
        printf("File error!\n");
        if (fptr)
            fclose(fptr);
        if (temp)
            fclose(temp);
        return;
    }

    int id, found = 0;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    struct Employee emp;
    while (readEmployeeBlock(fptr, &emp))
    {
        if (emp.id == id)
        {
            printf("Update fields - press ENTER to keep old value\n");
            char tempStr[100];

            printf("Name [%s]: ", emp.name);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
            {
                tempStr[strcspn(tempStr, "\n")] = '\0';
                strcpy(emp.name, tempStr);
            }

            printf("Age [%d]: ", emp.age);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
                emp.age = atoi(tempStr);

            printf("Contact [%s]: ", emp.contact);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
            {
                tempStr[strcspn(tempStr, "\n")] = '\0';
                strcpy(emp.contact, tempStr);
            }

            printf("Email [%s]: ", emp.email);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
            {
                tempStr[strcspn(tempStr, "\n")] = '\0';
                strcpy(emp.email, tempStr);
            }

            printf("Joining Date [%s]: ", emp.joining_date);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
            {
                tempStr[strcspn(tempStr, "\n")] = '\0';
                strcpy(emp.joining_date, tempStr);
            }

            printf("Position [%s]: ", emp.position);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
            {
                tempStr[strcspn(tempStr, "\n")] = '\0';
                strcpy(emp.position, tempStr);
            }

            printf("Base Salary [%.2f]: ", emp.base_salary);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
                emp.base_salary = atof(tempStr);

            printf("Attendance (out of %d) [%d]: ", WORK_DAYS, emp.attendance);
            fgets(tempStr, sizeof(tempStr), stdin);
            if (tempStr[0] != '\n')
                emp.attendance = atoi(tempStr);

            emp.bonus = (emp.attendance >= 25) ? emp.base_salary * 0.10f : 0.0f;
            emp.deduction = (WORK_DAYS - emp.attendance) * (emp.base_salary / WORK_DAYS);

            found = 1;
        }
        writeEmployeeToFile(temp, &emp);
    }

    fclose(fptr);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("Employee updated successfully.\n");
    else
        printf("Employee with ID %d not found.\n", id);
}

void updateAttendance()
{
    FILE *fptr = fopen(FILE_NAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fptr || !temp)
    {
        printf("File error!\n");
        if (fptr)
            fclose(fptr);
        if (temp)
            fclose(temp);
        return;
    }

    int id, found = 0;
    printf("Enter ID to update attendance: ");
    scanf("%d", &id);
    clearInputBuffer();

    struct Employee emp;
    while (readEmployeeBlock(fptr, &emp))
    {
        if (emp.id == id)
        {
            printf("Current attendance: %d (out of %d)\n", emp.attendance, WORK_DAYS);
            printf("Enter new attendance: ");
            scanf("%d", &emp.attendance);
            clearInputBuffer();

            emp.bonus = (emp.attendance >= 25) ? emp.base_salary * 0.10f : 0.0f;
            emp.deduction = (WORK_DAYS - emp.attendance) * (emp.base_salary / WORK_DAYS);

            found = 1;
        }
        writeEmployeeToFile(temp, &emp);
    }

    fclose(fptr);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("Attendance updated successfully.\n");
    else
        printf("Employee with ID %d not found.\n", id);
}

// If attendance is 25 days or more, give 10% bonus of base salary; else no bonus
// Deduct salary for each absent day (daily salary * number of absent days)
void calculateSalary()
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp)
    {
        printf("No employee data found!\n");
        return;
    }

    struct Employee emp;
    char line[LINE_LEN];

    printf("\n--- Salaries ---\n");

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "ID:", 3) == 0)
        {
            sscanf(line, "ID: %d", &emp.id);
        }
        else if (strncmp(line, "Name:", 5) == 0)
        {
            sscanf(line, "Name: %[^\n]", emp.name);
        }
        else if (strncmp(line, "Age:", 4) == 0)
        {
            sscanf(line, "Age: %d", &emp.age);
        }
        else if (strncmp(line, "Contact", 7) == 0)
        {
            sscanf(line, "Contact number (+880...): %[^\n]", emp.contact);
        }
        else if (strncmp(line, "Email:", 6) == 0)
        {
            sscanf(line, "Email: %[^\n]", emp.email);
        }
        else if (strncmp(line, "Joining date", 12) == 0)
        {
            sscanf(line, "Joining date (dd-mm-yyyy): %[^\n]", emp.joining_date);
        }
        else if (strncmp(line, "Position:", 9) == 0)
        {
            sscanf(line, "Position: %[^\n]", emp.position);
        }
        else if (strncmp(line, "Base salary:", 12) == 0)
        {
            sscanf(line, "Base salary: %f", &emp.base_salary);
        }
        else if (strncmp(line, "Attendance", 10) == 0)
        {
            sscanf(line, "Attendance (out of %*d): %d", &emp.attendance);

            emp.bonus = (emp.attendance >= 25) ? emp.base_salary * 0.10f : 0.0f;
            emp.deduction = (WORK_DAYS - emp.attendance) * (emp.base_salary / WORK_DAYS);

            float daily_salary = emp.base_salary / WORK_DAYS;
            float net_salary = (daily_salary * emp.attendance) + emp.bonus - emp.deduction;

            printf("ID: %d\nName: %s\nNet Salary: %.2f Tk\n----------------------\n",
                   emp.id, emp.name, net_salary);
        }
    }

    fclose(fp);
}

void adminMenu()
{
    int choice;
    do
    {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Employee\n");
        printf("2. Update Employee\n");
        printf("3. Update Attendance\n");
        printf("4. Switch Role\n");
        printf("5. Exit Admin\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addEmployee();
            break;
        case 2:
            updateEmployee();
            break;
        case 3:
            updateAttendance();
            break;
        case 4:
            return;
        case 5:
            printf("Exiting Admin panel.\n");
            exit(0);
        default:
            printf("Invalid option!\n");
        }
    } while (1);
}

void hrMenu()
{
    int choice;
    do
    {
        printf("\n--- HR Menu ---\n");
        printf("1. View Employees\n");
        printf("2. Update Attendance\n");
        printf("3. Calculate Salaries\n");
        printf("4. Switch Role\n");
        printf("5. Exit HR\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            viewEmployees();
            break;
        case 2:
            updateAttendance();
            break;
        case 3:
            calculateSalary();
            break;
        case 4:
            return; // switch role
        case 5:
            printf("Exiting HR panel.\n");
            exit(0);
        default:
            printf("Invalid option!\n");
        }
    } while (1);
}

int main()
{
    int role, loggedIn;

    inputUserCredentials();

    while (1)
    {
        printf("\n--- Employee Management System ---\n");
        printf("Select Role:\n1. Admin\n2. HR\n3. Exit Program\n");
        printf("Choice: ");
        scanf("%d", &role);
        clearInputBuffer();

        if (role == 1)
        {
            loggedIn = login("Admin");
            if (loggedIn)
            {
                printf("Login successful! Welcome Admin.\n");
                adminMenu();
            }
            else
            {
                printf("Invalid Admin credentials.\n");
            }
        }
        else if (role == 2)
        {
            loggedIn = login("HR");
            if (loggedIn)
            {
                printf("Login successful! Welcome HR.\n");
                hrMenu();
            }
            else
            {
                printf("Invalid HR credentials.\n");
            }
        }
        else if (role == 3)
        {
            printf("Exiting program. Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid role selected!\n");
        }
    }

    return 0;
}
