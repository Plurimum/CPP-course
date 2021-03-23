#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <iomanip>
#include <math.h>
#include <fstream>

class BigInteger
{
    static const int BASE = 1000 * 1000 * 1000;
    std::vector<int> digits;
    bool isNegative;
    bool isNaN;

    void excludeLeadingZeros();

    void shiftToRight();

public:

    BigInteger();

    BigInteger(std::string);

    BigInteger(signed int);

    ~BigInteger();

    friend std::ostream& operator<<(std::ostream&, const BigInteger&);

    friend std::istream& operator>>(std::istream&, BigInteger&);

    const BigInteger operator-() const;

    friend bool operator==(const BigInteger&, const BigInteger&);

    friend bool operator<(const BigInteger&, const BigInteger&);

    friend bool operator!=(const BigInteger&, const BigInteger&);

    friend bool operator<=(const BigInteger&, const BigInteger&);

    friend bool operator>(const BigInteger&, const BigInteger&);

    friend bool operator>=(const BigInteger&, const BigInteger&);

    friend const BigInteger operator+(BigInteger, const BigInteger&);

    BigInteger& operator+=(const BigInteger&);

    friend const BigInteger operator-(BigInteger, const BigInteger&);

    BigInteger& operator-=(const BigInteger&);

    friend const BigInteger operator*(const BigInteger&, const BigInteger&);

    friend const BigInteger operator/(const BigInteger&, const BigInteger&);

    friend const BigInteger operator%(const BigInteger& left, const BigInteger& right);

    const BigInteger sqrt();
};

BigInteger::BigInteger()
{
    this->isNaN = false;
    this->isNegative = false;
}

BigInteger::BigInteger(std::string str)
{
    this->isNaN = false;
    if (str[0] != '-')
    {
        this->isNegative = false;
    } else
    {
        str = str.substr(1);
        this->isNegative = true;
    }
    for (int i = str.length(); i > 0; i -= 9)
    {
        int result = 0;
        int pow = 1;
        if (i < 9)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                result += (str[j] - '0') * pow;
                pow *= 10;
            }
        } else
        {
            for (int j = i - 1; j >= i - 9; j--)
            {
                result += (str[j] - '0') * pow;
                pow *= 10;
            }
        }
        this->digits.push_back(result);
    }
    this->excludeLeadingZeros();
}

void BigInteger::excludeLeadingZeros()
{
    while (this->digits.size() > 1 && this->digits.back() == 0)
    {
        this->digits.pop_back();
    }
    if (this->digits.size() == 1 && this->digits[0] == 0)
    {
        this->isNegative = false;
    }
}

BigInteger::~BigInteger()
{
    digits.clear();
}

std::ostream& operator<<(std::ostream& out, const BigInteger& argument)
{
    if (argument.isNaN)
    {
        out << "NaN";
        return out;
    }
    if (argument.digits.empty())
    {
        out << 0;
    } else
    {
        if (argument.isNegative)
        {
            out << '-';
        }
        out << argument.digits.back();
        char old_fill = out.fill('0');
        for (int i = static_cast<int>(argument.digits.size()) - 2; i >= 0; --i)
        {
            out << std::setw(9) << argument.digits[i];
        }
        out.fill(old_fill);
    }
    return out;
}

std::istream& operator>>(std::istream& in, BigInteger& argument)
{
    std::string str;
    in >> str;
    argument = BigInteger(str);
    return in;
}

bool operator==(const BigInteger& left, const BigInteger& right)
{
    if (left.isNegative != right.isNegative)
    {
        return false;
    }
    if (left.digits.size() != right.digits.size())
    {
        return false;
    }
    for (size_t i = 0; i < left.digits.size(); ++i)
    {
        if (left.digits[i] != right.digits[i])
        {
            return false;
        }
    }
    return true;
}

bool operator<(const BigInteger& left, const BigInteger& right)
{
    if (left == right)
    {
        return false;
    }
    if (left.isNegative)
    {
        if (right.isNegative)
        {
            return ((-right) < (-left));
        } else
        {
            return true;
        }
    } else
    {
        if (right.isNegative)
        {
            return false;
        } else
        {
            if (left.digits.size() != right.digits.size())
            {
                return left.digits.size() < right.digits.size();
            } else
            {
                for (long long i = left.digits.size() - 1; i >= 0; i--)
                {
                    if (left.digits[i] != right.digits[i])
                    {
                        return left.digits[i] < right.digits[i];
                    }
                }
                return false;
            }
        }
    }
}

