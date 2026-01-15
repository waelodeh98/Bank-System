#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct st_User {
    string User_Name;
    string Password;
    int Permissions;
    bool Mark_For_Delete = false;
};

enum en_Transactions_Menu_Options {
    e_Deposit = 1, e_Withdraw = 2,
    e_Show_Total_Balance = 3, e_Show_Main_Menu = 4
};

enum en_Manage_Users_Menu_Options {
    e_List_Users = 1, e_Add_New_User = 2, e_Delete_User = 3,
    e_Update_User = 4, e_Find_User = 5, e_Main_Menu = 6
};

enum en_Main_Menu_Options {
    e_List_Clients = 1,
    e_Add_New_Client = 2, e_Delete_Client = 3,
    e_Update_Client = 4, e_Find_Client = 5,
    e_Show_Transactions_Menu = 6, e_Manage_Users = 7,
    e_Exit = 8
};

enum en_Main_Menu_Permissions {
    e_All = -1, P_List_clients = 1, P_Add_New_Client = 2, P_Delete_Client = 4,
    P_Update_Clients = 8, P_Find_Client = 16, P_Transactions = 32, P_Manage_Users = 64
};

const string Clients_File_Name = "Clients.txt";

const string Users_File_Name = "Users.txt";

st_User Current_User;

void Show_Main_Menu();

void Show_Transactions_Menu();

void Show_Manage_Users_Menu();

bool Check_Access_Permission(en_Main_Menu_Permissions Permission);

void Login();

struct st_Client
{
    string Account_Number;
    string Pin_Code;
    string Name;
    string Phone;
    double Account_Balance;
    bool Mark_For_Delete = false;


};

vector<string> Split_String(string S1, string Delimiter)
{

    vector<string> V_String;

    short position = 0;
    string S_Word;

    while ((position = S1.find(Delimiter)) != std::string::npos)
    {
        S_Word = S1.substr(0, position);
        if (S_Word != "")
        {
            V_String.push_back(S_Word);
        }

        S1.erase(0, position + Delimiter.length());
    }

    if (S1 != "")
    {
        V_String.push_back(S1);
    }

    return V_String;

}

st_User Convert_User_Line_To_Record(string Line, string Separator = "#//#") {
    st_User User;
    vector <string> V_User_Data;

    V_User_Data = Split_String(Line, Separator);

    User.User_Name = V_User_Data[0];
    User.Password = V_User_Data[1];
    User.Permissions = stoi(V_User_Data[2]);

    return User;
}

st_Client Convert_Line_To_Record(string Line, string Seperator = "#//#")
{

    st_Client Client;
    vector<string> V_Client_Data;

    V_Client_Data = Split_String(Line, Seperator);

    Client.Account_Number = V_Client_Data[0];
    Client.Pin_Code = V_Client_Data[1];
    Client.Name = V_Client_Data[2];
    Client.Phone = V_Client_Data[3];
    Client.Account_Balance = stod(V_Client_Data[4]);//cast string to double


    return Client;

}

string Convert_Record_To_Line(st_Client Client, string Seperator = "#//#")
{

    string st_Client_Record = "";

    st_Client_Record += Client.Account_Number + Seperator;
    st_Client_Record += Client.Pin_Code + Seperator;
    st_Client_Record += Client.Name + Seperator;
    st_Client_Record += Client.Phone + Seperator;
    st_Client_Record += to_string(Client.Account_Balance);

    return st_Client_Record;

}

string Convert_User_Record_To_Line(st_User User, string Separator = "#//#") {
    string st_User_Record = "";
    st_User_Record += User.User_Name + Separator;
    st_User_Record += User.Password + Separator;
    st_User_Record += to_string(User.Permissions);
    return st_User_Record;
}

bool Client_Exists_By_Account_Number(string Account_Number, string File_Name)
{

    vector <st_Client> V_Clients;

    fstream My_File;
    My_File.open(File_Name, ios::in);//read Mode

    if (My_File.is_open())
    {

        string Line;
        st_Client Client;

        while (getline(My_File, Line))
        {

            Client = Convert_Line_To_Record(Line);
            if (Client.Account_Number == Account_Number)
            {
                My_File.close();
                return true;
            }


            V_Clients.push_back(Client);
        }

        My_File.close();

    }

    return false;
}

