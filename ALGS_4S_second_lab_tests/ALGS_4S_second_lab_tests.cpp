#include "pch.h"
#include "CppUnitTest.h"
#include "../ALGS_4S_second_lab/ALGS_4S_second_lab.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ALGS4Ssecondlabtests
{
	TEST_CLASS(ALGS4Ssecondlabtests)
	{
	public:
		
		inline string const BoolToString(bool b)
		{
			return b ? "1" : "0";
		}
		TEST_METHOD(AlgorithmGeneralTest)
		{
			string quote = "Lets have some unit tests!";

			Map<char, int> symbolsMap;
			for (char character : quote)
				symbolsMap.Add(character, 1);

			HuffmanTree huffmantree;
			huffmantree = buildHuffmanTree(symbolsMap);

			Map<char, List<bool>*> haffmanCode = CreateHuffmanCode(huffmantree);

			List<bool> encodedLine = EncodeByHuffman(quote, haffmanCode);

			string EncodedString = bitSequanceToString(encodedLine);
			string ExpectedCode = "00001111011101000001011100011111100110010000101111000111101100010110110010111111010111001101";
			Assert::AreEqual(ExpectedCode, EncodedString);
		}
		TEST_METHOD(EncodeTableTest)
		{

		}
	};
}
