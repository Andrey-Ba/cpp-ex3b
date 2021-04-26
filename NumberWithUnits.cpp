#include "NumberWithUnits.hpp"

using namespace std;

namespace ariel
{
    // Init the map
    std::map<std::string, std::map<std::string, double>*> NumberWithUnits::conversions;

    void NumberWithUnits::input_check(string input)
    {
        if(conversions.find(input) == conversions.end())
        {
            throw invalid_argument("Not supported input");
        }
    }

    // Converts from one unit to another
    double NumberWithUnits::convert(string from, string to, double amount)
    {
        if(from == to){return amount;}
        if(conversions[from]->find(to) == conversions[from]->end())
        {
            throw invalid_argument("Not related units");
        }
        return amount * conversions[from]->at(to);
    }

    // Free the maps in the end of the program
    void NumberWithUnits::freemap()
    {
        map<string, map<string, double>*>::iterator itr = conversions.begin();
        unsigned int length = conversions.size();
        for(int i = 0; i < length; i++)
        {
            string key = itr->first;
            delete (conversions)[key];
            itr++;
        }
        conversions.clear();
    }

    void NumberWithUnits::read_units(ifstream& file)
    {
        // Make sure to free the maps in the end of the program
        std::atexit(freemap);
        string line = "";
        // While that reads the input file
        while (getline(file, line))
        {
            int length = line.length();
            string unit1 = "";
            string unit2 = "";
            string num;
            unsigned long i = 1;
            while (line[i] != '=')
            {
                if(line[i] != ' ')
                {
                 unit1 += line[i];
                }
                i++;
            }
            while(line[i] == '=' || line[i] == ' '){i++;}
            while (line[i] != ' ')
            {
                num += line[i];
                i++;
            }
            while (i < length)
            {
                if(line[i] != ' ')
                {                    
                unit2 += line[i];
                }
                i++;
            }
            // One of the units will have it's first entry (otherwise it's already in the hashmap of all similar units)
            // Save the one with the first entry. (And one might already is in the map but it doesn't matter)
            string new_unit = "";
            string existing_unit = "";
            // Checks which units didn't enter the map yet.
            if(conversions.find(unit1) == conversions.end()){new_unit = unit1; existing_unit = unit2; conversions.insert(pair<string, map<string, double>*>(unit1, new map<string, double>()));}
            if(conversions.find(unit2) == conversions.end()){new_unit = unit2; existing_unit = unit1;conversions.insert(pair<string, map<string, double>*>(unit2, new map<string, double>()));}
            conversions[unit1]->insert(pair<string, double>(unit2,stod(num)));
            conversions[unit2]->insert(pair<string, double>(unit1,1/stod(num)));

            if(new_unit == ""){continue;}

            // Take the an existing unit and go after it's conversion to add the 
            // new unit to all of them, and add them to the map of the new unit.
            // (If both units are new, the function wont do anything...)
            map<string, double>::iterator itr = conversions[existing_unit]->begin();
            length = conversions[existing_unit]->size();
            for(int i = 0; i < length; i++)
            {
                string unit = itr->first;
                if(unit != new_unit)
                {
                    // Conversion..
                    conversions[unit]->insert(pair<string, double>(new_unit, conversions[existing_unit]->at(new_unit) * conversions[unit]->at(existing_unit)));
                    conversions[new_unit]->insert(pair<string, double>(unit, conversions[existing_unit]->at(unit) * conversions[new_unit]->at(existing_unit)));
                }
                itr++;
            }

        }
        // NumberWithUnits::print();
    }

    // Function that prints all the units type and it's conversions. 
    // Usefull for checking correction.
    void NumberWithUnits::print()
    {
        map<string, map<string, double>*>::iterator itr1 = conversions.begin();
        int length = conversions.size();
        for(int i = 0; i < length; i++)
        {
            string key = itr1->first;
            cout << "( key: " << key << ", ";
            map<string, double>::iterator itr2 = conversions.at(key)->begin();
            int len = conversions[key]->size();
            for(int j = 0; j < len; j++)
            {
                cout << itr2->first << ": " << itr2->second << ", ";
                itr2++;
            }
            cout << ")" << endl;
            itr1++;
        }
    }

    const NumberWithUnits operator+(const NumberWithUnits& n)
    {
        return NumberWithUnits(n.amount,n.unit_type);
    }

    const NumberWithUnits operator-(const NumberWithUnits& n)
    {
        return NumberWithUnits(-n.amount, n.unit_type);
    }

