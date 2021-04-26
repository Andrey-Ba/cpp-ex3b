#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

namespace ariel
{
    class NumberWithUnits
    {
    private:
        static std::map<std::string, std::map<std::string, double>*> conversions;
        double amount;
        std::string unit_type;
        
        static void input_check(std::string input);
        static double convert(std::string from, std::string to, double amount);

        static void freemap();
        static void print();
        
    public:
        NumberWithUnits()
        {
            amount = 0;
            unit_type = "None";
        }

        NumberWithUnits(double amount, std::string unit_type)
        {
            input_check(unit_type);
            this->amount = amount;
            this->unit_type = unit_type;
        }

        ~NumberWithUnits(){}
        static void read_units(std::ifstream& file);

        friend const NumberWithUnits operator+(const NumberWithUnits& n);
        const NumberWithUnits operator+(const NumberWithUnits& other);
        NumberWithUnits& operator+=(const NumberWithUnits& other);
        const NumberWithUnits operator+();
        friend const NumberWithUnits operator+(const NumberWithUnits& n1, const NumberWithUnits& n2);
        const NumberWithUnits operator-(const NumberWithUnits& other);
        friend const NumberWithUnits operator-(const NumberWithUnits& n);
        NumberWithUnits& operator-=(const NumberWithUnits& other);
        const NumberWithUnits operator-();
        friend const NumberWithUnits operator-(const NumberWithUnits& n1, const NumberWithUnits& n2);

        friend bool operator>(const NumberWithUnits& n1, const NumberWithUnits& n2);
        friend bool operator>=(const NumberWithUnits& n1, const NumberWithUnits& n2);
        friend bool operator<(const NumberWithUnits& n1, const NumberWithUnits& n2);
        friend bool operator<=(const NumberWithUnits& n1, const NumberWithUnits& n2);
        friend bool operator==(const NumberWithUnits& n1, const NumberWithUnits& n2);
        friend bool operator!=(const NumberWithUnits& n1, const NumberWithUnits& n2);

        NumberWithUnits& operator++();
        const NumberWithUnits operator++(int flag);
        NumberWithUnits& operator--();
        const NumberWithUnits operator--(int flag);

        friend const NumberWithUnits operator*(double x, const NumberWithUnits& n);
        friend const NumberWithUnits operator*(const NumberWithUnits& n, double x);

        friend std::ostream& operator<<(std::ostream& os, const NumberWithUnits& n);
        friend std::istream& operator>>(std::istream& is, NumberWithUnits& n);
    };
}