#include "pch.h"
#include "CppUnitTest.h"
#include "../Project1/Project1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			fstream f("D:\\Patiuk\\AP\\lab_2\\labs11\\lab11.3_B\\Project1\\unittest.dat", ios::binary | ios::in | ios::out);
			int mathMark = fMark(f, 0);
			Assert::AreEqual(mathMark, 3);
		}
	};
}
