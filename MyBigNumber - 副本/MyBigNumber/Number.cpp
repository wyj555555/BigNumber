#include "Number.h"
#include <stdexcept>
#include <cstdlib>
#include <iostream>

Number::Number(size_t BitSize):NumberType(Undefine)
{
    this->BitSize = BitSize;
    // ����Data�����Ԫ�ظ���
    size_t NeedGroup = (BitSize + 7) / 8;

    // �����ڴ�
    Data = static_cast<unsigned char*>(malloc(NeedGroup));
    Invalid = static_cast<unsigned char*>(malloc(NeedGroup));

    // ����ڴ�����Ƿ�ɹ�
    if (Data == nullptr || Invalid == nullptr)
    {
        // �ͷ��ѷ�����ڴ�
        free(Data);
        free(Invalid);
        throw std::bad_alloc();
    }

    // ��ʼ��Data��Invalid
    std::fill_n(Data, NeedGroup, 0);
    std::fill_n(Invalid, NeedGroup, 0xFF);  // ����0xFF��ʾ��Ч
}

Number::Number(const Number& other) :NumberType(Undefine)
{
    BitSize = other.BitSize;  // ����λ��С
    size_t NeedGroup = (BitSize + 7) / 8;  // ������Ҫ���ֽ���

    // �����ڴ�����Data��Invalid
    Data = static_cast<unsigned char*>(malloc(NeedGroup));
    Invalid = static_cast<unsigned char*>(malloc(NeedGroup));

    // ����ڴ�����Ƿ�ɹ�
    if (Data == nullptr || Invalid == nullptr)
    {
        free(Data);  // �ͷ��ѷ�����ڴ�
        free(Invalid);
        throw std::bad_alloc();  // �׳��ڴ����ʧ���쳣
    }

    // ����other�����Data��Invalid���ݵ��¶���
    std::copy(other.Data, other.Data + NeedGroup, Data);
    std::copy(other.Invalid, other.Invalid + NeedGroup, Invalid);
}

Number& Number::operator=(const Number& other)
{
    if (this == &other)  // ����Ը�ֵ
    {
        return *this;
    }

    free(Data);  // �ͷŵ�ǰ�����Data�ڴ�
    free(Invalid);  // �ͷŵ�ǰ�����Invalid�ڴ�

    BitSize = other.BitSize;  // ����λ��С
    size_t NeedGroup = (BitSize + 7) / 8;  // ������Ҫ���ֽ���

    // �����ڴ�����Data��Invalid
    Data = static_cast<unsigned char*>(malloc(NeedGroup));
    Invalid = static_cast<unsigned char*>(malloc(NeedGroup));

    // ����ڴ�����Ƿ�ɹ�
    if (Data == nullptr || Invalid == nullptr)
    {
        free(Data);  // �ͷ��ѷ�����ڴ�
        free(Invalid);
        throw std::bad_alloc();  // �׳��ڴ����ʧ���쳣
    }

    // ����other�����Data��Invalid���ݵ���ǰ����
    std::copy(other.Data, other.Data + NeedGroup, Data);
    std::copy(other.Invalid, other.Invalid + NeedGroup, Invalid);

    return *this;  // ���ص�ǰ������֧����ʽ��ֵ
}

void Number::SetBit(size_t BitIndex)
{
    // ��� BitIndex �Ƿ񳬳���Χ
    if (BitIndex >= BitSize)
    {
        throw std::out_of_range("Bit index out of range");
    }

    size_t GroupSize = (BitSize + 7) / 8 - 1;

    // �����ֽ�������λλ��
    size_t operatorgroup = (BitIndex / 8); // �ֽ����������ҵ���
    size_t bitPosition = BitIndex % 8; // λλ�ã����ҵ���

    // �����ض�λ
    Data[GroupSize-operatorgroup] |= (1 << bitPosition);
}

