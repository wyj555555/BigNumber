#pragma once
#include "Number.h"
#include <iostream>
#include <string>

class BigInt :public Number
{
public:
	BigInt(const char* num,size_t BitSize);
	BigInt(const char* num, size_t BitSize,int radix);
	BigInt(int num, size_t BitSize);

	std::string ToString() const;

	// ‘ÀÀ„∑˚÷ÿ‘ÿ
	BigInt& operator+=(const Number& other);
	BigInt operator+(const Number& other) const;
	BigInt& operator+=(int value);
	BigInt operator+(int value) const;


	virtual ~BigInt() override = default;

private:
	void StringToBinary(const std::string& numberStr);

protected:
	void add(const Number& other, bool subtract);
	void multiply(const Number& other);
	void divide(const Number& other);
};