// Individual Final Project
// This program will perform the first derivative test and find any possible critical points.
// Connor Robison
#include<iostream>
#include<vector>
#include<cmath>
using namespace std;

/* This class will help store needed inforation about the equation */
class Polynomial
{
private:
	static const int CS = 6;
	static const int DS = 5;
	const string term[6] = { " ", "x", "x^2", "x^3", "x^4", " " };
	const int exponent[5] = { 4, 3, 2, 1, 0 };		// Holds the degree of each term
	int coefficient[CS];							// Holds the coefficient of each term
	int derivative[DS];								// Helps to store the derivative of the user's equation
	int numPS;										// The number of possible solutions
	int fndSol;										// The number of solutions that have been found
	vector<int>numerator;							/* These two vector will help to store possible solutions */
	vector<int>denominator;

public:
	Polynomial();
	void setCoe(int);								// This function helps the user to create their equation
	void setDer();									// This function finds the derivative
	void setNum(int);								// This function will find factors of the leading coefficient
	void setDen(int);								// This function will find factors of the constant
	void setNumPS(int);								// This function will store the number of possible solutions
	int getNumPS();									// This function returns numPS
	int getFndSol();								// This function returns fndSol
	int findC(int[], int);							// This function will find the constant
	int findLC(int[], int);							// This function will find the leading coefficient
	void displayEq(int[], int, int);				// This function will use an array to display an equation
};

/* This class will eliminate extranuous solutions */
class Solution
{
private:
	vector<int>nSol;								// This vector will hold the numerator of true solutions
	vector<int>dSol;								// This vector will hold the denominator of true solutions
public:
	Solution();
	bool checkSol(int, int, int[], int, int);		// This funtion checks possible solutions from the numerator and denominator vectors
	void setNSol(int);								// This function will add the numerators of true solutions to nSol
	void setDSol(int);								// This function will add the denominators of true solutions to dSol
	void displayPCP();								// This fuction will diplay the possible critical points
};

/* Function prototypes */
int validateInput(int, int);
int displayMenu();
int gcd(int, int);									/*        I DID NOT WRITE THESE        */
int findGCD(int[], int);							/*        TWO FUNCTIONS		           */

int main()
{
	Polynomial eq;
	Solution sol;
	int choice;

	choice = displayMenu();
	eq.setNumPS(choice);
	eq.setCoe(choice);

	cout << endl; system("pause");
	return 0;
}

int validateInput(int min, int max)
{
	int choice = -1;

	while (choice < min || choice > max) {
		cin >> choice;
		cin.ignore();

		if ((choice > max || choice < min) || (choice % 1 != 0))
			cout << "     Enter an integer between " << min << " and " << max << ": ";
	}

	return choice;
}

int  displayMenu()
{
	Polynomial eq;

	system("CLS");

	cout << "This program will test to to see if possible critical points exist\nfor any of the following functions using the first derivative test." << endl;
	cout << "A critical point is any point on a graph where the slope is equal to zero or undefined." << endl;
	cout << "Quadratic and quartic functions are the most likely to contain critical points\nand linear functions do not have any critical points." << endl;
	cout << "\nFeel free to check any answers provided with a separate graphing utility." << endl;
	cout << "\nPlease choose one of the following:" << endl;
	cout << "1. Linear\n2. Quadratic\n3. Cubic\n4. Quartic" << endl << endl;

	int choice = validateInput(1, 4);				// The user has 4 options to choose from

	eq.setNumPS(choice);

	system("CLS");

	return choice;
}