void Number::ClearBit(size_t BitIndex)
{
    // ��� BitIndex �Ƿ񳬳���Χ
    if (BitIndex >= BitSize)
    {
        throw std::out_of_range("Bit index out of range");
    }

    size_t GroupSize = (BitSize + 7) / 8 - 1;

    // �����ֽ�������λλ��
    size_t operatorgroup = (BitIndex / 8); // �ֽ����������ҵ���
    size_t bitPosition = BitIndex % 8; // λλ�ã����ҵ���

    // ����ض�λ
    Data[GroupSize - operatorgroup] &= ~(1 << bitPosition);
}

void Number::ToggleBit(size_t BitIndex)
{
    // ��� BitIndex �Ƿ񳬳���Χ
    if (BitIndex >= BitSize)
    {
        throw std::out_of_range("Bit index out of range");
    }

    size_t GroupSize = (BitSize + 7) / 8 - 1;

    // �����ֽ�������λλ��
    size_t operatorgroup = (BitIndex / 8); // �ֽ����������ҵ���
    size_t bitPosition = BitIndex % 8; // λλ�ã����ҵ���

    // �л��ض�λ
    Data[GroupSize - operatorgroup] ^= (1 << bitPosition);
}

size_t Number::GetBitSize()const
{
    return BitSize;
}

int Number::GetBit(size_t BitIndex) const
{
    if (BitIndex >= BitSize) throw std::out_of_range("BitIndex out of range");
    size_t GroupSize = (BitSize + 7) / 8 - 1;

    // �����ֽ�������λλ��
    size_t operatorgroup = (BitIndex / 8); // �ֽ����������ҵ���
    size_t bitPosition = BitIndex % 8; // λλ�ã����ҵ���

    return (Data[GroupSize - operatorgroup] & (1 << bitPosition)) != 0;
}