bool User_Exist_By_User_Name(string User_Name, string File_Name) {
    fstream My_File;
    My_File.open(File_Name, ios::in);
    if (My_File.is_open()) {
        string Line;
        st_User User;

        while (getline(My_File, Line)) {
            User = Convert_User_Line_To_Record(Line);
            if (User.User_Name == User_Name) {
                My_File.close();
                return true;
            }

        }
    }
    return false;
}

st_Client Read_New_Client()
{
    st_Client Client;

    cout << "Enter Account Number? ";

    getline(cin >> ws, Client.Account_Number);

    while (Client_Exists_By_Account_Number(Client.Account_Number, Clients_File_Name))
    {
        cout << "\nClient with [" << Client.Account_Number << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.Account_Number);
    }


    cout << "Enter Pin Code? ";
    getline(cin, Client.Pin_Code);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.Account_Balance;

    return Client;
}

int Read_Permissions_To_Set() {
    int Permissions = 0;
    char Answer = 'N';

    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n ";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') {
        Permissions += en_Main_Menu_Permissions::P_List_clients;
    }

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += en_Main_Menu_Permissions::P_Add_New_Client;
    }

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += en_Main_Menu_Permissions::P_Delete_Client;
    }

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += en_Main_Menu_Permissions::P_Update_Clients;
    }

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += en_Main_Menu_Permissions::P_Find_Client;
    }

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += en_Main_Menu_Permissions::P_Transactions;
    }

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += en_Main_Menu_Permissions::P_Manage_Users;
    }

    return Permissions;
}

st_User Read_New_User() {
    st_User User;

    cout << "Enter User Name ? ";
    getline(cin >> ws, User.User_Name);

    while (User_Exist_By_User_Name(User.User_Name, Users_File_Name)) {
        cout << "\nUser with [" << User.User_Name << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.User_Name);
    }

    cout << "Enter Password ? ";
    getline(cin >> ws, User.Password);

    User.Permissions = Read_Permissions_To_Set();

    return User;
}

vector <st_User> Load_Users_Data_From_File(string File_Name) {
    vector <st_User> V_Users;
    fstream My_File;
    My_File.open(File_Name, ios::in);

    if (My_File.is_open()) {
        string Line;
        st_User User;
        while (getline(My_File, Line)) {
            User = Convert_User_Line_To_Record(Line);
            V_Users.push_back(User);
        }
        My_File.close();
    }
    return V_Users;
}

vector <st_Client> Load_Cleints_Data_From_File(string File_Name)
{
    vector <st_Client> V_Clients;

    fstream My_File;
    My_File.open(File_Name, ios::in);//read Mode

    if (My_File.is_open())
    {

        string Line;
        st_Client Client;

        while (getline(My_File, Line))
        {

            Client = Convert_Line_To_Record(Line);

            V_Clients.push_back(Client);
        }

        My_File.close();

    }
    return V_Clients;
}

void Print_Client_Record_Line(st_Client Client)
{
    cout << "| " << setw(15) << left << Client.Account_Number;
    cout << "| " << setw(10) << left << Client.Pin_Code;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.Account_Balance;

}

void Print_User_Record_Line(st_User User) {
    cout << "| " << setw(15) << left << User.User_Name;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;
}

void Print_Client_Record_Balance_Line(st_Client Client)
{
    cout << "| " << setw(15) << left << Client.Account_Number;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Account_Balance;
}

