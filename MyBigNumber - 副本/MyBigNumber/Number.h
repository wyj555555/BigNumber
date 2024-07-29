#pragma once

#define SIZE_8BIT   8
#define SIZE_16BIT  16
#define SIZE_32BIT  32
#define SIZE_64BIT  64
#define SIZE_128BIT 128
#define SIZE_256BIT 256
#define SIZE_512BIT 512
#define SIZE_1024BIT 1024

class Number
{
public:
	enum Type
	{
		Integer,
		FloatIngpoint,
		Undefine
	};

	Number(size_t BitSize);
	Number(const Number& other); // 拷贝构造函数声明

	void SetBit(size_t BitIndex);
	void ClearBit(size_t BitIndex);
	void ToggleBit(size_t BitIndex);
	size_t GetBitSize()const;
	int GetBit(size_t BitIndex) const;
	void ToNegative();
	const unsigned char* GetData()const;
	Type GetType();

	Number& operator&=(const Number& other);
	Number& operator|=(const Number& other);
	Number& operator^=(const Number& other);
	Number& operator<<=(size_t shift);
	Number& operator>>=(size_t shift);
	Number operator~() const;
	Number operator<<(size_t shift) const;
	Number operator>>(size_t shift) const;
	Number& operator+=(const Number& other);
	Number& operator-=(const Number& other);
	Number& operator*=(const Number& other);
	Number& operator/=(const Number& other);
	Number operator+(const Number& other) const;
	Number operator-(const Number& other) const;
	Number operator*(const Number& other) const;
	Number operator/(const Number& other) const;

	// 运算符重载函数声明
	bool operator>=(const Number& other) const;
	bool operator<=(const Number& other) const;
	bool operator>(const Number& other) const;
	bool operator<(const Number& other) const;

	Number& operator=(const Number& other);  // 赋值操作符声明
	
	virtual ~Number();
protected:
	Type NumberType;
	size_t BitSize;
	unsigned char* Data;
	unsigned char* Invalid;

	void InvertSignBit();
	void checkBitIndex(size_t BitIndex) const;
	virtual void add(const Number& other, bool subtract) {};
	virtual void multiply(const Number& other) {};
	virtual void divide(const Number& other) {};
};