    double tolorance = 0.00001;

    //All operators
    const NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& other)
    {
        return NumberWithUnits(amount + convert(other.unit_type, unit_type, other.amount), unit_type);
    }

    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other)
    {
        amount += convert(other.unit_type, unit_type, other.amount);
        return *this;
    }

    const NumberWithUnits NumberWithUnits::operator+()
    {
        return NumberWithUnits(amount, unit_type);
    }

    const NumberWithUnits operator+(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return NumberWithUnits(n1.amount + NumberWithUnits::convert(n2.unit_type, n1.unit_type, n2.amount) ,n1.unit_type);
    }

    const NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& other)
    {
        return NumberWithUnits(amount - convert(other.unit_type, unit_type, other.amount), unit_type);
    }

    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& other)
    {
        amount -= convert(other.unit_type, unit_type, other.amount);
        return *this;
    }

    const NumberWithUnits NumberWithUnits::operator-()
    {
        return NumberWithUnits(-amount, unit_type);
    }

    const NumberWithUnits operator-(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return NumberWithUnits(n1.amount - NumberWithUnits::convert(n2.unit_type, n1.unit_type, n2.amount) ,n1.unit_type);
    }

    bool operator>(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return n1.amount - NumberWithUnits::convert(n2.unit_type, n1.unit_type, n2.amount) > tolorance;
    }
    bool operator>=(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return n1.amount >= NumberWithUnits::convert(n2.unit_type, n1.unit_type, n2.amount);
    }
    bool operator<(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return n1.amount < NumberWithUnits::convert(n2.unit_type, n1.unit_type, n2.amount);
    }
    bool operator<=(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return n1.amount <= NumberWithUnits::convert(n2.unit_type, n1.unit_type, n2.amount);
    }
        bool operator==(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return abs(n1.amount - NumberWithUnits::convert(n2.unit_type, n1.unit_type, n2.amount)) <= tolorance;
    }
    bool operator!=(const NumberWithUnits& n1, const NumberWithUnits& n2)
    {
        return !(n1==n2);
    }

    NumberWithUnits& NumberWithUnits::operator++()
    {
        amount++;
        return *this;
    }
    const NumberWithUnits NumberWithUnits::operator++(int flag)
    {
        NumberWithUnits copy = *this;
        amount++;
        return copy;
    }
    NumberWithUnits& NumberWithUnits::operator--()
    {
        amount--;
        return *this;
    }
    const NumberWithUnits NumberWithUnits::operator--(int flag)
    {
        NumberWithUnits copy = *this;
        amount--;
        return copy;
    }

    const NumberWithUnits operator*(const double x, const NumberWithUnits& n)
    {
        return NumberWithUnits(x*n.amount, n.unit_type);
    }
    const NumberWithUnits operator*(const NumberWithUnits& n, const double x)
    {
        return NumberWithUnits(x*n.amount, n.unit_type);
    }

    std::ostream& operator<<(std::ostream& os, const NumberWithUnits& n)
    {
        return os << n.amount << '[' << n.unit_type << ']';
    }

    // Check that the input is in a correct format.
    static istream& getandcheckchar(istream& is, char expectedchar)
    {
        char actualchar;
        is >> actualchar;
        if(!is){return is;}
        if(actualchar != expectedchar)
        {
            is.setstate(ios::failbit);
        }
        return is;
    }

    std::istream& operator>>(std::istream& is, NumberWithUnits& n)
    {
        string new_unit = "";
        double new_amount = 0;
        ios::pos_type startpos = is.tellg();
        if(((!(is >> new_amount)) || (!getandcheckchar(is, '[')) || (!(is >> new_unit))))
        {
            auto errorstate = is.rdstate();
            is.clear();
            is.seekg(startpos);
            is.clear(errorstate);
        }
        else
        {
            unsigned long len = new_unit.length();
            if(len > 0 && new_unit[len - 1] == ']')
            {
                string temp = "";
                for(unsigned long i = 0; i < len -1; i++)
                {
                    temp+=new_unit[i];
                }
                new_unit = temp;
            }
            else if((!getandcheckchar(is, ']')))
            {
                auto errorstate = is.rdstate();
                is.clear();
                is.seekg(startpos);
                is.clear(errorstate);
            }
            NumberWithUnits::input_check(new_unit);
            n.amount = new_amount;
            n.unit_type = new_unit;
        }
        return is;
    }
}