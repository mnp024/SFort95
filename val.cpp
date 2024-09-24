//Michael Piscopo
//mnp24
//CS280-012

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <sstream>
using namespace std;
#include "val.h"

//plus
Value Value::operator+(const Value& op) const{
    if(IsInt() && op.IsInt()){
        Value op3(int (GetInt() + op.GetInt()));
        return op3;
    } else if (IsReal() && op.IsInt()){
        Value op3(double (GetReal() + op.GetInt()));
        return op3;
    } else if (IsInt() && op.IsReal()){
        Value op3(double (GetInt() + op.GetReal()));
        return op3;
    } else if (IsReal() && op.IsReal()){
        Value op3(double (GetReal() + op.GetReal()));
        return op3;
    } else {
        Value op3;
        return op3;
    }
}

//minus
Value Value::operator-(const Value& op) const{
    if(IsInt() && op.IsInt()){
        Value op3(int (GetInt() - op.GetInt()));
        return op3;
    } else if (IsReal() && op.IsInt()){
        Value op3(double (GetReal() - op.GetInt()));
        return op3;
    } else if (IsInt() && op.IsReal()){
        Value op3(double (GetInt() - op.GetReal()));
        return op3;
    } else if (IsReal() && op.IsReal()){
        Value op3(double (GetReal() - op.GetReal()));
        return op3;
    } else {
        Value op3;
        return op3;
    }
}

//multiply
Value Value::operator*(const Value& op) const{
    if(IsInt() && op.IsInt()){
        Value op3(int (GetInt() * op.GetInt()));
        return op3;
    } else if (IsReal() && op.IsInt()){
        Value op3(double (GetReal() * op.GetInt()));
        return op3;
    } else if (IsInt() && op.IsReal()){
        Value op3(double (GetInt() * op.GetReal()));
        return op3;
    } else if (IsReal() && op.IsReal()){
        Value op3(double (GetReal() * op.GetReal()));
        return op3;
    } else {
        Value op3;
        return op3;
    }
}

//divide
Value Value::operator/(const Value& op) const{
    if(IsInt() && op.IsInt()){
        Value op3(double (GetInt() / op.GetInt())); //return double or int? int division??
        return op3;
    } else if (IsReal() && op.IsInt()){
        Value op3(double (GetReal() / op.GetInt()));
        return op3;
    } else if (IsInt() && op.IsReal()){
        Value op3(double (GetInt() / op.GetReal()));
        return op3;
    } else if (IsReal() && op.IsReal()){
        Value op3(double (GetReal() / op.GetReal()));
        return op3;
    } else {
        Value op3;
        return op3;
    }
}

//less than
Value Value::operator<(const Value& op) const{
    if(IsInt() && op.IsInt()){
        Value op3(bool (GetInt() < op.GetInt()));
        return op3;
    } else if (IsReal() && op.IsInt()){
        Value op3(bool (GetReal() < op.GetInt()));
        return op3;
    } else if (IsInt() && op.IsReal()){
        Value op3(bool (GetInt() < op.GetReal()));
        return op3;
    } else if (IsReal() && op.IsReal()){
        Value op3(bool (GetReal() < op.GetReal()));
        return op3;
    } else { //add IsString() && op.IsString()?
        Value op3;
        return op3;
    }
}

//greater than
Value Value::operator>(const Value& op) const{
    if(IsInt() && op.IsInt()){
        Value op3(bool (GetInt() > op.GetInt()));
        return op3;
    } else if (IsReal() && op.IsInt()){
        Value op3(bool (GetReal() > op.GetInt()));
        return op3;
    } else if (IsInt() && op.IsReal()){
        Value op3(bool (GetInt() > op.GetReal()));
        return op3;
    } else if (IsReal() && op.IsReal()){
        Value op3(bool (GetReal() > op.GetReal()));
        return op3;
    } else { //add IsString() && op.IsString()?
        Value op3;
        return op3;
    }
}

//equals
Value Value::operator==(const Value& op) const{
    if(IsInt() && op.IsInt()){
        Value op3(bool (GetInt() == op.GetInt()));
        return op3;
    } else if (IsReal() && op.IsInt()){
        Value op3(bool (GetReal() == op.GetInt()));
        return op3;
    } else if (IsInt() && op.IsReal()){
        Value op3(bool (GetInt() == op.GetReal()));
        return op3;
    } else if (IsReal() && op.IsReal()){
        Value op3(bool (GetReal() == op.GetReal()));
        return op3;
    } else { //add IsString() && op.IsString()?
        Value op3;
        return op3;
    }
}

//concatenate
Value Value::Catenate(const Value& op) const{
    if(IsString() && op.IsString()){
        Value op3(string (GetString() + op.GetString()));
        return op3;
    } else {
        Value op3;
        return op3;
    }
}

//power
Value Value::Power(const Value& op) const{
    if(IsInt() && op.IsInt()){
        double temp = pow(GetInt(), op.GetInt());
        Value op3(temp);
        return op3;
    } else if (IsReal() && op.IsInt()){
        double temp = pow(GetReal(), op.GetInt());
        Value op3(temp);
        return op3;
    } else if (IsInt() && op.IsReal()){
        double temp = pow(GetInt(), op.GetReal());
        Value op3(temp);
        return op3;
    } else if (IsReal() && op.IsReal()){
        double temp = pow(GetReal(), op.GetReal());
        Value op3(temp);
        return op3;
    } else {
        Value op3;
        return op3;
    }
}