const BigInteger BigInteger::operator-() const
{
    BigInteger flipped(*this);
    flipped.isNegative = !flipped.isNegative;
    return flipped;
}

bool operator<=(const BigInteger& left, const BigInteger& right)
{
    return (!(left != right && left >= right));
}

bool operator>(const BigInteger& left, const BigInteger& right)
{
    return !(left <= right);
}

bool operator>=(const BigInteger& left, const BigInteger& right)
{
    return !(left < right);
}

bool operator!=(const BigInteger& left, const BigInteger& right)
{
    return !(left == right);
}

const BigInteger operator+(BigInteger left, const BigInteger& right)
{
    if (left.isNegative)
    {
        if (right.isNegative)
        {
            return -(-left + (-right));
        } else
        {
            return right - (-left);
        }
    } else
    {
        if (right.isNegative)
        {
            return left - (-right);
        }
    }
    int delta = 0;
    for (size_t i = 0; (i < std::max(left.digits.size(), right.digits.size())) || (delta != 0); i++)
    {

        int sum = 0;
        if (i == left.digits.size())
        {
            left.digits.push_back(0);
        }
        if (i < right.digits.size())
        {
            sum = (left.digits[i] + delta + right.digits[i]);
        } else
        {
            sum = (left.digits[i] + delta);
        }
        delta = sum / BigInteger::BASE;
        if (delta != 0)
        {
            left.digits[i] = sum % BigInteger::BASE;
        } else {
            left.digits[i] = sum;
        }
    }
    return left;
}

const BigInteger operator-(BigInteger left, const BigInteger& right)
{
    if (right.isNegative)
    {
        return left + (-right);
    } else
    {
        if (left.isNegative)
        {
            return -(-left + right);
        } else
        {
            if (left < right)
            {
                return -(right - left);
            }
        }
    }
    int delta = 0;
    for (size_t i = 0; i < right.digits.size() || delta != 0; ++i)
    {
        int difference;
        int leftGroup = left.digits[i];
        if (i < right.digits.size())
        {
            int rightGroup = right.digits[i];
            difference = leftGroup - delta - rightGroup;
        } else
        {
            difference = leftGroup - delta;
        }
        if (difference < 0)
        {
            left.digits[i] = difference + BigInteger::BASE;
            delta = 1;
        } else
        {
            left.digits[i] = difference;
            delta = 0;
        }
    }

    left.excludeLeadingZeros();
    return left;
}

BigInteger& BigInteger::operator+=(const BigInteger& value)
{
    return *this = (*this + value);
}

BigInteger& BigInteger::operator-=(const BigInteger& value)
{
    return *this = (*this - value);
}

const BigInteger operator*(const BigInteger& left, const BigInteger& right)
{
    BigInteger result;
    result.isNegative = left.isNegative != right.isNegative;
    result.digits.resize(std::max(left.digits.size(), right.digits.size()) * 2);
    for (size_t i = 0; i < left.digits.size(); i++)
    {
        int delta = 0;
        for (size_t j = 0; j < right.digits.size() || delta != 0; j++)
        {
            long long product;
            if (j < right.digits.size())
            {
                product = result.digits[i + j] + 1LL * left.digits[i] * right.digits[j] + delta;
            } else
            {
                product = result.digits[i + j] + delta;
            }
            result.digits[i + j] = static_cast<int>(product % BigInteger::BASE);
            delta = static_cast<int>(product / BigInteger::BASE);
        }
    }
    result.excludeLeadingZeros();
    return result;
}

void BigInteger::shiftToRight()
{
    if (this->digits.empty())
    {
        this->digits.push_back(0);
        return;
    }
    this->digits.push_back(this->digits[this->digits.size() - 1]);
    for (size_t i = this->digits.size() - 2; i > 0; --i)
    {
        this->digits[i] = this->digits[i - 1];
    }
    this->digits[0] = 0;
}

