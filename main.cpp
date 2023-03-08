#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

void Returnmenu();
int inputValidation(int);
/*
//Global Variables
*/
sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::ResultSet* res;
//stringstream strstr;
string query;
int switchVar;
typedef struct
{
	int day;
	int month;
	int year;
} date;

/*
//Class declarations
*/
class books
{
	int id;			// Primary Key
	string name;
	string auth;
	string price;
	string qty;
public:
	void add();
	void multiAdd();
	void update_price();
	void search();
	void update();
	void display();
};
class employees
{
	int id;				//Primary Key
	string name;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
	string phn;
	date date_of_joining;
	string salary;
	string mgr_status;	//check(T or F) def f
public:
	void add_emp();
	void search_emp();
	void assign_mgr_stat();
	void display();
	void update_sal();
};
class members
{
	int id;				//Primary Key
	string name;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
	string phn;
	date beg_date;
	date end_date;
	string valid;
public:
	void add_mem();
	void refresh();
	void search_mem();
};
class purchases
{
	int ord_id;			//Primary Key
	int book_id;		//FK ref (books)
	int sup_id;			//FK ref (suppliers)
	int qty;
	date dt_ordered;
	int eta;
	char received;		// Check(T or C or F) def (F)
	int inv;
public:
	void new_ord();
	void view();
	void mar_cancel();
	void mark_reciv();
};
class sales
{
	int invoice_id;		//Primary key
	int member_id;		//FK ref member(id)
	int book_id;		//FK ref books(id)
	int qty;
	int amount;
	date date_s;
public:
	void add();
	void find_total_sales();
};
class suppliers
{
	int id;				//Primary Key
	string name;
	string phn;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
public:
	void add_sup();
	void remove_supplier();
	void search_id();
};

/*
//Class definitions
*/

