#include <stdlib.h>
#include <stdio.h>
namespace radixDep
{
	/** 0-7*/
	bool getBitOfByte(unsigned char byte, unsigned char zeroTillSeventh)
	{
		if (zeroTillSeventh == 0)
		{
			return (bool)(1 & byte);
		}
		else if (zeroTillSeventh == 1)
		{
			return (bool)(2 & byte);
		}
		else if (zeroTillSeventh == 2)
		{
			return (bool)(4 & byte);
		}
		else if (zeroTillSeventh == 3)
		{
			return (bool)(8 & byte);
		}
		else if (zeroTillSeventh == 4)
		{
			return (bool)(16 & byte);
		}
		else if (zeroTillSeventh == 5)
		{
			return (bool)(32 & byte);
		}
		else if (zeroTillSeventh == 6)
		{
			return (bool)(64 & byte);
		}
		else if (zeroTillSeventh == 7)
		{
			return (bool)(128 & byte);
		}

		printf("ByteOverflow\n");
		return false;
	}

	void setBitOfByte(unsigned char* byte, unsigned char zeroTillSeventh, bool state)
	{
		if (state)
		{
			if (zeroTillSeventh == 0)
			{
				*byte = *byte | 1;
			}
			else if (zeroTillSeventh == 1)
			{
				*byte = *byte | 2;
			}
			else if (zeroTillSeventh == 2)
			{
				*byte = *byte | 4;
			}
			else if (zeroTillSeventh == 3)
			{
				*byte = *byte | 8;
			}
			else if (zeroTillSeventh == 4)
			{
				*byte = *byte | 16;
			}
			else if (zeroTillSeventh == 5)
			{
				*byte = *byte | 32;
			}
			else if (zeroTillSeventh == 6)
			{
				*byte = *byte | 64;
			}
			else if (zeroTillSeventh == 7)
			{
				*byte = *byte | 128;
			}
		}
		else // and not 1, 2, 4, 8, ...
		{
			if (zeroTillSeventh == 0)
			{
				*byte = *byte & 254;
			}
			else if (zeroTillSeventh == 1)
			{
				*byte = *byte & 253;
			}
			else if (zeroTillSeventh == 2)
			{
				*byte = *byte & 251;
			}
			else if (zeroTillSeventh == 3)
			{
				*byte = *byte & 247;
			}
			else if (zeroTillSeventh == 4)
			{
				*byte = *byte & 239;
			}
			else if (zeroTillSeventh == 5)
			{
				*byte = *byte & 223;
			}
			else if (zeroTillSeventh == 6)
			{
				*byte = *byte & 191;
			}
			else if (zeroTillSeventh == 7)
			{
				*byte = *byte & 127;
			}
		}
	}

	bool getBitOfBlock(void* block, size_t bitZeroTillN)
	{
		size_t byte = bitZeroTillN / 8;
		unsigned char bit = (unsigned char)(bitZeroTillN - byte * 8);
		return getBitOfByte(((unsigned char*)(block))[byte], bit);
	}

	void setBitOfBlock(void* block, size_t bitZeroTillN, bool state)
	{
		size_t byte = bitZeroTillN / 8;
		unsigned char bit = (unsigned char)(bitZeroTillN - byte * 8);
		setBitOfByte(((unsigned char*)(block)) + byte, bit, state);
	}



}


/** RadixSort unsafe implementation - you need to know the limits - call this crazySort :D
*   rsort works on original data.
*   Additional memory usage:   2*numElements*sizeElementInBytes
*   Memory including original: 3*numElements*sizeElementInBytes
*   In case "out of memory":   dataBlock is unchanged
*/
void rsort(void* dataBlock,const size_t numElements,const size_t sizeElementInBytes)
{
	unsigned char* subListZero = (unsigned char*)malloc(numElements * sizeElementInBytes);
	unsigned char* subListOne = (unsigned char*)malloc(numElements * sizeElementInBytes);
	if (subListZero && subListOne)
	{
		const size_t maxLevel = sizeElementInBytes * 8;
		// In C/C++ UNSIGNED types are listed like 001 == 4 DEZ 01 == 2  DEZ 1 == 1 DEZ
		size_t bitLevel = maxLevel-1;
		size_t indexZero = 0;
		size_t indexOne = 0;
		unsigned char* data = (unsigned char*)dataBlock;

		size_t workingBlockSize = numElements;
		size_t workingBlockEnd = numElements;

		//Reduce max level
		size_t tx = 0;
		while (bitLevel >= 0)
		{
			for (tx=0; tx < numElements; tx++)
			{
				//if first Bit of Level is set
				if (radixDep::getBitOfBlock((data + tx * sizeElementInBytes), bitLevel))
				{
					break;
				}
			}
			if (tx < numElements)
			{
				break;
			}
			else
			{
				bitLevel--;
			}
		}
		int dbg = 1;
		while (bitLevel >= 0)
		{

			for (size_t x = 0; x < workingBlockSize; x++)
			{

				//DEBUG
				//printf("(%d-", *((unsigned short*)(data + x * sizeElementInBytes)));
				//DEBUG

				//if first Bit of Level is set
				if (radixDep::getBitOfBlock((data + x * sizeElementInBytes), bitLevel))
				{
					//DEBUG
					//printf("1)");
					//DEBUG
					//Push in 1 list
					memcpy((subListOne + indexOne * sizeElementInBytes), (data + x * sizeElementInBytes), sizeElementInBytes);
					indexOne++;
				}
				else
				{
					//DEBUG
					//printf("0)");
					//DEBUG
					//Push in 0 list
					memcpy((subListZero + indexZero * sizeElementInBytes), (data + x * sizeElementInBytes), sizeElementInBytes);
					indexZero++;
				}
			}
			//Order data by sublists order.
			size_t idx = 0;
			
			while (indexZero > 0)
			{
				//Pop from 0 list
				indexZero--;
				//Overwrite original
				memcpy((data + idx * sizeElementInBytes), (subListZero + indexZero * sizeElementInBytes), sizeElementInBytes);
				idx++;
			}
			while (indexOne > 0)
			{
				//Pop from 1 list
				indexOne--;
				workingBlockSize--;
				//Overwrite original
				memcpy((data + idx * sizeElementInBytes), (subListOne + indexOne * sizeElementInBytes), sizeElementInBytes);
				idx++;
			}

			//Working on the fixed block
			if (workingBlockSize + 1 < workingBlockEnd && workingBlockSize > 0)
			{
				rsort((data + (workingBlockSize - 1) * sizeElementInBytes), workingBlockEnd - workingBlockSize, sizeElementInBytes);
				workingBlockEnd = workingBlockSize;
			}

			//DEBUG
			/*for (int i = 0; i < numElements; i++)
			{
				if (i > 0)
				{
					printf("_");
				}
				printf("%d", *((unsigned short*)(data + i * sizeElementInBytes)));
			}
			printf("\n");*/
			//DEBUG

			if (bitLevel == 0)
			{
				break;
			}

			bitLevel--;
		}

	}

	if (subListOne)
	{
		free(subListOne);
	}
	if (subListZero)
	{
		free(subListZero);
	}
}


