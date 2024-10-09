/*******************************************************************************
 * Filename: fastmult.cpp
 * Author : Alice Agoletto
 * Version : 1.0
 * Date : 11/18/2023
 * Description: Karatsuba's fast multiplication algorithm.
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

/*
    pseudocode:

    multiply

        split a and b into a0,b0 and a1,b1
        basecase: a and b are 1 digit

        c2 <- multiply(a1,b1)
        c0 <- multiply(a0,b0)
        c1 <- subtract( multiply( add(a1,a0), add(b1,b0) ), add(c2,c0))
        c <- c2 * 10^n + c1 * 10^ n/2 + c0

*/

/*
    removeLeadingZeros: removes the leading zeroes of a string number
*/
string removeLeadingZeros(const string &s)
{
    size_t pos = s.find_first_not_of('0');
    // returns the string starting at the position of the first non zero digit
    return s.substr(min(pos, s.size() - 1));
}

/*
    powerTen: multiplies num by 10^power, without having to convert into int
*/
string powerTen(const string &num, const int power)
{
    if (power == 0)
    {
        // num*10^0 = num*1=num
        return num;
    }

    string result = num;
    // loop to append '0's
    for (int i = 0; i < power; ++i)
    {
        // Append '0' to the end
        result += '0';
    }
    return result;
}

/*
    padZeroes: adds zeroes to the front of the string to make sure both strings (a,b)
    have the same length. The set length is passed.
*/
string padZeroes(const string &s, size_t length)
{
    string result;

    if (s.length() >= length)
    {
        return s;
    }

    result = s;
    while (result.length() < length)
    {
        result = '0' + result;
    }

    return result;
}