/* Member functions for class Polynomial */
Polynomial::Polynomial() {
	coefficient[0] = derivative[0] = 1;				// This element holds a GCD

	for (int i = 1; i < CS; i++) {
		coefficient[i] = 0;
	}
	for (int i = 1; i < DS; i++) {
		derivative[i] = 0;
	}

	numerator.clear();
	denominator.clear();

	numPS = fndSol = 0;
}
void Polynomial::setCoe(int c) {
	system("CLS");

	cout << "Time to create your equation." << endl << endl;

	switch (c)
	{
	case 1:cout << "Linear:\n     parent function : f(x) = x\n     standard form : f(x) = mx + b" << endl; break;
	case 2:cout << "Quadratic:\n     parent function : f(x) = x^2\n     standard form : f(x) = ax^2 + bx + c" << endl; break;
	case 3:cout << "Cubic:\n     parent function : f(x) = x^3\n     standard form : f(x) = ax^3 + bx^2 + cx + d" << endl; break;
	case 4:cout << "Quartic:\n     parent function : f(x) = x^4\n     standard form : f(x) = ax^4 + bx^3 + cx^2 + dx + e" << endl; break;
	}

	const int min = 1, max = 50;

	cout << "\nPlease enter " << (numPS + 1) << " integers between " << min << " and " << max << "." << endl;
	cout << "For the best results, your last value should be greater than the first" << endl << endl;

	switch (c)
	{
	case 4: {
		cout << "Enter the number of x^4 you would like: ";
		int num = validateInput(min, max);
		coefficient[1] = num;
	}
	case 3: {
		cout << "Enter the number of x^3 you would like: ";
		int num = validateInput(min, max);
		coefficient[2] = num;
	}
	case 2: {
		cout << "Enter the number of x^2 you would like: ";
		int num = validateInput(min, max);
		coefficient[3] = num;
	}
	case 1: {
		cout << "Enter the number of x you would like: ";
		int num = validateInput(min, max);
		coefficient[4] = num;
	}
	default: {
		cout << "Enter a constant: ";
		int num = validateInput(min, max);
		coefficient[5] = num;
		setDer(); break;							// Calculate the derivative once the user has entered all their values and break
	}
	}

	cout << "\nYour equation: ";
	displayEq(coefficient, CS, numPS);

	cout << "\nDerivative: ";
	displayEq(derivative, DS, (numPS - 1));
	if (numPS != 1) {
		derivative[0] = findGCD(derivative, DS);	// Find a GCD if there is one
	}
	if (derivative[0] != 1) {
		cout << "Extracting a GCF..." << endl;

		for (int i = 1; i < DS; i++) {
			int temp = derivative[i];

			derivative[i] = (temp / derivative[0]);
		}

		cout << "\nNew derivative: ";
		displayEq(derivative, DS, (numPS - 1));
	}

	setNum(derivative[findC(derivative, DS)]);
	setDen(derivative[findLC(derivative, DS)]);
}
void Polynomial::setDer() {
	for (int i = 1; i < DS; i++)
	{
		derivative[i] = (exponent[i - 1] * coefficient[i]);
	}
}
void Polynomial::setNum(int c) {
	for (int i = 1; i <= c; i++) {
		if ((abs(c) % i) == 0) {
			numerator.push_back(i);
		}
	}
}
void Polynomial::setDen(int lc) {
	for (int i = 1; i <= lc; i++) {
		if ((abs(lc) % i) == 0) {
			denominator.push_back(i);
		}
	}

	cout << "\nThere are " << 2 * (numerator.size() * denominator.size()) << " possible critical points. Checking solutions..." << endl;

	Solution sol;

	for (int d : denominator) {
		for (int n : numerator) {
			if (sol.checkSol(-n, d, derivative, DS, (numPS - 1))) {
				sol.setNSol(-n);
				sol.setDSol(d);
				fndSol++;
			} cout << endl;
			if (sol.checkSol(n, d, derivative, DS, (numPS - 1))) {
				sol.setNSol(n);
				sol.setDSol(d);
				fndSol++;
			} cout << endl;
		}
	}

	sol.displayPCP();
}
void Polynomial::setNumPS(int ns) {
	numPS = ns;
}
int Polynomial::getNumPS() {
	return numPS;
}
int Polynomial::getFndSol() {
	return fndSol;
}
int Polynomial::findC(int list[], int size) {
	int index = (size - 1);							// Used as a subscript to search array 
	int position = -1;								// Used to record position of search value // Flag to indicate if the value was found 
	bool found = false;								// Flag to indicate if the value was found 

	while (index < size && !found)
	{
		if (list[index] != 0)						// If the value is found 
		{
			found = true;							// Set the flag  
			position = index;						// Record the value's subscript
		}
		index++;
		// Go to the next element
	}
	return position;								// Return the position, or -1 
}													// -1 indicates the value was not found 
int Polynomial::findLC(int list[], int size) {
	int index = 1;									// Used as a subscript to search array 
	int position = -1;								// Used to record position of search value // Flag to indicate if the value was found 
	bool found = false;								// Flag to indicate if the value was found 

	while (index < size && !found)
	{
		if (list[index] != 0)						// If the value is found 
		{
			found = true;							// Set the flag  
			position = index;						// Record the value's subscript
		}
		index++;
		// Go to the next element
	}
	return position;								// Return the position, or -1 
}													// -1 indicates the value was not found 
void Polynomial::displayEq(int a[], int s, int hp) {
	int start = findLC(a, s);						// Start at the leading coefficient
	int end = findC(a, s);							// End and the constant
	int k = 0;

	if (a[0] != 1)									// This is necessary to display the GCD if there is one
		cout << a[0] << " ( ";
	for (int i = start; i <= end; i++) {
		if (a[i] != 1 || i == end) {				// cout a[i] if it is not a constand or equal to one
			cout << a[i];
		} cout << term[(hp - k)]; k++;				// cout the respective term and increment k
		if (i != end && start != end)				// cout a + in between terms
			cout << " + ";
	}
	if (a[0] != 1)									// end with a parenthesis if there is one at the beginning
		cout << ")";
	cout << endl;
}