BigInteger::BigInteger(signed int i)
{
    this->isNegative = i < 0;
    this->digits.push_back(std::abs(i) % BigInteger::BASE);
    i /= BigInteger::BASE;
    if (i != 0) {
        this->digits.push_back(std::abs(i));
    }
}

const BigInteger operator/(const BigInteger& left, const BigInteger& right)
{
    if (right.digits[right.digits.size() - 1] == 0)
    {
        BigInteger NaN;
        NaN.isNaN = true;
        return NaN;
    }
    BigInteger result, current, divisor = right;
    divisor.isNegative = false;
    result.digits.resize(left.digits.size());
    for (int i = left.digits.size() - 1; i >= 0; --i)
    {
        current.shiftToRight();
        current.digits[0] = left.digits[i];
        current.excludeLeadingZeros();
        int quotient = 0, leftBorder = 0, rightBorder = BigInteger::BASE;
        while (leftBorder <= rightBorder)
        {
            int middle = (leftBorder + rightBorder) / 2;
            BigInteger product = divisor * middle;
            if (product <= current)
            {
                quotient = middle;
                leftBorder = middle + 1;
            } else {
                rightBorder = middle - 1;
            }
        }
        result.digits[i] = quotient;
        current -= divisor * quotient;
    }
    result.isNegative = left.isNegative != right.isNegative;
    result.excludeLeadingZeros();
    return result;
}

const BigInteger operator%(const BigInteger& left, const BigInteger& right)
{
    if (right.digits[right.digits.size() - 1] == 0)
    {
        BigInteger NaN;
        NaN.isNaN = true;
        return NaN;
    }
    BigInteger result = left - (left / right) * right;
    if (result.isNegative)
    {
        result += right;
    }
    return result;
}

const BigInteger BigInteger::sqrt()
{
    if (this->isNegative)
    {
        BigInteger NaN;
        NaN.isNaN = true;
        return NaN;
    }
    BigInteger number = *this;
    if (number == 1)
    {
        return BigInteger("1");
    }
    BigInteger leftBorder, rightBorder, current;
    leftBorder = BigInteger("0");
    rightBorder = *this;
    while (leftBorder < rightBorder - 1)
    {
        current = (leftBorder + rightBorder) / 2;
        if (current * current <= number)
        {
            leftBorder = current;
        } else
        {
            rightBorder = current;
        }
    }
    return leftBorder;
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        printf("You should enter three arguments to program:\nlab3 <input file name> <output file name>");
        exit(EXIT_FAILURE);
    }
    std::ifstream fin(argv[2]);
    if (!fin)
    {
        perror("Can't open input file");
        exit(errno);
    }
    std::ofstream fout(argv[3]);
    if (!fout)
    {
        perror("Can't open output file");
        exit(errno);
    }
    std::string operations = " + - * / % < <= > >= == != # ";
    BigInteger firstBigInt, secondBigInt;
    std::string operation;
    fin >> firstBigInt >> operation;
    operation = " " + operation + " ";
    int mode = operations.find(operation);
    if (mode == 26)
    {
        fout << firstBigInt.sqrt();
    } else
    {
        fin >> secondBigInt;
        switch (mode)
        {
            case 0:
            {
                fout << firstBigInt + secondBigInt;
                break;
            }
            case 2:
            {
                fout << firstBigInt - secondBigInt;
                break;
            }
            case 4:
            {
                fout << firstBigInt * secondBigInt;
                break;
            }
            case 6:
            {
                fout << firstBigInt / secondBigInt;
                break;
            }
            case 8:
            {
                fout << firstBigInt % secondBigInt;
                break;
            }
            case 10:
            {
                fout << (firstBigInt < secondBigInt);
                break;
            }
            case 12:
            {
                fout << (firstBigInt <= secondBigInt);
                break;
            }
            case 15:
            {
                fout << (firstBigInt > secondBigInt);
                break;
            }
            case 17:
            {
                fout << (firstBigInt >= secondBigInt);
                break;
            }
            case 20:
            {
                fout << (firstBigInt == secondBigInt);
                break;
            }
            case 23:
            {
                fout << (firstBigInt != secondBigInt);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    fin.close();
    fout.close();
    return 0;
}
