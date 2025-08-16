#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<windows.h>
#include<stack>
#include<bits/stdc++.h>
#include<conio.h>
using namespace std;
struct Phonebook {
    string name;
    string pno;
    string address;
    string state;
};
struct Node {
    Phonebook entry;
    Node *left,*right;
    Node(Phonebook entry):entry(entry),left(nullptr),right(nullptr) {}
};
bool check_no();
void upper();
Node* insertbst(Node* root, Phonebook entry);
Node* databst(const string& filename);
bool duplicate(Node* root, string pno);
void inserttxt(Phonebook entry);
vector<Phonebook> readtxt();
void merge(vector<Phonebook>& entries,int left,int mid,int right);
void mergeSort(vector<Phonebook>& entries,int left,int right);
void display();
vector<Node*> search_name(Node* root, const string& name);
void Bulkinsert(Node*& root);
vector<Node*> search_state(Node* root, const string& state);
void adminoptions();
void admin();
void us();
int login();
bool check_no(const string& pno) {
    if (pno.length()!=10) {
        return false;
    }
    for (char c:pno) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
void upper(string &str) {
    for (char &c : str) {
        c = toupper(c);
    }
}
Node* insertbst(Node* root, Phonebook entry) {
    if (root==nullptr)
        return new Node(entry);
    if (entry.name<root->entry.name)
        root->left=insertbst(root->left, entry);
    else
        root->right=insertbst(root->right,entry);
    return root;
}
Node* databst(const string& filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cout<<"Error opening file!"<<endl;
        return nullptr;
    }
    Node* root=nullptr;
    string name,pno,address,state;
    while (file>>ws && getline(file, name,',') && getline(file, pno,',') && getline(file, state,',') && getline(file,address)){
        Phonebook entry = {name, pno, state, address};
        root = insertbst(root, entry);
    }
    file.close();
    return root;
}
bool duplicate(Node* root, string pno){
    bool d = false;
        stack<Node*> nodeStack;
        Node* current=root;
        while (current!=nullptr || !nodeStack.empty()) {
            while (current!=nullptr) {
                nodeStack.push(current);
                current=current->left;
            }
            current=nodeStack.top();
            nodeStack.pop();
            if (current->entry.pno==pno) {
                d=true;
                break;
            }
            current=current->right;
        }
        return d;
}
void inserttxt(Phonebook entry) {
    ofstream file("phonebook.txt",ios::app);
    if (file.is_open()){
        file<<entry.name<<","<<entry.pno<<","<<entry.state<<","<<entry.address<<"\n";
        file.close();
    } else {
        cout<<"Error opening file for writing!"<<endl;
    }
}
vector<Phonebook> readtxt() {
    vector<Phonebook> entries;
    ifstream file("phonebook.txt");
    if (file.is_open()){
        string name,pno,address,state;
        while (file>>ws && getline(file,name,',') && getline(file,pno,',') && getline(file,state,',') && getline(file,address)){
            entries.push_back({name,pno,state,address});
        }
        file.close();
    } else {
        cout<<"Error opening file for reading!"<<endl;
    }
    return entries;
}
void merge(vector<Phonebook>& entries,int left,int mid,int right) {
    int n1=mid-left+1;
    int n2=right-mid;
    vector<Phonebook> L(n1);
    vector<Phonebook> R(n2);
    for (int i=0;i<n1;++i)
        L[i]=entries[left+i];
    for (int i=0;i<n2;++i)
        R[i]=entries[mid+1+i];
    int i=0,j=0,k=left;
    while(i<n1 && j<n2) {
        if (L[i].name<=R[j].name)
            entries[k++]=L[i++];
        else
            entries[k++]=R[j++];
    }
    while (i<n1)
        entries[k++]=L[i++];
    while (j<n2)
        entries[k++]=R[j++];
}
void mergeSort(vector<Phonebook>& entries,int left,int right) {
    if (left<right){
        int mid=left+(right - left)/2;
        mergeSort(entries,left,mid);
        mergeSort(entries,mid+1,right);
        merge(entries,left,mid,right);
    }
}
void display(){
    vector<Phonebook> entries=readtxt();
    if (entries.empty()){
        cout<<"No entries found in the phonebook!"<<endl;
        return;
    }
    mergeSort(entries,0,entries.size()-1);
    system("cls");
    cout << "\nPhonebook Directory:\n";
    cout << "===========================================================================================\n";
    cout << "| Name              | Phone Number  | State          | Address                            |\n";
    cout << "===========================================================================================\n";
    for (auto entry : entries) {
        cout<<"| "<<left<<setw(18)<<entry.name
            <<"| "<<setw(14)<<entry.pno
            <<"| "<<setw(15)<<entry.state
            <<"| "<<setw(35)<<entry.address<<"|\n";
    }
    cout << "===========================================================================================\n";
}
vector<Node*> search_name(Node* root, const string& name) {
    vector<Node*> results;
    stack<Node*> nodeStack;
    Node* current=root;
    string nameUpper=name;
    upper(nameUpper);
    while (current!=nullptr || !nodeStack.empty()) {
        while (current!=nullptr) {
            nodeStack.push(current);
            current=current->left;
        }
        current=nodeStack.top();
        nodeStack.pop();
        string currentName = current->entry.name;
        upper(currentName);
        if (currentName==nameUpper) {
            results.push_back(current);
        }
        current=current->right;
    }
    return results;
}
void inordertrav(Node* root){
    if (root==nullptr) return;
    inordertrav(root->left);
    cout <<"Name: "<<root->entry.name<<", Phone Number: "<<root->entry.pno
         <<",State: "<<root->entry.state<<", Address: "<<root->entry.address<<endl;
    inordertrav(root->right);
}
void Bulkinsert(Node*& root) {
    stack<Phonebook> entryStack;
    string name, pno, address, state;
    string choice;
    while (true){
        cin.ignore();
        start:
        system("cls");
        cout << "Enter Name: ";
        getline(cin, name);
        upper(name);
        cout << "Enter Phone Number: ";
        getline(cin, pno);
        if (!check_no(pno)) {
            cout << "Invalid phone number. Please enter a 10-digit number.\n";
            system("pause");
            continue;
        }
        if (duplicate(root,pno)) {
            cout << "Duplicate phone number found: " << pno << ". Entry rejected.\n";
            system("pause");
            goto start;
        }
        cout << "Enter your state: ";
        getline(cin, state);
        upper(state);
        cout << "Enter Address: ";
        getline(cin, address);
        Phonebook entry={name, pno, state, address};
        entryStack.push(entry);
        cout << "Entry added: " << entry.name << ", " << entry.pno << ", State: " << entry.state << ", Address: " << entry.address << endl;
    x:
        cout<<"Do you want to delete the previous entry? (y/n): ";
        cin>>choice;
        if (choice=="y" || choice=="Y") {
            entryStack.pop();
            cout<<"Previous entry deleted.\n";
        } else if (choice!="n"&&choice!="N") {
            cout << "Invalid input.\n";
            system("pause");
            goto x;
        }
    y:
        cout << "Do you want to stop adding entries? (y/n): ";
        cin >> choice;
        if (choice=="y" || choice=="Y") {
            break;
        } else if (choice!="n" && choice!="N") {
            cout<<"Invalid input.\n";
            system("pause");
            goto y;
        }
    }
    while (!entryStack.empty()) {
        Phonebook entry=entryStack.top();
        entryStack.pop();
        root=insertbst(root, entry);
        inserttxt(entry);
    }
    cout<<"Entries added to the phonebook.\n";
}
vector<Node*> search_state(Node* root, const string& state) {
    vector<Node*> results;
    stack<Node*> nodeStack;
    Node* current = root;
    string stateUpper = state;
    upper(stateUpper);
    while (current!=nullptr||!nodeStack.empty()) {
        while (current!=nullptr) {
            nodeStack.push(current);
            current=current->left;
        }
        current = nodeStack.top();
        nodeStack.pop();
        string currentState = current->entry.state;
        upper(currentState);
        if (currentState == stateUpper) {
            results.push_back(current);
        }
        current = current->right;
    }
    return results;
}
Node* delnode(Node* root, const string& pno, const string& name) {
    if (root==nullptr) {
        return root;
    }
    if (name<root->entry.name) {
        root->left=delnode(root->left, pno, name);
    } else if (name>root->entry.name) {
        root->right=delnode(root->right, pno, name);
    } else {
        if (root->left==nullptr) {
            Node* temp=root->right;
            delete root;
            return temp;
        } else if (root->right==nullptr) {
            Node* temp=root->left;
            delete root;
            return temp;
        }
        Node* temp=root->right;
        while (temp && temp->left!=nullptr) {
            temp=temp->left;
        }
        root->entry=temp->entry;
        root->right=delnode(root->right, temp->entry.pno, name);
    }
    return root;
}
void delpno(Node*& root) {
    if (root==nullptr) {
        cout<<"The phonebook is empty. No contacts to delete.\n";
        return;
    }
    string pno,name;
    cout<<"Enter the name of the contact to delete: ";
    cin>>name;
    upper(name);
    cout<<"Enter the phone number of the contact to delete: ";
    cin>>pno;
    if (!check_no(pno)) {
        cout<<"Invalid phone number format.\n";
        return;
    }
    bool found=duplicate(root,pno);
    root=delnode(root, pno, name);
    if (!found) {
        cout<<"Contact with phone number "<<pno<<" not found.\n";
        return;
    }
    vector<Phonebook> entries=readtxt();
    auto it = remove_if(entries.begin(), entries.end(), [&](const Phonebook& entry) {
        return entry.pno==pno;
    });
    if (it!=entries.end()) {
        entries.erase(it, entries.end());
        ofstream file("phonebook.txt");
        if (file.is_open()) {
            for (const auto& entry : entries) {
                file<<entry.name<<","<<entry.pno<<","<<entry.address<<","<<entry.state<<"\n";
            }
            file.close();
            cout<<"Contact deleted successfully!\n";
        } else {
            cout<<"Error updating the file.\n";
        }
    } else {
        cout<<"Contact with phone number " << pno << " was not found in the file.\n";
    }
}
void updatecontact(Node* root) {
    if (root==nullptr) {
        cout<<"The phonebook is empty. Cannot update any contacts.\n";
        return;
    }
    string searchKey;
    cout<<"Enter the name or phone number of the contact to update: ";
    cin.ignore();
    getline(cin, searchKey);
    upper(searchKey);
    Node* contactNode=nullptr;
    stack<Node*> nodeStack;
    Node* current=root;
    while (current!=nullptr || !nodeStack.empty()) {
        while (current!=nullptr) {
            nodeStack.push(current);
            current=current->left;
        }
        current=nodeStack.top();
        nodeStack.pop();
        string currentName=current->entry.name;
        string currentPno=current->entry.pno;
        upper(currentName);
        upper(currentPno);
        if (currentName==searchKey || currentPno==searchKey) {
            contactNode=current;
            break;
        }
        current = current->right;
    }
    if (contactNode==nullptr) {
        cout<<"Contact not found.\n";
        return;
    }
    cout<<"Contact found:\n";
    cout<<"Name: "<<contactNode->entry.name
        <<", Phone Number: "<< contactNode->entry.pno
        <<", State: "<<contactNode->entry.state
        <<", Address: "<<contactNode->entry.address<<"\n";
    cout<<"Enter new details (press Enter to keep the current value):\n";
    cout<<"New Name ("<<contactNode->entry.name<<"): ";
    string newName;
    getline(cin, newName);
    if (!newName.empty()) {
        upper(newName);
        contactNode->entry.name=newName;
    }
    cout<<"New Phone Number ("<<contactNode->entry.pno<<"): ";
    string newpno;
    getline(cin, newpno);
    if (!newpno.empty()) {
        if (!check_no(newpno)) {
            cout << "Invalid phone number. Update canceled.\n";
            return;
        }
        contactNode->entry.pno=newpno;
    }
    cout << "New State ("<< contactNode->entry.state<<"): ";
    string newState;
    getline(cin, newState);
    if (!newState.empty()) {
        upper(newState);
        contactNode->entry.state = newState;
    }

    cout << "New Address (" << contactNode->entry.address << "): ";
    string newAddress;
    getline(cin, newAddress);
    if (!newAddress.empty()) {
        contactNode->entry.address = newAddress;
    }

    vector<Phonebook> entries = readtxt();
    bool updated = false;
    for (auto& entry : entries) {
        string entryNameUpper = entry.name;
        string entryPnoUpper = entry.pno;
        upper(entryNameUpper);
        upper(entryPnoUpper);
        if (entryPnoUpper == searchKey || entryNameUpper == searchKey) {
            entry.name = contactNode->entry.name;
            entry.pno = contactNode->entry.pno;
            entry.state = contactNode->entry.state;
            entry.address = contactNode->entry.address;
            updated = true;
            break;
        }
    }
    if (!updated) {
        cout << "Error: Contact not found in the text file.\n";
        return;
    }
    ofstream file("phonebook.txt");
    if (file.is_open()) {
        for (const auto& entry : entries) {
            file << entry.name << "," << entry.pno << "," << entry.address << "," << entry.state << "\n";
        }
        file.close();
        cout << "Contact updated successfully!\n";
    } else {
        cout << "Error updating contact in file.\n";
    }
}
void adminoptions() {
    Node* root = databst("phonebook.txt");
    int choice;
    string name, pno, address, state;
    c:
    while (true) {
        system("cls");
        cout << "\n1. Insert Entry\n2. Display Phonebook\n3. Insert Bulk Entries\n4. Search Contact by Name\n5. Search Contact by State\n6. Update Contact\n7. Delete contact\n8. Main Menu\nEnter your choice: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "ERROR!!!! Invalid input. Please enter a number.\n";
            system("pause");
            continue;
        }
        switch (choice) {
            case 1: {
                start:
                system("cls");
                fflush(stdin);
                cout << "Enter Name: ";
                getline(cin, name);
                upper(name);
                cout << "Enter Phone Number: ";
                getline(cin, pno);
                if (!check_no(pno)) {
                    cout << "Enter a valid Phone number!!";
                    system("pause");
                    goto start;
                }
                if (duplicate(root,pno)) {
                    cout << "Duplicate by Phone Number found: " << pno << endl;
                    cout << "Contact addition canceled due to duplicate entry.\n";
                    system("pause");
                    goto c;
                }
                cout << "Enter your state: ";
                // cin.ignore();
                getline(cin, state);
                upper(state);
                cout << "Enter Address: ";
                getline(cin, address);
                Phonebook entry = {name, pno, state, address};
                root = insertbst(root, entry);
                inserttxt(entry);
                cout << "Entry Added successfully!";
                system("pause");
                break;
            }
            case 2:
                display();
                system("pause");
                break;
            case 3:
                Bulkinsert(root);
                system("pause");
                break;
            case 4: {
                cin.ignore();
                cout << "Enter the name to search: ";
                getline(cin, name);
                upper(name);
                vector<Node*> results=search_name(root, name);
                if (!results.empty()) {
                    cout<<"\nContacts Found:\n";
                    cout<<"===========================================================================================\n";
                    cout<<"| Name              | Phone Number  | State          | Address                            |\n";
                    cout<<"===========================================================================================\n";
                    for(const auto& result:results) {
                        cout<<"| "<<left<<setw(18)<<result->entry.name
                            <<"| "<<setw(14)<<result->entry.pno
                            <<"| "<<setw(15)<<result->entry.state
                            <<"| "<<setw(35)<<result->entry.address<<"|\n";
                    }
                    cout<<"===========================================================================================\n";
                } else {
                    cout<<"Contact not found!"<<endl;
                }
                system("pause");
                break;
            }
            case 5: {
                cin.ignore();
                cout << "Enter the state to search: ";
                getline(cin, state);
                upper(state);
                vector<Node*> results=search_state(root, state);
                if (!results.empty()) {
                    cout<<"\nContacts Found:\n";
                    cout<<"===========================================================================================\n";
                    cout<<"| Name              | Phone Number  | State          | Address                            |\n";
                    cout<<"===========================================================================================\n";
                    for(const auto& result:results) {
                        cout<<"| "<<left<<setw(18)<<result->entry.name
                            <<"| "<<setw(14)<<result->entry.pno
                            <<"| "<<setw(15)<<result->entry.state
                            <<"| "<<setw(35)<<result->entry.address<<"|\n";
                    }
                    cout<<"===========================================================================================\n";
                } else {
                    cout<<"Contact not found!"<<endl;
                }
                system("pause");
                break;
            }
            case 6:
                updatecontact(root);
                system("pause");
                break;
            case 7:
                delpno(root);
                system("pause");
                break;
            case 8:
                login();
                break;
            default:
                cout<<"Wrong Input!\n";
                system("pause");
                goto c;
        }
    }
}
void admin(){
    stepback:
    system("cls");
    string user;
    cout<<"\t\t\t\t\t\t"<<"Username :";
    cin>>user;
    fflush(stdin);
    cout<<"\t\t\t\t\t\t"<<"Password :";
    char password[20];
    int i=0;
    char ch;
    while ((ch=getch())!='\r') {
        if (ch=='\b') {
            if(i>0) {
                cout<<"\b \b";
                i--;
            }
        }
        else {
                password[i++]=ch;
                cout<<'*';
        }
        }
        password[i]='\0';
        int x=0;
        char pass[6]={'a','b','c','d','e','f'};
        int n=sizeof(pass);
        for(int i=0;i<n;i++){
            if(password[i]==pass[i]){
                x++;
            }
            else{
                system("cls");
                cout<<endl<<endl<<endl;
                cout<<endl<<"\t\t\t\t\t\t"<<"WRONG PASSWORD TRY AGAIN"<<endl;
                cout<<endl<<endl<<endl;
                goto stepback;
            }
        }
        if(x==n){
            cout<<endl<<endl<<endl;
            cout<<endl<<"\t\t\t\t\t\t"<<"LOGIN SUCCESSFULL"<<endl;
            cout<<endl<<endl<<endl;
            system("pause");
            adminoptions();
        }
        cout<<endl<<endl<<endl<<endl<<endl<<endl;
}
void us(){
    Node* root = databst("phonebook.txt");
    int choice;
    string name, pno, address, state;
    c:
    while (true) {
        system("cls");
        cout<<"\n1. Display Phonebook\n2. Search Contact by Name\n3. Search Contact by State\n4. Main Menu\nEnter your choice: ";
        cin>>choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "ERROR!!!! Invalid input. Please enter a number.\n";
            system("pause");
            continue;
        }
        switch(choice){
        case 1:{
            display();
            system("pause");
            break;
        }
        case 2:{
            cin.ignore();
            cout<<"Enter the name to search: ";
            getline(cin,name);
            upper(name);
            vector<Node*> results=search_name(root, name);
            if (!results.empty()) {
                cout<<"\nContacts Found:\n";
                cout<<"===========================================================================================\n";
                cout<<"| Name              | Phone Number  | State          | Address                            |\n";
                cout<<"===========================================================================================\n";
                for(const auto& result:results) {
                    cout<<"| "<<left<<setw(18)<<result->entry.name
                        <<"| "<<setw(14)<<result->entry.pno
                        <<"| "<<setw(15)<<result->entry.state
                        <<"| "<<setw(35)<<result->entry.address<<"|\n";
                    }
                cout<<"===========================================================================================\n";
            } else {
                cout<<"Contact not found!"<<endl;
            }
            system("pause");
            break;
        }
        case 3:{
            cin.ignore();
            cout << "Enter the state to search: ";
            getline(cin, state);
            upper(state);
            vector<Node*> results=search_state(root, state);
            if (!results.empty()) {
                cout<<"\nContacts Found in State: " << state << "\n";
                cout<<"===========================================================================================\n";
                cout<<"| Name              | Phone Number  | State          | Address                            |\n";
                cout<<"===========================================================================================\n";
                for(const auto& result:results) {
                    cout<<"| "<<left<<setw(18)<< result->entry.name
                        <<"| "<<setw(14)<<result->entry.pno
                        <<"| "<<setw(15)<<result->entry.state
                        <<"| "<<setw(35)<<result->entry.address << "|\n";
                }
                cout<<"===========================================================================================\n";
            } else {
                cout<<"No contacts found in the state: "<<state<<endl;
            }
            system("pause");
            break;
        }
        case 4:{
            login();
            break;
        }
        default:{
            cout<<"Wrong Input!\n";
            system("pause");
            goto c;
        }
        }
    }
}
int login(){
    system("title Phonebook Management System");
    stepback:
    system("@echo off");
    system("cls");
    system("color 70");
    cout<<"\t\t\t\t\t\t"<<" ---------------------------"<<endl;
    cout<<"\t\t\t\t\t\t"<<"|                           |"<<endl;
    cout<<"\t\t\t\t\t\t"<<"|    PHONEBOOK MANAGEMENT   |"<<endl;
    cout<<"\t\t\t\t\t\t"<<"|          SYSTEM           |"<<endl;
    cout<<"\t\t\t\t\t\t"<<"|                           |"<<endl;
    cout<<"\t\t\t\t\t\t"<<" ---------------------------"<<endl;
    cout<<"\t\t\t\t"<<"    WELCOME TO THE INTERFACE OF PHONEBOOK MANAGEMENT SYSTEM"<<endl<<endl<<endl<<endl<<endl;
    string ask;
    cout<<"\t\t\t\t\t\t1.ADMIN: \n\t\t\t\t\t\t2.USER: \n\t\t\t\t\t\t3. Exit: \n\t\t\t\t\t\tEnter Choice: ";
    cin>>ask;
    if(ask=="1"){admin();}
    else if(ask=="2"){ us();}
    else if(ask=="3"){
        cout<<"Exiting...\n";
        // system("timeout 3");
        return 0;
    }
    else{
        cout<<"\t\t\t\t\t\tWrong Input!!\n\t\t\t\t\t\t";
        system("pause");
        goto stepback;
    }
}
int main() {
    login();
    return 0;
}





