/*
//Books
*/
void books::add()
{
	stringstream strstr;
	int idnum = 0;
	res = stmt->executeQuery("SELECT id FROM books ORDER BY id DESC LIMIT 1;");
	while (res->next())
	{
		idnum = (res->getInt(1)) + 1;
	}
	cout << "Enter the Name of the book: ";
	cin >> name;
	cout << "Enter the Author: ";
	cin >> auth;
	cout << "Enter the Price of the book: ";
	cin >> price;
	cout << "Enter the Qty Received: ";
	cin >> qty;
	strstr.str("");
	strstr << "INSERT INTO books VALUES('" << idnum << "','" << name << "','" << auth << "'," << price << "," << qty << ")";
	stmt->execute(strstr.str());
	cout << "\nBook entered successfully!\n";
}
void books::multiAdd()
{
	const int superCAPACITY = 1000;
	string names[superCAPACITY];
	ifstream inf{ "Names.txt" };
	if (!inf)
	{
		cerr << "Uh oh, Names.txt couldn't be opened!\n";
		exit(1);
	}
	for (int i = 0; i < superCAPACITY; i++)
		inf >> names[i];

	stringstream strstr;
	int val;
	cout << "Enter number of entries here: ";
	cin >> val;
	for (int i = 0; i < val; i++)
	{
		int idnum = 0;
		res = stmt->executeQuery("SELECT id FROM books ORDER BY id DESC LIMIT 1;");
		while (res->next())
		{
			idnum = (res->getInt(1)) + 1;
		}
		//Name
		name = 'A' + rand() % 26;
		//Auth
		auth = names[rand() % superCAPACITY];
		//Price
		price = to_string(rand() % 41) + '.' + to_string(rand() % 10) + to_string(rand() % 10);
		//Quantity
		qty = to_string(rand() % 11);
		strstr.str("");
		strstr << "INSERT INTO books VALUES('" << idnum << "','" << name << "','" << auth << "'," << price << "," << qty << ")";
		stmt->execute(strstr.str());
	}
	cout << "\nBooks entered successfully!\n";
}
void books::update_price()
{
	char choice;
	stringstream strstr;
	int indent = 25;
	cout << "Enter the id of the book for update in price: ";
	cin >> id;
	strstr.str("");
	strstr << "SELECT name, price FROM books WHERE id = " << id << ";";
	query = strstr.str();
	res = stmt->executeQuery(query);

	if (res->rowsCount() > 0)
	{
		while (res->next())
		{
			cout << left << setw(indent) << "The Name of the book is : " << res->getString("name").c_str() << endl;
			cout << left << setw(indent) << "The current price of the book is : " << res->getString("price").c_str() << endl;
		}
		cout << "Do you Want to Update the Price [y/n] : ";
		cin >> choice;
		if (choice == 121 || choice == 89)
		{
			cout << "Enter the new price : ";
			cin >> price;
			strstr.str("");
			strstr << "UPDATE books SET price = " << price << " WHERE id = " << id << ";";
			query = strstr.str();
			int update = stmt->executeUpdate(query);
			if (update != 0)
				cout << endl << endl << "Book Price Updated Successfully" << endl << endl << endl;
			else
				cout << endl << endl << "Entry ERROR !" << endl << "Contact Technical Team " << endl << endl << endl;
		}
		else
		{
			cout << "No changes Made!!";
		}
	}
	else
	{
		cout << "No Book found!!!";
	}
}
void books::search()
{
	int indent = 26;
	stringstream strstr;
	cout << "Enter book id number for details: ";
	cin >> id;
	strstr.str("");
	strstr << "SELECT * FROM books WHERE id = " << id << ";";
	query = strstr.str();
	res = stmt->executeQuery(query);
	if (res->rowsCount() > 0)
	{
		while (res->next())
		{
			cout << left << setw(indent) << "Details of book id number" << res->getInt(1) << endl;
			cout << left << setw(indent) << "Name of the book: " << res->getString("name").c_str() << endl;
			cout << left << setw(indent) << "Author of the book: " << res->getString("auth").c_str() << endl;
			cout << left << setw(indent) << "Price of the book: " << res->getString("price").c_str() << endl;
			cout << left << setw(indent) << "Inventory count: " << res->getString("qty").c_str() << endl;
		}
	}
	else
	{
		cout << "No record Found" << endl;
	}
}
void books::update()
{
	stringstream strstr;
	const int SIZE = 20;
	int b_id[SIZE] = {}, qty[SIZE] = {}, i = 0, max;
	strstr.str("");
	strstr << "SELECT book_id,qty FROM purchases WHERE received = 'T' and inv IS NULL;";
	query = strstr.str();
	res = stmt->executeQuery(query);
	strstr.str("");
	strstr << "UPDATE purchases SET inv = 1 WHERE received = 'T' and inv IS NULL;";
	query = strstr.str();
	stmt->executeUpdate(query);
	while (res->next())
	{
		b_id[i] = res->getInt(1);
		qty[i] = res->getInt(2);
		i++;
	}
	max = i;
	for (int i = 0; i <= max; i++)
	{
		strstr.str("");
		strstr << "UPDATE books SET qty = " << qty[i] << " WHERE id = " << b_id[i] << ";";
		query = strstr.str();
		stmt->executeUpdate(query);
	}
	cout << "The orders recieved have been updated.\n\n";
}
void books::display()
{
	int i = 0, indent = 20;
	query = "SELECT * FROM books;";
	res = stmt->executeQuery(query);
	while (res->next())
	{
		cout << left << setw(indent) << "Book id number " << ++i << endl;
		cout << left << setw(indent) << "Name of book: " << res->getString(2) << endl;
		cout << left << setw(indent) << "Name of author: " << res->getString(3) << endl;
		cout << left << setw(indent) << "Price: " << res->getString(4) << endl;
		cout << left << setw(indent) << "Quantity: " << res->getString(5) << endl << endl;
	}
}
/*
//Employees
*/
void employees::add_emp()
{
	int idnum = 0;
	res = stmt->executeQuery("SELECT id FROM employees ORDER BY id DESC LIMIT 1;");
	while (res->next())
	{
		idnum = (res->getInt(1)) + 1;
	}

	stringstream strstr;
	cout << "Enter Your Id for verification: ";
	cin >> id;
	strstr.str("");
	strstr << "SELECT mgr_status FROM employees WHERE id  = " << id << ";";
	res = stmt->executeQuery(strstr.str());
	if (!res->next())
	{
		cout << "Employee Not Found!!" << endl << endl << endl;
		//return;
	}
	else
	{
		mgr_status = res->getString("mgr_status");
		if (mgr_status == "F")
		{
			cout << "You Do Not have Manager Rights!!!" << endl << endl;
			return;
		}
	}
	cout << "Enter The details of the new employee\n";
	cout << "Enter The name of the employee: ";
	cin >> name;
	cout << "Enter the Address (in 3 lines): " << endl;
	cin >> addr_line1;
	cin >> addr_line2;
	cin >> addr_city;
	cout << "Enter State: ";
	cin >> addr_state;
	cout << "Enter phone no.: ";
	cin >> phn;
	cout << "Enter the salary: ";
	cin >> salary;
	strstr.str("");
	strstr << "INSERT INTO employees (id,name,addr_line1,addr_line2,addr_city,addr_state,phn,date_of_joining,salary) VALUES ('" << idnum << "','" << name << "','" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "','" << phn << "',CURDATE(),'" << salary << "');";
	stmt->execute(strstr.str());
	cout << endl << endl << "Employee Added Successfully!" << endl << endl << endl;
}
void employees::assign_mgr_stat()
{
	stringstream strstr;
	cout << "Enter Your Id for verification: ";
	cin >> id;
	strstr.str("");
	strstr << "SELECT mgr_status FROM employees WHERE id  = " << id << ";";
	res = stmt->executeQuery(strstr.str());
	if (!res->next())
	{
		cout << "Employee Not Found!!" << endl << endl << endl;
		return;
	}
	else
	{
		mgr_status = res->getString("mgr_status");
		if (mgr_status == "F")
		{
			cout << "You Do Not have Manager Rights!!!" << endl << endl;
			//return;
		}
	}
	cout << "Enter the employee id to grant Manager status: ";
	cin >> id;
	strstr.str("");
	strstr << "UPDATE employees SET mgr_status = 'T' WHERE id = " << id << ";";
	stmt->executeUpdate(strstr.str());
	cout << endl << endl << endl;
	cout << "Manager Status granted";
	cout << endl << endl << endl;
}
void employees::search_emp()
{
	stringstream strstr;
	int indent = 25;
	cout << "Enter the id for searching an employee: ";
	cin >> id;
	strstr.str("");
	strstr << "SELECT * FROM employees WHERE id  = " << id << ";";
	res = stmt->executeQuery(strstr.str());
	if (res->next())
	{
		cout << left << setw(indent) << "Employees Details" << endl;
		cout << left << setw(indent) << "Name: " << res->getString("name") << endl;
		cout << left << setw(indent) << "Address: " << res->getString(3) << " " << res->getString(4) <<
			", " << res->getString(5) << ", " << res->getString("addr_state") << endl;
		cout << left << setw(indent) << "Contact no.: " << res->getString("phn") << endl;
		cout << left << setw(indent) << "Date of Joining: " << res->getString("date_of_joining") << endl;
		cout << left << setw(indent) << "Salary: " << "$" << res->getString("salary") << endl << endl << endl;
	}
	else
	{
		cout << "No Employee Found!!" << endl << endl << endl;
	}
}
void employees::display()
{
	int i = 0, indent = 25;
	query = "SELECT * FROM employees;";
	res = stmt->executeQuery(query);
	if (res->next())
	{
		do
		{
			cout << "Employees Details of Emp No." << ++i << endl;
			cout << left << setw(indent) << "Name: " << res->getString("name") << endl;
			cout << left << setw(indent) << "Address: " << res->getString(3) << " " << res->getString(4) <<
				", " << res->getString(5) << ", " << res->getString("addr_state") << endl;
			cout << left << setw(indent) << "Contact no.: " << res->getString("phn") << endl;
			cout << left << setw(indent) << "Date of Joining: " << res->getString("date_of_joining") << endl;
			cout << left << setw(indent) << "Salary: " << "$" << res->getString("salary") << endl << endl;
			cout << endl << endl;
		} while (res->next());
	}
	else
	{
		cout << "Employees Not found!" << endl;
	}
}
void employees::update_sal()
{
	stringstream strstr;
	cout << "Enter the id to update the salary of an employee: ";
	cin >> id;
	cout << "Enter the updated salary: ";
	cin >> salary;
	strstr.str("");
	strstr << "UPDATE employees SET salary = '" << salary << "' WHERE id = " << id << ";";
	stmt->executeUpdate(strstr.str());
	cout << endl << endl;
	cout << "Salary update Successfully";
	cout << endl << endl;
}
/*
//Members
*/
void members::add_mem()
{
	int idnum = 0;
	res = stmt->executeQuery("SELECT id FROM members ORDER BY id DESC LIMIT 1;");
	while (res->next())
	{
		idnum = (res->getInt(1)) + 1;
	}

	stringstream strstr;
	cout << "Enter the name of the member: ";
	cin >> name;
	cout << "Enter phone no.: ";
	cin >> phn;
	cout << "Enter address (in 3 lines)" << endl;
	cin >> addr_line1;
	cin >> addr_line2;
	cin >> addr_city;
	cout << "Enter the name of the state: ";
	cin >> addr_state;
	strstr.str("");
	strstr << "INSERT INTO members(id,name,addr_line1,addr_line2,addr_city,addr_state,phn,beg_date,end_date) VALUES(" << idnum << ",'" << name << "','" <<
		addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "','" << phn << "',curdate(),Date_add(curdate(), INTERVAL 1 YEAR))";
	stmt->execute(strstr.str());
	// fetching member id...
	strstr.str("");
	strstr << "SELECT id FROM members WHERE phn = '" << phn << "';";
	res = stmt->executeQuery(strstr.str());
	cout << endl << endl;
	while (res->next())
	{
		cout << "Member Added successfully" << endl << endl << "Member Id: " << res->getInt("id");
		stmt->execute("UPDATE members SET valid = 'valid';");
	}
	cout << endl << endl;
}
void members::refresh()
{
	query = "UPDATE members SET valid = 'invalid' WHERE end_date <= curdate();";
	stmt->executeUpdate(query);
}
void members::search_mem()
{
	int indent = 25;
	stringstream strstr;
	cout << "Enter member id: ";
	cin >> id;
	strstr.str("");
	strstr << "SELECT * FROM members WHERE id  = " << id << ";";
	res = stmt->executeQuery(strstr.str());
	if (res->next())
	{
		cout << left << setw(indent) << "Member Details" << endl;
		cout << left << setw(indent) << "Name: " << res->getString(2) << endl;
		cout << left << setw(indent) << "Address: " << res->getString(3) << " " << res->getString(4) << ", " << res->getString(5) << ", " << res->getString(6) << endl;
		cout << left << setw(indent) << "Contact no.: " << res->getString(7) << endl;
		cout << left << setw(indent) << "Membership begin date: " << res->getString(8) << endl;
		cout << left << setw(indent) << "Membership end date: " << res->getString(9) << endl;
		cout << left << setw(indent) << "Membership Status: " << res->getString(10) << endl << endl;
	}
	else
	{
		cout << "No Member Found!!" << endl << endl << endl;
	}
}
/*
//Purchases
*/
void purchases::new_ord()
{
	stringstream strstr;
	cout << "Enter the book Id: ";
	cin >> book_id;
	cout << "Enter Supplier Id: ";
	cin >> sup_id;
	cout << "Enter the Quantity: ";
	cin >> qty;
	cout << "Estimated expected Delivery (in days): ";
	cin >> eta;
	strstr.str("");
	strstr << "Insert into purchases(book_id, sup_id, qty, dt_ordered, eta) values(" << book_id << "," << sup_id << "," << qty <<
		"," << "curdate(), Date_add(curdate(), INTERVAL " << eta << " DAY))";
	stmt->execute(strstr.str());
	cout << "New order Added!!";
}
void purchases::mark_reciv()
{
	stringstream strstr;
	cout << "Enter the order id for order recieved: ";
	cin >> ord_id;
	strstr.str("");
	strstr << "UPDATE purchases SET received = 'T' WHERE ord_id = " << ord_id << ";";
	stmt->execute(strstr.str());
	cout << "Received Marked successfully\n\n";
}
void purchases::mar_cancel()
{
	stringstream strstr;
	cout << "Enter the order id for order cancelled : ";
	cin >> ord_id;
	strstr.str("");
	strstr << "UPDATE purchases SET received = 'C' WHERE ord_id = " << ord_id << ";";
	stmt->execute(strstr.str());
	cout << "Cancelled Marked successfully";
}
void purchases::view()
{
	stringstream strstr;
	int c, indent = 28;
	cout << "Select an Option" << endl;
	cout << "1. View orders not Recieved" << endl;
	cout << "2. View orders Cancelled" << endl;
	cout << "3. View orders Recieved" << endl;
	cout << "Enter Your choice: ";
	cin >> c;
	if (c == 1)
		received = 'F';
	else if (c == 2)
		received = 'C';
	else if (c == 3)
		received = 'T';
	else
		return;
	strstr.str("");
	strstr << "SELECT * FROM purchases WHERE received = '" << received << "';";
	res = stmt->executeQuery(strstr.str());
	if (c == 1)
		cout << endl << "Orders not recieved are" << endl;
	else if (c == 2)
		cout << endl << "Orders Cancelled are" << endl;
	else if (c == 3)
		cout << endl << "Orders recieved are" << endl;
	while (res->next())
	{
		cout << endl;
		cout << left << setw(indent) << "Order Id: " << res->getInt(1) << endl;
		cout << left << setw(indent) << "Book Id: " << res->getInt(2) << endl;
		cout << left << setw(indent) << "Supplier Id:" << res->getInt(3) << endl;
		cout << left << setw(indent) << "Quantity: " << res->getInt(4) << endl;
		cout << left << setw(indent) << "Date Ordered: " << res->getString(5) << endl;
		cout << left << setw(indent) << "Estimated Delivery date: " << res->getString(6) << endl;
		cout << endl << endl << endl;
	}
	cout << endl << endl << endl << endl << endl;
}
/*
//Sales
*/
void sales::add()
{
	stringstream strstr;
	cout << "Enter member id: ";
	cin >> member_id;
	cout << "Enter the book id: ";
	cin >> book_id;
	cout << "Enter the quantity: ";
	cin >> qty;
	strstr.str("");
	strstr << "SELECT price*" << qty << " FROM books WHERE id = " << book_id << ";";
	res = stmt->executeQuery(strstr.str());
	if (res->next())
	{
		cout << "The bill amount: $" << res->getInt(1) << endl << endl;
		amount = res->getInt(1);
	}
	else
	{
		cout << "Book Id invalid!!" << endl;
		return;
	}
	strstr.str("");
	strstr << "INSERT INTO sales(member_id,book_id,qty,amount,date_s) VALUES(" << member_id << "," << book_id << "," << qty << "," << amount << ",curdate());";
	stmt->execute(strstr.str());
	// fetching invoice id...
	strstr.str("");
	strstr << "SELECT invoice_id FROM sales WHERE member_id = " << member_id << " AND book_id = " << book_id << " AND qty = " << qty << " AND date_s = curdate();";
	res = stmt->executeQuery(strstr.str());
	if (res->next())
	{
		cout << "The Invoice id for the bill is " << res->getInt(1) << endl << endl;
	}
	else
	{
		cout << "The entered details may be wrong." << endl << "Please Recheck and Enter again" << endl << endl;
	}
}
void sales::find_total_sales()
{
	query = "SELECT SUM(amount) FROM sales WHERE YEAR(date_s) = YEAR(curdate());";
	res = stmt->executeQuery(query);
	if (res->next())
	{
		cout << "Total sales this year: $" << res->getInt(1) << endl << endl;
	}
}
/*
//Suppliers
*/
void suppliers::add_sup()
{
	int idnum = 0;
	res = stmt->executeQuery("SELECT id FROM suppliers ORDER BY id DESC LIMIT 1;");
	while (res->next())
	{
		idnum = (res->getInt(1)) + 1;
	}

	stringstream strstr;
	cout << "Enter the Supplier Name: ";
	cin >> name;
	cout << "Enter Phone no.: ";
	cin >> phn;
	cout << "Enter the address (in 3 lines): ";
	cin >> addr_line1;
	cin >> addr_line2;
	cin >> addr_city;
	cout << "Enter State: ";
	cin >> addr_state;
	strstr.str("");
	strstr << "INSERT INTO suppliers VALUES('" << idnum << "','" << name << "'," << phn << ",'" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "');";
	stmt->execute(strstr.str());
	if (stmt)
		cout << endl << endl << "Supplier Record Inserted Successfully" << endl << endl << endl;
	else
		cout << endl << endl << "Entry ERROR !" << endl << "Contact Technical Team " << endl << endl << endl;
}
void suppliers::remove_supplier()
{
	stringstream strstr;
	cout << "enter the supplier id to remove the supplier: ";
	cin >> id;
	strstr.str("");
	strstr << "DELETE FROM suppliers WHERE id = " << id << ";";
	stmt->execute(strstr.str());
	cout << "Supplier removed.";
}
void suppliers::search_id()
{
	stringstream strstr;
	int indent = 25;
	cout << "Enter the supplier id to find the Supplier details: ";
	cin >> id;
	strstr.str("");
	strstr << "SELECT * FROM suppliers WHERE id = " << id << ";";
	res = stmt->executeQuery(strstr.str());
	if (res->next())
	{
		cout << left << setw(indent) << "Details of Supplier Id: " << res->getInt(1) << endl;
		cout << left << setw(indent) << "Name: " << res->getString(2) << endl;
		cout << left << setw(indent) << "Phone no.: " << res->getString(3) << endl;
		cout << left << setw(indent) << "Address Line 1: " << res->getString(4) << endl;
		cout << left << setw(indent) << "Address Line 2: " << res->getString(5) << endl;
		cout << left << setw(indent) << "City: " << res->getString(6) << endl;
		cout << left << setw(indent) << "State: " << res->getString(7) << endl;
	}
	else
	{
		cout << "No Supplier Found!!\n\n";
	}
}




