#include "BigInt.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

BigInt::BigInt(const char* num, size_t BitSize):Number(BitSize)
{
    this->NumberType = Type::Integer;
    StringToBinary(num);
}

BigInt::BigInt(const char* num, size_t BitSize, int radix) :Number(BitSize)
{

}

BigInt::BigInt(int num, size_t BitSize) : Number(BitSize)
{
    std::string numStr = std::to_string(num);
    StringToBinary(numStr);
}

void BigInt::StringToBinary(const std::string& numberStr)
{
    bool isNegative = (numberStr[0] == '-');
    size_t startIndex = isNegative ? 1 : 0;
    std::string numStr = numberStr.substr(startIndex);

    // Clear existing bits
    for (size_t i = 0; i < BitSize; ++i)
    {
        ClearBit(i);
    }

    // Convert the decimal number to binary
    std::vector<bool> binaryRepresentation;

    while (!numStr.empty())
    {
        int remainder = 0;
        std::string newNumStr;

        for (char c : numStr)
        {
            int currentDigit = remainder * 10 + (c - '0');
            remainder = currentDigit % 2;
            if (!(newNumStr.empty() && currentDigit / 2 == 0))
            {
                newNumStr.push_back(currentDigit / 2 + '0');
            }
        }

        binaryRepresentation.push_back(remainder);
        numStr = newNumStr;
    }

    // Store the binary representation into the Data array
    for (size_t i = 0; i < binaryRepresentation.size() && i < BitSize; ++i)
    {
        if (binaryRepresentation[i])
        {
            SetBit(i);
        }
    }

    // If the number is negative, convert to two's complement
    if (isNegative)
    {
        ToNegative();
    }
}

std::string addStrings(const std::string& num1, const std::string& num2)
{
    int carry = 0;
    int maxLength = std::max(num1.size(), num2.size());
    std::string result;
    for (int i = 0; i < maxLength; ++i)
    {
        int digit1 = i < num1.size() ? num1[num1.size() - 1 - i] - '0' : 0;
        int digit2 = i < num2.size() ? num2[num2.size() - 1 - i] - '0' : 0;
        int sum = digit1 + digit2 + carry;
        result.push_back(sum % 10 + '0');
        carry = sum / 10;
    }
    if (carry > 0)
    {
        result.push_back(carry + '0');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string BigInt::ToString() const
{
    // 检查是否为负数
    bool isNegative = (this->GetBit(BitSize - 1) == 1);

    // 创建一个临时对象来处理补码转换（避免修改原始数据）
    BigInt temp = *this;
    if (isNegative)
    {
        temp.ToNegative(); // 将负数转换为正数（补码转换）
    }

    std::string result = "0";
    std::string base = "1";

    for (size_t i = 0; i < temp.BitSize; ++i)
    {
        if (temp.GetBit(i))
        {
            result = addStrings(result, base);
        }
        base = addStrings(base, base); // base *= 2;
    }

    if (isNegative)
    {
        result = "-" + result;
    }

    return result;
}

BigInt& BigInt::operator+=(const Number& other)
{
    this->add(other, false);
    return *this;
}

BigInt BigInt::operator+(const Number& other) const
{
    BigInt result = *this;
    result.add(other, false);
    return result;
}

BigInt& BigInt::operator+=(int value)
{
    BigInt temp(value, this->BitSize);
    this->add(temp, false);
    return *this;
}

BigInt BigInt::operator+(int value) const
{
    BigInt result = *this;
    BigInt temp(value, this->BitSize);
    result.add(temp, false);
    return result;
}

void BigInt::add(const Number& other, bool subtract)
{
    if (BitSize != other.GetBitSize())
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    size_t NeedGroup = (BitSize + 7) / 8;
    unsigned carry = subtract ? 1 : 0;  // If subtracting, start with carry as 1 to handle two's complement
    unsigned char* tempData = static_cast<unsigned char*>(malloc(NeedGroup));

    if (tempData == nullptr)
    {
        throw std::bad_alloc();
    }

    for (size_t i = 0; i < NeedGroup; ++i)
    {
        unsigned temp1 = Data[i];
        unsigned temp2 = other.GetData()[i];

        if (subtract)
        {
            temp2 = ~temp2;
        }

        unsigned sum = temp1 + temp2 + carry;
        carry = (sum > 0xFF) ? 1 : 0;
        tempData[i] = sum & 0xFF;
    }

    std::copy(tempData, tempData + NeedGroup, Data);
    free(tempData);

    if (subtract)
    {
        bool carry = true;
        for (size_t i = 0; i < BitSize && carry; ++i)
        {
            if (GetBit(i) == 0)
            {
                SetBit(i);
                carry = false;
            }
            else
            {
                ClearBit(i);
            }
        }
    }
}

void BigInt::multiply(const Number& other)
{
    if (BitSize != other.GetBitSize())
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    Number result(BitSize);
    Number temp(*this);

    for (size_t i = 0; i < BitSize; ++i)
    {
        if (other.GetBit(i))
        {
            result += temp;
        }
        temp <<= 1;
    }

    std::copy(result.GetData(), result.GetData() + (BitSize + 7) / 8, Data);
}

void BigInt::divide(const Number& other)
{
    if (BitSize != other.GetBitSize())
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    Number dividend(*this);
    Number divisor(other);
    Number quotient(BitSize);
    Number one(BitSize);

    one.SetBit(0);

    for (size_t i = BitSize; i > 0; --i)
    {
        quotient <<= 1;
        if (dividend >= divisor)
        {
            dividend -= divisor;
            quotient |= one;
        }
        divisor >>= 1;
    }

    std::copy(quotient.GetData(), quotient.GetData() + (BitSize + 7) / 8, Data);
}