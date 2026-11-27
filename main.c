#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define CAT 5
#define FILE_NAME "expenses.dat"

struct Expense
{
    char name[50];
    int category;
    float amount;
};

char categories[CAT][20] = {
    "Food", "Travel", "Books",
    "Entertainment", "Others"
};

void addExpense(struct Expense e[], int *n);
void viewExpenses(struct Expense e[], int n);
float totalExpense(struct Expense e[], int n);
void categoryWise(struct Expense e[], int n);
void highestCategory(struct Expense e[], int n);
void checkBudget(struct Expense e[], int n, float budget);
void updateBudget(float *budget);
void updateExpense(struct Expense e[], int n);
void deleteExpense(struct Expense e[], int *n);
void saveExpenses(struct Expense e[], int n);
void loadExpenses(struct Expense e[], int *n);
int getInt(char message[]);
float getFloat(char message[]);

int main()
{
    struct Expense e[MAX];
    int n = 0, choice;
    float budget = 0;

    loadExpenses(e, &n);

    printf("\n=================================\n");
    printf("       SPENDWISE TRACKER\n");
    printf("=================================\n");

    budget = getFloat("Enter monthly budget: Rs. ");

    while(budget <= 0)
    {
        printf("Budget must be positive!\n");
        budget = getFloat("Enter monthly budget: Rs. ");
    }

    do
    {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Add Expense\n");
        printf("2. View Expenses\n");
        printf("3. Total Spending\n");
        printf("4. Category-wise Spending\n");
        printf("5. Highest Spending Category\n");
        printf("6. Remaining Budget\n");
        printf("7. Update Budget\n");
        printf("8. Update Expense\n");
        printf("9. Delete Expense\n");
        printf("10. Exit\n");

        choice = getInt("\nEnter your choice: ");

        switch(choice)
        {
            case 1:
                addExpense(e, &n);
                break;

            case 2:
                viewExpenses(e, n);
                break;

            case 3:
                printf("\nTotal Spending: Rs. %.2f\n",
                       totalExpense(e, n));
                break;

            case 4:
                categoryWise(e, n);
                break;

            case 5:
                highestCategory(e, n);
                break;

            case 6:
                checkBudget(e, n, budget);
                break;

            case 7:
                updateBudget(&budget);
                break;

            case 8:
                updateExpense(e, n);
                break;

            case 9:
                deleteExpense(e, &n);
                break;

            case 10:
                saveExpenses(e, n);
                printf("\nThank you for using SpendWise!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while(choice != 10);

    return 0;
}

/* Integer input validation */
int getInt(char message[])
{
    char input[100], extra;
    int value;

    while(1)
    {
        printf("%s", message);

        if(fgets(input, sizeof(input), stdin) == NULL)
            exit(1);

        if(sscanf(input, " %d %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter an integer.\n");
    }
}

/* Float input validation */
float getFloat(char message[])
{
    char input[100], extra;
    float value;

    while(1)
    {
        printf("%s", message);

        if(fgets(input, sizeof(input), stdin) == NULL)
            exit(1);

        if(sscanf(input, " %f %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter a number.\n");
    }
}

/* Add expense */
void addExpense(struct Expense e[], int *n)
{
    int category;

    if(*n >= MAX)
    {
        printf("\nExpense limit reached!\n");
        return;
    }

    printf("\nEnter expense name: ");

    if(fgets(e[*n].name, sizeof(e[*n].name), stdin) == NULL)
        return;

    e[*n].name[strcspn(e[*n].name, "\n")] = '\0';

    if(strlen(e[*n].name) == 0)
    {
        printf("Expense name cannot be empty!\n");
        return;
    }

    printf("\nSelect Category:\n");

    for(int i = 0; i < CAT; i++)
        printf("%d. %s\n", i + 1, categories[i]);

    category = getInt("Enter category number: ");

    if(category < 1 || category > CAT)
    {
        printf("Invalid category!\n");
        return;
    }

    e[*n].category = category - 1;

    e[*n].amount = getFloat("Enter amount: Rs. ");

    if(e[*n].amount <= 0)
    {
        printf("Invalid amount!\n");
        return;
    }

    (*n)++;

    saveExpenses(e, *n);

    printf("\nExpense added successfully!\n");
}

/* View expenses */
void viewExpenses(struct Expense e[], int n)
{
    if(n == 0)
    {
        printf("\nNo expenses recorded!\n");
        return;
    }

    printf("\n========== EXPENSE HISTORY ==========\n");

    for(int i = 0; i < n; i++)
    {
        printf("\n%d. %s", i + 1, e[i].name);
        printf("\nCategory: %s", categories[e[i].category]);
        printf("\nAmount: Rs. %.2f\n", e[i].amount);
    }
}

/* Calculate total */
float totalExpense(struct Expense e[], int n)
{
    float total = 0;

    for(int i = 0; i < n; i++)
        total += e[i].amount;

    return total;
}

/* Category-wise spending */
void categoryWise(struct Expense e[], int n)
{
    float sum[CAT] = {0};

    for(int i = 0; i < n; i++)
        sum[e[i].category] += e[i].amount;

    printf("\n========== CATEGORY REPORT ==========\n");

    for(int i = 0; i < CAT; i++)
        printf("%s: Rs. %.2f\n", categories[i], sum[i]);
}

/* Highest spending category */
void highestCategory(struct Expense e[], int n)
{
    float sum[CAT] = {0}, highest = 0;
    int index = -1;

    if(n == 0)
    {
        printf("\nNo expenses recorded!\n");
        return;
    }

    for(int i = 0; i < n; i++)
        sum[e[i].category] += e[i].amount;

    for(int i = 0; i < CAT; i++)
    {
        if(sum[i] > highest)
        {
            highest = sum[i];
            index = i;
        }
    }

    printf("\nHighest Spending Category: %s\n",
           categories[index]);

    printf("Amount: Rs. %.2f\n", highest);
}

/* Check budget */
void checkBudget(struct Expense e[], int n, float budget)
{
    float total = totalExpense(e, n);
    float remaining = budget - total;

    printf("\nMonthly Budget: Rs. %.2f\n", budget);
    printf("Total Spending: Rs. %.2f\n", total);

    if(remaining >= 0)
    {
        printf("Remaining Budget: Rs. %.2f\n", remaining);
        printf("You are within your budget!\n");
    }
    else
    {
        printf("Budget Exceeded By: Rs. %.2f\n", -remaining);
        printf("WARNING! You exceeded your budget!\n");
    }
}

/* Update budget */
void updateBudget(float *budget)
{
    float newBudget = getFloat("\nEnter new monthly budget: Rs. ");

    if(newBudget <= 0)
    {
        printf("Invalid budget!\n");
        return;
    }

    *budget = newBudget;

    printf("Budget updated successfully!\n");
}

/* Update expense */
void updateExpense(struct Expense e[], int n)
{
    int id, category;

    if(n == 0)
    {
        printf("\nNo expenses to update!\n");
        return;
    }

    viewExpenses(e, n);

    id = getInt("\nEnter expense number to update: ");

    if(id < 1 || id > n)
    {
        printf("Invalid expense number!\n");
        return;
    }

    id--;

    printf("Enter new expense name: ");

    if(fgets(e[id].name, sizeof(e[id].name), stdin) == NULL)
        return;

    e[id].name[strcspn(e[id].name, "\n")] = '\0';

    if(strlen(e[id].name) == 0)
    {
        printf("Name cannot be empty!\n");
        return;
    }

    printf("\nSelect new category:\n");

    for(int i = 0; i < CAT; i++)
        printf("%d. %s\n", i + 1, categories[i]);

    category = getInt("Enter category number: ");

    if(category < 1 || category > CAT)
    {
        printf("Invalid category!\n");
        return;
    }

    e[id].category = category - 1;

    float amount = getFloat("Enter new amount: Rs. ");

    if(amount <= 0)
    {
        printf("Invalid amount!\n");
        return;
    }

    e[id].amount = amount;

    saveExpenses(e, n);

    printf("\nExpense updated successfully!\n");
}

/* Delete expense */
void deleteExpense(struct Expense e[], int *n)
{
    int id;

    if(*n == 0)
    {
        printf("\nNo expenses to delete!\n");
        return;
    }

    viewExpenses(e, *n);

    id = getInt("\nEnter expense number to delete: ");

    if(id < 1 || id > *n)
    {
        printf("Invalid expense number!\n");
        return;
    }

    id--;

    for(int i = id; i < *n - 1; i++)
        e[i] = e[i + 1];

    (*n)--;

    saveExpenses(e, *n);

    printf("\nExpense deleted successfully!\n");
}

/* Save expenses in binary file */
void saveExpenses(struct Expense e[], int n)
{
    FILE *fp = fopen(FILE_NAME, "wb");

    if(fp == NULL)
    {
        printf("Error saving expenses!\n");
        return;
    }

    fwrite(&n, sizeof(int), 1, fp);
    fwrite(e, sizeof(struct Expense), n, fp);

    fclose(fp);
}

/* Load expenses from binary file */
void loadExpenses(struct Expense e[], int *n)
{
    FILE *fp = fopen(FILE_NAME, "rb");

    if(fp == NULL)
        return;

    int count;

    if(fread(&count, sizeof(int), 1, fp) != 1 ||
       count < 0 || count > MAX)
    {
        printf("Invalid expense file!\n");
        fclose(fp);
        return;
    }

    if(fread(e, sizeof(struct Expense), count, fp) != (size_t)count)
    {
        printf("Error reading expense file!\n");
        fclose(fp);
        return;
    }

    *n = count;

    fclose(fp);
}