void Number::ToNegative()
{
    if (NumberType == Type::Integer)
    {
        // ������ʹ�ò����ʾ����
        // Invert all bits
        for (size_t i = 0; i < BitSize; ++i)
        {
            ToggleBit(i);
        }

        // Add 1 to the least significant bit
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
    else if (NumberType == Type::FloatIngpoint)
    {
        // ��������ȡ������λ
        InvertSignBit();
    }
    else
    {
        throw std::runtime_error("Undefined number type");
    }
}

const unsigned char* Number::GetData()const
{
    return Data;
}

Number::Type Number::GetType()
{
    return NumberType;
}

void Number::InvertSignBit()
{
    // ���踡�����ķ���λ�����λ
    ToggleBit(BitSize - 1);
}

//void Number::add(const Number& other, bool subtract)
//{
//    if (BitSize != other.BitSize)
//    {
//        throw std::invalid_argument("Bit sizes do not match");
//    }
//
//    size_t NeedGroup = (BitSize + 7) / 8;
//    unsigned carry = subtract ? 1 : 0;  // If subtracting, start with carry as 1 to handle two's complement
//    unsigned char* tempData = static_cast<unsigned char*>(malloc(NeedGroup));
//
//    if (tempData == nullptr)
//    {
//        throw std::bad_alloc();
//    }
//
//    for (size_t i = 0; i < NeedGroup; ++i)
//    {
//        unsigned temp1 = Data[i];
//        unsigned temp2 = other.Data[i];
//
//        if (subtract)
//        {
//            temp2 = ~temp2;
//        }
//
//        unsigned sum = temp1 + temp2 + carry;
//        carry = (sum > 0xFF) ? 1 : 0;
//        tempData[i] = sum & 0xFF;
//    }
//
//    std::copy(tempData, tempData + NeedGroup, Data);
//    free(tempData);
//
//    if (subtract)
//    {
//        bool carry = true;
//        for (size_t i = 0; i < BitSize && carry; ++i)
//        {
//            if (GetBit(i) == 0)
//            {
//                SetBit(i);
//                carry = false;
//            }
//            else
//            {
//                ClearBit(i);
//            }
//        }
//    }
//}
//
//void Number::multiply(const Number& other)
//{
//    if (BitSize != other.BitSize)
//    {
//        throw std::invalid_argument("Bit sizes do not match");
//    }
//
//    Number result(BitSize);
//    Number temp(*this);
//
//    for (size_t i = 0; i < BitSize; ++i)
//    {
//        if (other.GetBit(i))
//        {
//            result += temp;
//        }
//        temp <<= 1;
//    }
//
//    std::copy(result.Data, result.Data + (BitSize + 7) / 8, Data);
//}
//
//void Number::divide(const Number& other)
//{
//    if (BitSize != other.BitSize)
//    {
//        throw std::invalid_argument("Bit sizes do not match");
//    }
//
//    Number dividend(*this);
//    Number divisor(other);
//    Number quotient(BitSize);
//    Number one(BitSize);
//
//    one.SetBit(0);
//
//    for (size_t i = BitSize; i > 0; --i)
//    {
//        quotient <<= 1;
//        if (dividend >= divisor)
//        {
//            dividend -= divisor;
//            quotient |= one;
//        }
//        divisor >>= 1;
//    }
//
//    std::copy(quotient.Data, quotient.Data + (BitSize + 7) / 8, Data);
//}

Number& Number::operator&=(const Number& other)
{
    if (BitSize != other.BitSize)
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    size_t NeedGroup = (BitSize + 7) / 8;
    for (size_t i = 0; i < NeedGroup; ++i)
    {
        Data[i] &= other.Data[i];
    }

    return *this;
}

Number& Number::operator|=(const Number& other)
{
    if (BitSize != other.BitSize)
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    size_t NeedGroup = (BitSize + 7) / 8;
    for (size_t i = 0; i < NeedGroup; ++i)
    {
        Data[i] |= other.Data[i];
    }

    return *this;
}

Number& Number::operator^=(const Number& other)
{
    if (BitSize != other.BitSize)
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    size_t NeedGroup = (BitSize + 7) / 8;
    for (size_t i = 0; i < NeedGroup; ++i)
    {
        Data[i] ^= other.Data[i];
    }

    return *this;
}

Number& Number::operator<<=(size_t shift)
{
    if (shift >= BitSize)
    {
        std::fill_n(Data, (BitSize + 7) / 8, 0);
        return *this;
    }

    size_t byteShift = shift / 8;
    size_t bitShift = shift % 8;

    if (bitShift == 0)
    {
        std::memmove(Data, Data + byteShift, (BitSize + 7) / 8 - byteShift);
        std::fill_n(Data + (BitSize + 7) / 8 - byteShift, byteShift, 0);
    }
    else
    {
        for (size_t i = (BitSize + 7) / 8; i > byteShift; --i)
        {
            Data[i - 1] = (Data[i - 1 - byteShift] << bitShift) |
                (Data[i - 2 - byteShift] >> (8 - bitShift));
        }
        Data[byteShift] = Data[0] << bitShift;
        std::fill_n(Data, byteShift, 0);
    }

    return *this;
}

Number& Number::operator>>=(size_t shift)
{
    if (shift >= BitSize)
    {
        std::fill_n(Data, (BitSize + 7) / 8, 0);
        return *this;
    }

    size_t byteShift = shift / 8;
    size_t bitShift = shift % 8;

    if (bitShift == 0)
    {
        std::memmove(Data, Data + byteShift, (BitSize + 7) / 8 - byteShift);
        std::fill_n(Data + (BitSize + 7) / 8 - byteShift, byteShift, 0);
    }
    else
    {
        for (size_t i = 0; i < (BitSize + 7) / 8 - byteShift - 1; ++i)
        {
            Data[i] = (Data[i + byteShift] >> bitShift) |
                (Data[i + byteShift + 1] << (8 - bitShift));
        }
        Data[(BitSize + 7) / 8 - byteShift - 1] = Data[(BitSize + 7) / 8 - 1] >> bitShift;
        std::fill_n(Data + (BitSize + 7) / 8 - byteShift, byteShift, 0);
    }

    return *this;
}

Number Number::operator~() const
{
    Number result(BitSize);
    size_t NeedGroup = (BitSize + 7) / 8;
    for (size_t i = 0; i < NeedGroup; ++i)
    {
        result.Data[i] = ~Data[i];
    }
    return result;
}

Number Number::operator<<(size_t shift) const
{
    Number result = *this;
    result <<= shift;
    return result;
}

Number Number::operator>>(size_t shift) const
{
    Number result = *this;
    result >>= shift;
    return result;
}

Number::~Number()
{
    if (Data != nullptr) {
        std::free(Data);
        Data = nullptr;
    }

    if (Invalid != nullptr) {
        std::free(Invalid);
        Invalid = nullptr;
    }
}

Number& Number::operator+=(const Number& other)
{
    add(other, false);
    return *this;
}

Number& Number::operator-=(const Number& other)
{
    add(other, true);
    return *this;
}

Number& Number::operator*=(const Number& other)
{
    multiply(other);
    return *this;
}

Number& Number::operator/=(const Number& other)
{
    divide(other);
    return *this;
}

Number Number::operator+(const Number& other) const
{
    Number result(*this);
    result += other;
    return result;
}

Number Number::operator-(const Number& other) const
{
    Number result(*this);
    result -= other;
    return result;
}

Number Number::operator*(const Number& other) const
{
    Number result(*this);
    result *= other;
    return result;
}

Number Number::operator/(const Number& other) const
{
    Number result(*this);
    result /= other;
    return result;
}

// Helper function to check if a number is negative
bool isNegative(const Number& num)
{
    return num.GetBit(num.GetBitSize() - 1) == 1;
}

bool Number::operator>=(const Number& other) const
{
    if (GetBitSize() != other.GetBitSize())
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    bool thisNegative = isNegative(*this);
    bool otherNegative = isNegative(other);

    if (thisNegative != otherNegative)
    {
        return otherNegative;
    }

    for (size_t i = GetBitSize(); i > 0; --i)
    {
        size_t index = i - 1;
        if (GetBit(index) != other.GetBit(index))
        {
            if (thisNegative)
            {
                return GetBit(index) < other.GetBit(index);
            }
            else
            {
                return GetBit(index) > other.GetBit(index);
            }
        }
    }

    return true;
}

bool Number::operator<=(const Number& other) const
{
    if (GetBitSize() != other.GetBitSize())
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    bool thisNegative = isNegative(*this);
    bool otherNegative = isNegative(other);

    if (thisNegative != otherNegative)
    {
        return thisNegative;
    }

    for (size_t i = GetBitSize(); i > 0; --i)
    {
        size_t index = i - 1;
        if (GetBit(index) != other.GetBit(index))
        {
            if (thisNegative)
            {
                return GetBit(index) > other.GetBit(index);
            }
            else
            {
                return GetBit(index) < other.GetBit(index);
            }
        }
    }

    return true;
}

bool Number::operator>(const Number& other) const
{
    if (GetBitSize() != other.GetBitSize())
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    bool thisNegative = isNegative(*this);
    bool otherNegative = isNegative(other);

    if (thisNegative != otherNegative)
    {
        return otherNegative;
    }

    for (size_t i = GetBitSize(); i > 0; --i)
    {
        size_t index = i - 1;
        if (GetBit(index) != other.GetBit(index))
        {
            if (thisNegative)
            {
                return GetBit(index) < other.GetBit(index);
            }
            else
            {
                return GetBit(index) > other.GetBit(index);
            }
        }
    }

    return false;
}

bool Number::operator<(const Number& other) const
{
    if (GetBitSize() != other.GetBitSize())
    {
        throw std::invalid_argument("Bit sizes do not match");
    }

    bool thisNegative = isNegative(*this);
    bool otherNegative = isNegative(other);

    if (thisNegative != otherNegative)
    {
        return thisNegative;
    }

    for (size_t i = GetBitSize(); i > 0; --i)
    {
        size_t index = i - 1;
        if (GetBit(index) != other.GetBit(index))
        {
            if (thisNegative)
            {
                return GetBit(index) > other.GetBit(index);
            }
            else
            {
                return GetBit(index) < other.GetBit(index);
            }
        }
    }

    return false;
}

void Number::checkBitIndex(size_t BitIndex) const
{
    if (BitIndex >= BitSize) throw std::out_of_range("BitIndex out of range");
}