void book_menu();
void sup_menu();
void pur_menu();
void emp_menu();
void mem_menu();
void sal_menu();

void main_menu()
{
	int c;
	cout << "*************************************************" << endl;
	cout << "         BOOKSHOP MANGEMENT SYSTEM" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. BOOKS" << endl;
	cout << "   2. SUPPLIERS" << endl;
	cout << "   3. PURCHASES" << endl;
	cout << "   4. EMPLOYEES" << endl;
	cout << "   5. MEMBERS" << endl;
	cout << "   6. SALES" << endl;
	cout << "   7. EXIT" << endl << endl << endl;
	cout << "Enter Your Choice: ";
	cin >> c;
	switchVar = inputValidation(c);
	cout << endl;
	switch (switchVar)
	{
	case 1:
		book_menu();
		break;
	case 2:
		sup_menu();
		break;
	case 3:
		pur_menu();
		break;
	case 4:
		emp_menu();
		break;
	case 5:
		mem_menu();
		break;
	case 6:
		sal_menu();
		break;
	case 7:
		cout << "Sayonara!\n";
		exit(1);
	default:
		cout << "Out of range, try again\n\n";
		break;
	}
	return;
}
void book_menu()
{
	int c;
	books b;
	cout << "*************************************************" << endl;
	cout << "                  BOOK MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. ADD" << endl;
	cout << "   2. multiADD" << endl;
	cout << "   3. UPDATE PRICE" << endl;
	cout << "   4. SEARCH" << endl;
	cout << "   5. UPDATE STATUS" << endl;
	cout << "   6. DISPLAY ALL" << endl;
	cout << "   7. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice: ";
	cin >> c;
	switchVar = inputValidation(c);
	cout << endl;
	switch (switchVar)
	{
	case 1:
		b.add();
		break;
	case 2:
		b.multiAdd();
		break;
	case 3:
		b.update_price();
		break;
	case 4:
		b.search();
		break;
	case 5:
		b.update();
		break;
	case 6:
		b.display();
		break;
	case 7:
		return;
	default:
		cout << "Out of range, try again\n\n";
		break;
	}
	Returnmenu();
}
void sup_menu()
{
	int c;
	suppliers s;
	cout << "*************************************************" << endl;
	cout << "                SUPPLIER MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. ADD" << endl;
	cout << "   2. REMOVE" << endl;
	cout << "   3. SEARCH" << endl;
	cout << "   4. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice: ";
	cin >> c;
	switchVar = inputValidation(c);
	cout << endl;
	switch (switchVar)
	{
	case 1:
		s.add_sup();
		break;
	case 2:
		s.remove_supplier();
		break;
	case 3:
		s.search_id();
		break;
	case 4:
		return;
	default:
		cout << "Out of range, try again\n\n";
		break;
	}
	Returnmenu();
}
void pur_menu()
{
	int c;
	purchases p;
	cout << "*************************************************" << endl;
	cout << "                PURCHASES MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Order" << endl;
	cout << "   2. View All" << endl;
	cout << "   3. Cancel Order" << endl;
	cout << "   4. Recieved Order" << endl;
	cout << "   5. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice: ";
	cin >> c;
	switchVar = inputValidation(c);
	cout << endl;
	switch (switchVar)
	{
	case 1:
		p.new_ord();
		break;
	case 2:
		p.view();
		break;
	case 3:
		p.mar_cancel();
		break;
	case 4:
		p.mark_reciv();
		break;
	case 5:
		return;
	default:
		cout << "Out of range, try again\n\n";
		break;
	}
	Returnmenu();
}
void emp_menu()
{
	int c;
	employees e;
	cout << "*************************************************" << endl;
	cout << "                 EMPLOYEE MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Employee" << endl;
	cout << "   2. Search Employee" << endl;
	cout << "   3. Assign Manager" << endl;
	cout << "   4. View All" << endl;
	cout << "   5. Update Salary" << endl;
	cout << "   6. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice: ";
	cin >> c;
	switchVar = inputValidation(c);
	cout << endl;
	switch (switchVar)
	{
	case 1:
		e.add_emp();
		break;
	case 2:
		e.search_emp();
		break;
	case 3:
		e.assign_mgr_stat();
		break;
	case 4:
		e.display();
		break;
	case 5:
		e.update_sal();
		break;
	case 6:
		return;
	default:
		cout << "Out of range, try again\n\n";
		break;
	}
	Returnmenu();
}
void mem_menu()
{
	int c;
	members m;
	m.refresh();
	cout << "*************************************************" << endl;
	cout << "                 MEMBERS MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Member" << endl;
	cout << "   2. Search Member" << endl;
	cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice: ";
	cin >> c;
	switchVar = inputValidation(c);
	cout << endl;
	switch (switchVar)
	{
	case 1:
		m.add_mem();
		break;
	case 2:
		m.search_mem();
		break;
	case 3:
		return;
	default:
		cout << "Out of range, try again\n\n";
		break;
	}
	Returnmenu();
}
void sal_menu()
{
	int c;
	sales s;
	cout << "*************************************************" << endl;
	cout << "                 SALES MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. Add New Bill" << endl;
	cout << "   2. Total Sales Of The Year" << endl;
	cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice: ";
	cin >> c;
	switchVar = inputValidation(c);
	cout << endl;
	switch (switchVar)
	{
	case 1:
		s.add();
		break;
	case 2:
		s.find_total_sales();
		break;
	case 3:
		return;
	default:
		cout << "Wrong Input" << endl << "Invalid input";
		break;
	}
	Returnmenu();
}
int main(void)
{
	try {
		string s1, s2;
		cout << "Enter mysql user and password here: ";
		cin >> s1 >> s2;
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", s1, s2);
		con->setSchema("Management");
		stmt = con->createStatement();

		int choice = {};
		if (con)
			while (1)
				main_menu();
		else
			cout << "\nError While connecting to database." << endl << "Contact Tech Expert." << endl;

		delete res;
		delete stmt;
		delete con;
		cout << "\nSuccess\n";
	}

	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	return EXIT_SUCCESS;
}
void Returnmenu()
{
	cout << "Press 'b' key to continue: ";
	char val;
	cin >> val;
	cout << endl;
	if (val == 'b')
		return;
	else
	{
		while (val != 'b')
		{
			cout << "illegal value entered, please try again: ";
			cin >> val;
			cout << endl;
		}
	}
}
int inputValidation(int c)
{
	while (1)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Must enter integer, try again: ";
			cin >> c;
			cout << endl;
		}
		if (!cin.fail())
			return c;
	}
}