/* Member functions for class Solution */
Solution::Solution() {
	nSol.clear();
	dSol.clear();
}
bool Solution::checkSol(int n, int d, int de[], int s, int hp) {
	double sum = 0;
	int k = 0;

	for (int i = 1; i < s; i++) {
		sum += (static_cast<double>(de[i] * static_cast<double>(pow((static_cast<double>(n) / static_cast<double>(d)), (3 - k)))));
		k++;
	} sum *= static_cast<double>(de[0]);

	if (sum < 1e-10 && sum > -1e-10)
		sum = 0;

	cout << "     f'" << "( " << n;
	if (d != 1)
		cout << "/" << d;
	cout << " ) = " << sum;

	if (sum == 0) {
		return true;
	}
	else {
		return false;
	}
}
void Solution::setNSol(int n) {
	nSol.push_back(n);
}
void Solution::setDSol(int d) {
	cout << "; adding argument to the solution set";

	dSol.push_back(d);
}
void Solution::displayPCP() {
	if (nSol.empty())
		cout << "\nThere are not any possible critical points" << endl;
	else {
		cout << "\nPossible critical points include:" << endl;

		for (int i = 0; i < nSol.size(); i++) {
			cout << "     x = " << nSol[i];
			if (dSol[i] != 1)
				cout << "/" << dSol[i];
			cout << endl;
		}
	}
}

/*	THIS IS NOT MY ORIGINAL WORK	*/

// Function to return gcd of a and b
int gcd(int a, int b)
{
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

// Function to find gcd of array of
// numbers
int findGCD(int arr[], int n)
{
	int result = arr[1];									/*	THE SEARCH FOR A GCD NEEDS TO BEGIN AT  	*/
	for (int i = 2; i < n; i++)								/*	THE SECOND ELEMENT OF THE ARRAY				*/
	{
		result = gcd(arr[i], result);

		if (result == 1)
		{
			return 1;
		}
	}
	return result;
}

/*	I DID MODIFY THESE TWO FUNCTIONS SLIGHTLY	*/