/*
    subtract: subtract two numbers in the form of strings without having to convert the whole
    numbers. Helps when the numbers are too big.
*/
string subtract(const string &a, const string &b)
{
    // Get signs of a and b
    int signA, signB;
    if (a[0] == '-') {
        signA = -1;
    } else {
        signA = 1;
    }

    if (b[0] == '-') {
        signB = -1;
    } else {
        signB = 1;
    }

    // Get values of a and b
    string numA, numB;
    if (signA == -1) {
        numA = a.substr(1);
    } else {
        numA = a;
    }

    if (signB == -1) {
        numB = b.substr(1);
    } else {
        numB = b;
    }

    // Get lengths of numA and numB
    int sizeA = numA.size();
    int sizeB = numB.size();

    // Check which one is smaller
    if (sizeA < sizeB || (sizeA == sizeB && numA < numB)) {
        // Swap numbers and add a negative sign if necessary
        string result = subtract(b, a); // Recursive call to ensure positive result
        if (result == "0") {
            return result;
        } else {
            return '-' + result;
        }
    }

    int borrow = 0;
    string result;

    // Perform digit-wise subtraction
    for (int i = sizeA - 1, j = sizeB - 1; i >= 0 || j >= 0; i--, j--)
    {
        int digitA, digitB;

        if (i >= 0) {
            digitA = numA[i] - '0';
        } else {
            digitA = 0;
        }

        if (j >= 0) {
            digitB = numB[j] - '0';
        } else {
            digitB = 0;
        }

        int sub = (signA * digitA) - (signB * digitB) - borrow;

        if (sub < 0)
        {
            sub += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        result = to_string(sub) + result;
    }

    // Remove leading zeros from result
    return removeLeadingZeros(result);
}

/*
    add: adds two string numbers without having to convert each one into integers
*/
string add(const string &a, const string &b)
{
    // get signs of a and b
    int signA, signB;
    if (a[0] == '-')
    {
        signA = -1;
    }
    else
    {
        signA = 1;
    }
    if (b[0] == '-')
    {
        signB = -1;
    }
    else
    {
        signB = 1;
    }

    // Get values of a and b
    string numA, numB;

    if (signA == -1)
    {
        numA = a.substr(1);
    }
    else
    {
        numA = a;
    }
    if (signB == -1)
    {
        numB = b.substr(1);
    }
    else
    {
        numB = b;
    }

    int carry = 0;
    int i = numA.length() - 1;
    int j = numB.length() - 1;
    string result = "";

    // Perform digit-wise addition
    while (i >= 0 || j >= 0 || carry > 0)
    {
        int digitA, digitB;
        if (i >= 0)
        {
            digitA = numA[i] - '0';
        }
        else
        {
            digitA = 0;
        }

        if (j >= 0)
        {
            digitB = numB[j] - '0';
        }
        else
        {
            digitB = 0;
        }

        int sum = signA * digitA + signB * digitB + carry;
        // Update carry for the next iteration
        carry = sum / 10;
        // Get the single digit sum
        sum = abs(sum) % 10;

        // result string
        result = to_string(sum) + result;

        i--;
        j--;
    }

    // Adjust result based on signs and handle negative numbers
    if ((signA == -1 && signB == -1) || (signA == 1 && signB == 1))
    {
        if (signA == -1)
        {
            result = '-' + result;
        }
    }
    else
    {
        if (signA == -1)
        {
            result = subtract(b, a.substr(1)); // Subtract a from b if signs are different
        }
        else
        {
            result = subtract(a, b.substr(1)); // Subtract b from a if signs are different
        }
    }

    // Remove leading zeros from result
    return removeLeadingZeros(result);
}

/*
    multiply:
    KARATSUBA's algorithm
    recursively iterate until a and b are 1 digit
        split a and b into a0,b0 and a1,b1
        basecase: a and b are 1 digit

        c2 <- multiply(a1,b1)
        c0 <- multiply(a0,b0)
        c1 <- subtract( multiply( add(a1,a0), add(b1,b0) ), add(c2,c0))
        c <- c2 * 10^n + c1 * 10^ n/2 + c0

*/
string multiply(const string &a, const string &b)
{
    if (a.length() <= 1 && b.length() <= 1)
    {
        // Base case: single-digit multiplication
        unsigned long long int p = stoull(a) * stoull(b); // used long long int just in case
        return to_string(p);
    }
    else
    {
        unsigned long long int n = max(a.length(), b.length());
        unsigned long long int m = n / 2;

        string a1, a0, b1, b0;
        string newb, newa;

        // if/else if statements: check if a and b are the same length or if either
        // of them is only one digit long, in that case the value of the first substring must be 0
        if (a.length() == b.length())
        {
            a1 = a.substr(0, m);
            a0 = a.substr(m);
            b1 = b.substr(0, m);
            b0 = b.substr(m);
        }
        else if (a.length() > b.length() && b.length() == 1)
        {
            a1 = a.substr(0, m);
            a0 = a.substr(m);
            b1 = "0";
            b0 = b;
        }
        else if (a.length() < b.length() && a.length() == 1)
        {
            a1 = "0";
            a0 = a;
            b1 = b.substr(0, m);
            b0 = b.substr(m);
        }
        else
        {
            // find the longest length between a and b
            int x = max(a.length(), b.length());

            // pad with zeroes the smallest

            if (a.length() > b.length())
            {
                newb = padZeroes(b, x);
                newa = a;
            }
            else if (a.length() < b.length())
            {
                newa = padZeroes(a, x);
                newb = b;
            }

            a1 = newa.substr(0, m);
            a0 = newa.substr(m);
            b1 = newb.substr(0, m);
            b0 = newb.substr(m);
        }

        // recursively compute the sub-products
        string c0 = multiply(a0, b0); // c2 <- a1*b1
        string c2 = multiply(a1, b1); // c0 <-a0*b0
        // c1 <- (a1+a0)*(b1+b0) - (c2+c0)
        string c1 = subtract(multiply(add(a1, a0), add(b1, b0)), add(c2, c0));

        // fix the n and m for the exponent (must always be even)
        if (n % 2 != 0)
        {
            // if n is odd, make it even by adding 1
            n++;
        }
        m = n / 2;

        // c <- c2 * 10^n + c1 * 10^ n/2 + c0
        //  split into pieces
        string c2_pow = powerTen(c2, n);
        string c1_pow = powerTen(c1, m);

        string c = add(add(c2_pow, c1_pow), c0);

        return c;
    }
}

int main(int argc, char *argv[])
{
    string a, b;

    istringstream iss;

    if (argc == 3)
    {
        // get arg values
        a = argv[1];
        b = argv[2];

        // multiply
        string result = multiply(a, b);
        // print result
        cout << result << endl;

        return 0;
    }
    else
    {
        cerr << "Usage: " << argv[0] << " <string> <string>" << endl;
        return 1;
    }
}