void Show_Access_Denied_Message() {
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

void Show_All_Clients_Screen()
{
    if (!Check_Access_Permission(en_Main_Menu_Permissions::P_List_clients)) {
        Show_Access_Denied_Message();
        return;
    }

    vector <st_Client> V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);

    cout << "\n\t\t\t\t\tClient List (" << V_Clients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (V_Clients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (st_Client Client : V_Clients)
        {

            Print_Client_Record_Line(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void Show_All_Users_Screen() {
    vector <st_User> V_Users = Load_Users_Data_From_File(Users_File_Name);
    cout << "\n\t\t\t\t\tUsers List (" << V_Users.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (V_Users.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (st_User User : V_Users)
        {

            Print_User_Record_Line(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void Show_Total_Balances()
{
    vector <st_Client> V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);

    cout << "\n\t\t\t\t\tBalances List (" << V_Clients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double Total_Balances = 0;

    if (V_Clients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (st_Client Client : V_Clients)
        {

            Print_Client_Record_Balance_Line(Client);
            Total_Balances += Client.Account_Balance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << Total_Balances;
}

void Print_Client_Card(st_Client Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.Account_Number;
    cout << "\nPin Code     : " << Client.Pin_Code;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.Account_Balance;
    cout << "\n-----------------------------------\n";
}

void Print_User_Card(st_User User) {
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.User_Name;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";
}

bool Find_Client_By_Account_Number(string Account_Number, vector <st_Client> V_Clients, st_Client& Client)
{
    for (st_Client C : V_Clients)
    {
        if (C.Account_Number == Account_Number)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool Find_User_By_User_Name(string User_Name, vector <st_User> V_Users, st_User& User) {
    for (st_User U : V_Users) {
        if (U.User_Name == User_Name) {
            User = U;
            return true;
        }
    }
    return false;
}

bool Find_User_By_User_Name_And_Password(string User_Name, string Password, st_User& Current_User) {
    vector <st_User> V_Users = Load_Users_Data_From_File(Users_File_Name);
    for (st_User User : V_Users) {
        if (User.User_Name == User_Name && User.Password == Password) {
            Current_User = User;
            return true;
        }
    }
    return false;
}

st_Client Change_Client_Record(string Account_Number)
{
    st_Client Client;

    Client.Account_Number = Account_Number;

    cout << "\n\nEnter Pin Code ? ";
    getline(cin >> ws, Client.Pin_Code);

    cout << "Enter Name ? ";
    getline(cin, Client.Name);

    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance ? ";
    cin >> Client.Account_Balance;

    return Client;
}

st_User Change_User_Record(string User_Name) {
    st_User User;
    User.User_Name = User_Name;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = Read_Permissions_To_Set();

    return User;
}

bool Mark_Client_For_Delete_By_Account_Number(string Account_Number, vector <st_Client>& V_Clients)
{
    for (st_Client& C : V_Clients)
    {
        if (C.Account_Number == Account_Number)
        {
            C.Mark_For_Delete = true;
            return true;
        }
    }
    return false;
}

bool Mark_User_For_Delete_By_User_Name(string User_Name, vector <st_User>& V_Users) {
    for (st_User& User : V_Users) {
        if (User.User_Name == User_Name) {
            User.Mark_For_Delete = true;
            return true;
        }
    }
    return false;
}

vector <st_Client> Save_Cleints_Data_To_File(string File_Name, vector <st_Client> V_Clients)
{
    fstream My_File;
    My_File.open(File_Name, ios::out);

    string Data_Line;

    if (My_File.is_open())
    {
        for (st_Client C : V_Clients)
        {
            if (C.Mark_For_Delete == false)
            {
                Data_Line = Convert_Record_To_Line(C);
                My_File << Data_Line << endl;
            }
        }
        My_File.close();
    }
    return V_Clients;
}

vector <st_User> Save_Users_Data_To_File(string File_Name, vector <st_User> V_Users) {
    fstream My_File;
    My_File.open(File_Name, ios::out);
    string Data_Line;
    if (My_File.is_open()) {
        for (st_User User : V_Users) {

            if (User.Mark_For_Delete == false) {
                Data_Line = Convert_User_Record_To_Line(User);
                My_File << Data_Line << endl;
            }
        }
        My_File.close();
    }

    return V_Users;
}

void Add_Data_Line_To_File(string File_Name, string  st_Data_Line)
{
    fstream My_File;
    My_File.open(File_Name, ios::out | ios::app);

    if (My_File.is_open())
    {
        My_File << st_Data_Line << endl;

        My_File.close();
    }
}

void Add_New_Client()
{
    st_Client Client;
    Client = Read_New_Client();
    Add_Data_Line_To_File(Clients_File_Name, Convert_Record_To_Line(Client));
}

void Add_New_User() {
    st_User User;
    User = Read_New_User();
    Add_Data_Line_To_File(Users_File_Name, Convert_User_Record_To_Line(User));
}

void Add_New_Clients()
{
    char Add_More = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        Add_New_Client();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> Add_More;

    } while (toupper(Add_More) == 'Y');
}

void Add_New_Users() {
    char Add_More = 'Y';
    do {
        cout << "Adding New User:\n\n";

        Add_New_User();

        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
        cin >> Add_More;

    } while (toupper(Add_More) == 'Y');
}

bool Delete_Client_By_Account_Number(string Account_Number, vector <st_Client>& V_Clients)
{
    st_Client Client;
    char Answer = 'n';

    if (Find_Client_By_Account_Number(Account_Number, V_Clients, Client))
    {
        Print_Client_Card(Client);
        cout << "\n\nAre you sure you want delete this client? Y/N ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Mark_Client_For_Delete_By_Account_Number(Account_Number, V_Clients);
            Save_Cleints_Data_To_File(Clients_File_Name, V_Clients);

            V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << Account_Number << ") is Not Found!";
        return false;
    }
}

bool Delete_User_By_User_Name(string User_Name, vector <st_User>& V_Users) {
    if (User_Name == "Admin") {
        cout << "\n\nYou cannot Delete This User.";
        return false;
    }

    st_User User;
    char Answer = 'N';

    if (Find_User_By_User_Name(User_Name, V_Users, User)) {
        Print_User_Card(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y') {
            Mark_User_For_Delete_By_User_Name(User_Name, V_Users);
            Save_Users_Data_To_File(Users_File_Name, V_Users);
            vector <st_User> V_Users = Load_Users_Data_From_File(Users_File_Name);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else {
        cout << "\nUser with Username (" << User_Name << ") is Not Found!";
        return false;
    }
}

bool Update_Client_By_Account_Number(string Account_Number, vector <st_Client>& V_Clients)
{
    st_Client Client;
    char Answer = 'n';

    if (Find_Client_By_Account_Number(Account_Number, V_Clients, Client))
    {
        Print_Client_Card(Client);
        cout << "\n\nAre you sure you want update this client? Y/N ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (st_Client& C : V_Clients)
            {
                if (C.Account_Number == Account_Number)
                {
                    C = Change_Client_Record(Account_Number);
                    break;
                }
            }
            Save_Cleints_Data_To_File(Clients_File_Name, V_Clients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << Account_Number << ") is Not Found!";
        return false;
    }
}

bool Update_User_By_User_Name(string User_Name, vector <st_User>& V_Users) {
    st_User User;
    char Answer = 'N';

    if (Find_User_By_User_Name(User_Name, V_Users, User)) {
        Print_User_Card(User);

        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (st_User& U : V_Users) {
                if (U.User_Name == User_Name) {
                    U = Change_User_Record(User_Name);
                    break;
                }
            }
            Save_Users_Data_To_File(Users_File_Name, V_Users);
            return true;
        }
    }
    else {
        cout << "\nUser with Account Number (" << User_Name << ") is Not Found!";
        return false;
    }
}

bool Deposit_Balance_To_Client_By_Account_Number(string Account_Number, double Amount, vector <st_Client>& V_Clients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (st_Client& C : V_Clients)
        {
            if (C.Account_Number == Account_Number)
            {
                C.Account_Balance += Amount;
                Save_Cleints_Data_To_File(Clients_File_Name, V_Clients);
                cout << "\n\nDone Successfully. New balance is: " << C.Account_Balance;

                return true;
            }
        }
        return false;
    }
}

string Read_Client_Account_Number()
{
    string Account_Number = "";

    cout << "\nPlease enter Account Number ? ";
    cin >> Account_Number;
    return Account_Number;

}

string Read_User_Name() {
    string User_Name = "";
    cout << "\nPlease enter Username? ";
    cin >> User_Name;
    return User_Name;
}

void Show_List_Users_Screen() {
    Show_All_Users_Screen();
}

void Show_Add_New_User_Screen() {
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

    Add_New_Users();
}

void Show_Delete_User_Screen() {
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Users Screen";
    cout << "\n-----------------------------------\n";
    vector <st_User> V_Users = Load_Users_Data_From_File(Users_File_Name);
    string User_Name = Read_User_Name();
    Delete_User_By_User_Name(User_Name, V_Users);
}

void Show_Update_User_screen() {
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Users Screen";
    cout << "\n-----------------------------------\n";
    vector <st_User> V_Users = Load_Users_Data_From_File(Users_File_Name);
    string User_Name = Read_User_Name();
    Update_User_By_User_Name(User_Name, V_Users);
}

void Show_Delete_Client_Screen()
{
    if (!Check_Access_Permission(en_Main_Menu_Permissions::P_Delete_Client)) {
        Show_Access_Denied_Message();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <st_Client> V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);
    string Account_Number = Read_Client_Account_Number();
    Delete_Client_By_Account_Number(Account_Number, V_Clients);
}

void Show_Update_Client_Screen()
{
    if (!Check_Access_Permission(en_Main_Menu_Permissions::P_Update_Clients)) {
        Show_Access_Denied_Message();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <st_Client> V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);
    string Account_Number = Read_Client_Account_Number();
    Update_Client_By_Account_Number(Account_Number, V_Clients);
}

void Show_Add_New_Clients_Screen()
{
    if (!Check_Access_Permission(en_Main_Menu_Permissions::P_Add_New_Client)) {
        Show_Access_Denied_Message();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    Add_New_Clients();
}

void Show_Find_Client_Screen()
{
    if (!Check_Access_Permission(en_Main_Menu_Permissions::P_Find_Client)) {
        Show_Access_Denied_Message();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <st_Client> V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);
    st_Client Client;
    string Account_Number = Read_Client_Account_Number();
    if (Find_Client_By_Account_Number(Account_Number, V_Clients, Client))
        Print_Client_Card(Client);
    else
        cout << "\nClient with Account Number [" << Account_Number << "] is not found!";

}

void Show_Find_User_Screen() {
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <st_User> V_Users = Load_Users_Data_From_File(Users_File_Name);
    string User_Name = Read_User_Name();
    st_User User;

    if (Find_User_By_User_Name(User_Name, V_Users, User))
        Print_User_Card(User);
    else
        cout << "\nUser with Username [" << User_Name << "] is not found!";

}

void Show_End_Screen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void Show_Deposit_Screen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    st_Client Client;

    vector <st_Client> V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);
    string Account_Number = Read_Client_Account_Number();


    while (!Find_Client_By_Account_Number(Account_Number, V_Clients, Client))
    {
        cout << "\nClient with [" << Account_Number << "] does not exist.\n";
        Account_Number = Read_Client_Account_Number();
    }

    Print_Client_Card(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount ? ";
    cin >> Amount;

    Deposit_Balance_To_Client_By_Account_Number(Account_Number, Amount, V_Clients);
}

void Show_With_Draw_Screen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    st_Client Client;

    vector <st_Client> V_Clients = Load_Cleints_Data_From_File(Clients_File_Name);
    string Account_Number = Read_Client_Account_Number();


    while (!Find_Client_By_Account_Number(Account_Number, V_Clients, Client))
    {
        cout << "\nClient with [" << Account_Number << "] does not exist.\n";
        Account_Number = Read_Client_Account_Number();
    }

    Print_Client_Card(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount ? ";
    cin >> Amount;

    while (Amount > Client.Account_Balance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.Account_Balance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    Deposit_Balance_To_Client_By_Account_Number(Account_Number, Amount * -1, V_Clients);
}

void Show_Total_Balances_Screen()
{
    Show_Total_Balances();
}

bool Check_Access_Permission(en_Main_Menu_Permissions Permission) {
    if (Current_User.Permissions == en_Main_Menu_Permissions::e_All)
        return true;

    if ((Permission & Current_User.Permissions) == Permission)
        return true;
    else
        return false;
}

void Go_Back_To_Main_Menu()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    Show_Main_Menu();
}

void Go_Back_To_Transactions_Menu()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    Show_Transactions_Menu();
}

void Go_Back_To_Manage_Users_Menu() {
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("Pause>0");
    Show_Manage_Users_Menu();
}

short Read_Transactions_Menu_Option()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void Perfrom_Tranactions_Menu_Option(en_Transactions_Menu_Options Transaction_Menu_Option)
{
    switch (Transaction_Menu_Option)
    {
    case en_Transactions_Menu_Options::e_Deposit:
    {
        system("cls");
        Show_Deposit_Screen();
        Go_Back_To_Transactions_Menu();
        break;
    }
    case en_Transactions_Menu_Options::e_Withdraw:
    {
        system("cls");
        Show_With_Draw_Screen();
        Go_Back_To_Transactions_Menu();
        break;
    }
    case en_Transactions_Menu_Options::e_Show_Total_Balance:
    {
        system("cls");
        Show_Total_Balances_Screen();
        Go_Back_To_Transactions_Menu();
        break;
    }
    case en_Transactions_Menu_Options::e_Show_Main_Menu:
    {
        Show_Main_Menu();
    }
    }
}

void Show_Transactions_Menu()
{
    if (!Check_Access_Permission(en_Main_Menu_Permissions::P_Transactions)) {
        Show_Access_Denied_Message();
        Go_Back_To_Main_Menu();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    Perfrom_Tranactions_Menu_Option((en_Transactions_Menu_Options)Read_Transactions_Menu_Option());
}

short Read_Main_Menu_Option()
{
    cout << "Choose what do you want to do? [1 to 7] ? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void Perform_Manage_Users_Menu_Option(en_Manage_Users_Menu_Options Manage_Users_Menu_Option) {
    switch (Manage_Users_Menu_Option) {
    case en_Manage_Users_Menu_Options::e_List_Users:
    {
        system("cls");
        Show_List_Users_Screen();
        Go_Back_To_Manage_Users_Menu();
        break;
    }
    case en_Manage_Users_Menu_Options::e_Add_New_User:
    {
        system("cls");
        Show_Add_New_User_Screen();
        Go_Back_To_Manage_Users_Menu();
        break;
    }
    case en_Manage_Users_Menu_Options::e_Delete_User:
    {
        system("cls");
        Show_Delete_User_Screen();
        Go_Back_To_Manage_Users_Menu();
        break;
    }
    case en_Manage_Users_Menu_Options::e_Update_User:
    {
        system("cls");
        Show_Update_User_screen();
        Go_Back_To_Manage_Users_Menu();
        break;
    }
    case en_Manage_Users_Menu_Options::e_Find_User:
    {
        system("cls");
        Show_Find_User_Screen();
        Go_Back_To_Manage_Users_Menu();
        break;
    }
    case en_Manage_Users_Menu_Options::e_Main_Menu:
    {
        Show_Main_Menu();
    }
    }
}

short Read_Manage_Users_Menu_Option() {
    cout << "Choose What Do You Want To Do ? [1 to 6] ? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void Show_Manage_Users_Menu() {

    if (!Check_Access_Permission(en_Main_Menu_Permissions::P_Manage_Users)) {
        Show_Access_Denied_Message();
        Go_Back_To_Main_Menu();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    Perform_Manage_Users_Menu_Option((en_Manage_Users_Menu_Options)Read_Manage_Users_Menu_Option());
}

void Perfrom_Main_Menu_Option(en_Main_Menu_Options Main_Menu_Option)
{
    switch (Main_Menu_Option)
    {
    case en_Main_Menu_Options::e_List_Clients:
    {
        system("cls");
        Show_All_Clients_Screen();
        Go_Back_To_Main_Menu();
        break;
    }
    case en_Main_Menu_Options::e_Add_New_Client:
        system("cls");
        Show_Add_New_Clients_Screen();
        Go_Back_To_Main_Menu();
        break;

    case en_Main_Menu_Options::e_Delete_Client:
        system("cls");
        Show_Delete_Client_Screen();
        Go_Back_To_Main_Menu();
        break;

    case en_Main_Menu_Options::e_Update_Client:
        system("cls");
        Show_Update_Client_Screen();
        Go_Back_To_Main_Menu();
        break;

    case en_Main_Menu_Options::e_Find_Client:
        system("cls");
        Show_Find_Client_Screen();
        Go_Back_To_Main_Menu();
        break;

    case en_Main_Menu_Options::e_Show_Transactions_Menu:
        system("cls");
        Show_Transactions_Menu();
        break;
    case en_Main_Menu_Options::e_Manage_Users:
        system("cls");
        Show_Manage_Users_Menu();
        break;
    case en_Main_Menu_Options::e_Exit:
        system("cls");
        Login();
        break;
    }

}

void Show_Main_Menu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    Perfrom_Main_Menu_Option((en_Main_Menu_Options)Read_Main_Menu_Option());
}

bool Load_User_Info(string User_Name, string Password) {
    if (Find_User_By_User_Name_And_Password(User_Name, Password, Current_User))
        return true;
    else
        return false;
}

void Login() {
    bool Login_Failed = false;

    string User_Name, Password;

    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (Login_Failed) {
            cout << "Invalid User Name/Password!\n";
        }

        cout << "Enter User Name ? ";
        cin >> User_Name;

        cout << "Enter Password ? ";
        cin >> Password;

        Login_Failed = !Load_User_Info(User_Name, Password);

    } while (Login_Failed);

    Show_Main_Menu();
}

int main()
{
    Login();
    system("pause>0");
    return 0;
}