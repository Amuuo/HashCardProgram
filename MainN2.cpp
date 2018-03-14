#include<iostream>
#include<string>
#include<stdio.h>
#include<array>
#include<fstream>
#include<vector>
#include<iomanip>
#include<cctype>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

using namespace std;

const int TABLE_SIZE = 20;

class Card
{
private:
	string  face;
	string  suit;
	Card*   nextCard;
	Card*   prevCard;

public:
	Card();
	Card(string(face), string(suit));
	Card(Card&);
	Card(Card*);
	~Card();

	bool        operator==(Card*);
	Card*       operator=(Card*);
	Card        operator=(Card&);
	Card*       getNextPtr() const;
	Card*       getPrevPtr() const;
	string      getFace() const;
	string      getSuit() const;
	void        printCard() const;
	void        setFace(string);
	void        setSuit(string);
	void        setNextPtr(Card*);
	void        setPrevPtr(Card*);
	void        setPrevPtr(int);
	void        eraseCard();
	int         getKey();
};


class Set
{
private:

	string fileName;
	array<Card*, TABLE_SIZE> hashTable;

public:

	Set();

	int     hashFunction(int) const;
	Card*   getHashTable(int) const;
	string  bucketEnd(Card*) const;
	string  bucketFront(Card*) const;
	string  getFileName();
	void    setFileName(string);
	void    printHash();
	void    addCard(Card*);
};

int  titleScreen();
void setUnion(vector<Set>&);
void setImport(vector<Set>&);
void setIntersection(vector<Set>&);
void setDifference(vector<Set>&);
void searchCards(vector<Set>&);
void printSetHash(vector<Set>&);
void printSetList(vector<Set>&);
void outputSet(vector<Set>&);

//===========================================================================
//===========================================================================
//===========================================================================
int main()
{
	vector<Set>    cardSet;
	int            menuSelection;

	while (1)
	{
		menuSelection = titleScreen();
		switch (menuSelection)
		{
		case 1: setImport(cardSet); break;
		case 2: setUnion(cardSet); break;
		case 3: setIntersection(cardSet); break;
		case 4: setDifference(cardSet); break;
		case 5: searchCards(cardSet); break;
		case 6: printSetHash(cardSet); break;
		case 7: printSetList(cardSet); break;
		case 8: outputSet(cardSet); break;
		case -21: system("cls"); break;
		}
	}
	return 0;
}
//===========================================================================
//===========================================================================
//===========================================================================



Card::Card() :

	face(),
	suit(),
	nextCard(nullptr),
	prevCard(nullptr) {}

Card::Card(string(face), string(suit)) :

	nextCard(nullptr),
	prevCard(nullptr) {}
/*
=================================================================
COPY CONSTRUCTORS
=================================================================
*/
Card::Card(Card& c1) :

	face(c1.getFace()),
	suit(c1.getSuit()),
	nextCard(nullptr),
	prevCard(nullptr) {}

Card::Card(Card* c1) :

	face(c1->getFace()),
	suit(c1->getSuit()),
	nextCard(nullptr),
	prevCard(nullptr) {}
/*
=================================================================
DESTRUCTORS
=================================================================
*/
Card::~Card() { cout << "\nDestroying Card... (" << face << " of " << suit << ")"; }
/*
=================================================================
OPERATOR OVERLOADS
=================================================================
*/
bool Card::operator== (Card* c2)
{
	if (c2 == this)
	{
		cout << "\nThis is the same memory address for " << face << " of " << suit;
	}
	return ((this->face == c2->getFace()) && (this->suit == c2->getSuit()));
}
Card* Card::operator= (Card* rhs)
{
	return rhs;
}
Card Card::operator= (Card& rhs)
{
	this->face = rhs.getFace();
	this->suit = rhs.getSuit();
	this->nextCard = rhs.getNextPtr();
	this->prevCard = rhs.getPrevPtr();
}
/*
=================================================================
GETTERS
=================================================================
*/
string Card::getFace() const { return face; }
string Card::getSuit() const { return suit; }
Card*  Card::getNextPtr() const { return nextCard; }
Card*  Card::getPrevPtr() const { return prevCard; }
/*
=================================================================
SETTERS
=================================================================
*/
void  Card::setFace(string f) { face = f; }
void  Card::setSuit(string s) { suit = s; }
void  Card::setNextPtr(Card* tmpNextCard) { nextCard = tmpNextCard; }
void  Card::setPrevPtr(Card* tmpPrevCard) { prevCard = tmpPrevCard; }
void  Card::setPrevPtr(int i) { prevCard = nullptr; }


void Card::printCard() const { cout << face << " of " << suit << " "; return; }
/*
=================================================================
FUNCTION.   : getKey
PARAMETERS  : n/a
RETURN TYPE : int
DESCRIPTION : returns sum of ASCII values of the card's face and suit
=================================================================
*/
int Card::getKey()
{
	auto key = 0;
	for (int i : face) key += i;
	for (int i : suit) key += i;
	return key;
}

void Card::eraseCard()
{
	if ((this->nextCard == nullptr) && (this->prevCard == nullptr))
	{
		delete this;
		return;
	}
	else if (this->nextCard != nullptr)
	{
		if (this->prevCard != nullptr)
		{
			(this->nextCard)->setPrevPtr(this->prevCard);
			delete this;
			return;
		}
		else
		{
			(this->prevCard)->setPrevPtr(nullptr);
			delete this;
			return;
		}
	}
	else if (this->prevCard != nullptr)
	{
		(this->prevCard)->setNextPtr(nullptr);
		delete this;
		return;
	}
}


Set::Set() : hashTable() {}
/*
=================================================================
SETTERS
=================================================================
*/
string Set::getFileName() { return fileName; }
/*
=================================================================
GETTERS
=================================================================
*/
Card* Set::getHashTable(int i) const { return hashTable[i]; }
void  Set::setFileName(string file) { fileName = file; }
/*
=================================================================
FUNCTION.   : bucketEnd
PARAMETERS  : CARD POINTER (tail bucket link)
RETURN TYPE : STRING
DESCRIPTION : recursive function that takes the tail bucket link,
and prints out links from head to tail
=================================================================
*/
string Set::bucketEnd(Card* endLink) const
{
	string s;
	if (endLink->getPrevPtr() == nullptr)
	{
		s = endLink->getFace() + " of " + endLink->getSuit() + " -> ";
		return s;
	}
	else
	{
		s = endLink->getFace() + " of " + endLink->getSuit() + " -> ";
		return bucketEnd(endLink->getPrevPtr()) + s;
	}
}
/*
=================================================================
FUNCTION.   : bucketEnd
PARAMETERS  : CARD POINTER (tail bucket link)
RETURN TYPE : STRING
DESCRIPTION : recursive function that takes the tail bucket link,
and prints out links from head to tail
=================================================================
*/
string Set::bucketFront(Card* frontLink) const
{
	string s = frontLink->getFace() + " of " + frontLink->getSuit() + " -> ";
	if (frontLink->getNextPtr() == nullptr) return s;
	else { return s + bucketFront(frontLink->getNextPtr()); }
}
/*
=================================================================
FUNCTION.   : hashFunction
PARAMETERS  : Card hash key (INT)
RETURN TYPE : INT
DESCRIPTION : takes key, runs through has function, and returns
hash value
=================================================================
*/
int Set::hashFunction(int key) const { return (key % TABLE_SIZE); }
/*
=================================================================
FUNCTION.   : printHash
PARAMETERS  : n/a
RETURN TYPE : VOID
DESCRIPTION : prints the entire Set hashTable with bucket links
=================================================================
*/
void Set::printHash()
{
	Card* tmpPtr;
	cout << "\n-------------------------------------------------------------";
	cout << "\n\n>>Printing Hash Table (created from " << fileName << ": \n";
	cout << "\n\t[] - hash index, \n\t-> - bucket links\n";
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		tmpPtr = hashTable[i];
		if (tmpPtr == nullptr) cout << "\n\t " << "[" << i << "] -> NULL";

		else
		{
			while (tmpPtr->getNextPtr() != nullptr)
			{
				tmpPtr = tmpPtr->getNextPtr();
			}
			cout << "\n\t [" << i << "] -> " << bucketEnd(tmpPtr);
		}
	}
	cout << "\n\n-------------------------------------------------------------\n";
}
/*
=================================================================
FUNCTION.   : addCard
PARAMETERS  : CARD POINTER
RETURN TYPE : VOID
DESCRIPTION : takes a newly created card pointer and probes Set
hashTable for first available slot at the index
corresponding to hashFunction
=================================================================
*/
void Set::addCard(Card* newCard)
{
	int   hashValue = hashFunction(newCard->getKey());
	Card* arrayPtr = hashTable[hashValue];

	cout << "\n\n\t> Attempting to insert "; newCard->printCard(); cout << "at [" << hashValue << "]:";

	if (arrayPtr == nullptr)
	{
		cout << "\n\t> hash slot is empty... ";
		//cout << "\n\t> hashTable[" << hashValue << "] = "; newCard->printCard();
		hashTable[hashValue] = new Card(newCard);
		arrayPtr = hashTable[hashValue];
		cout << "\n\t    ==========================================================";
		cout << "\n\t    || UPDATED: hashTable[" << hashValue << "] -> " << bucketFront(arrayPtr);
		cout << "\n\t    ==========================================================";
		//string tmpFace = hashTable[hashValue]->getFace();
		//for (int i : tmpFace) cout << "\nASCII " << i << ": " << static_cast<char>(i);
		return;
	}
	cout << "\n\t> hashTable[" << hashValue << "] -> " << bucketFront(arrayPtr);
	while (arrayPtr != nullptr)
	{
		if (*arrayPtr == newCard)
		{
			//cout << "\n\t    -----------------------------------------------------------";
			cout << "\n     \n\t\tduplicate detected, deleting new card...("; arrayPtr->printCard(); cout << ")";
			cout << "\n\t       -----------------------------------------------------------";
			newCard->eraseCard();
			return;
		}
		else if (arrayPtr->getNextPtr() == nullptr)
		{
			//cout << "\n\t> next link slot is empty";
			cout << "\n\t> linking "; newCard->printCard();
			cout << "to "; arrayPtr->printCard();
			Card* arrayPtrNext = arrayPtr->getNextPtr();

			arrayPtrNext = new Card(newCard);
			arrayPtr->setNextPtr(arrayPtrNext);
			arrayPtrNext->setPrevPtr(arrayPtr);
			arrayPtrNext->setNextPtr(nullptr);
			cout << "\n\t    ==========================================================";
			cout << "\n\t    || UPDATED: hashTable[" << hashValue << "]: " << bucketEnd(arrayPtrNext);
			cout << "\n\t    ==========================================================";
			return;
		}
		else if (arrayPtr->getNextPtr() != nullptr)
		{
			//cout << "\n\t> next link slot is occupied";
			//cout << "\n\t>> tempCard(" << arrayPtr << ") = tempCard->getNextPtr(" << arrayPtr->getNextPtr() << ")";
			//cout << "\n\t>> " << arrayPtr->getNextPtr()->getFace() << " of " << arrayPtr->getNextPtr()->getSuit()
			//	 << " linked to " << arrayPtr->getFace() << " of " << arrayPtr->getSuit();
			//cout << "\n\t> hashTable[" << hashValue << "]: " << bucketEnd(arrayPtr->getNextPtr());
			arrayPtr = arrayPtr->getNextPtr();

		}
	}
}

string getFileContents(ifstream& File)
{
	string   Lines = "";

	if (File.good())
	{
		while (File.good())
		{
			string TempLine;
			getline(File, TempLine);
			TempLine += "\n";
			Lines += TempLine;
		} return Lines;
	}
	else { return "File did not open\n"; }
}

int titleScreen()
{
	ifstream  titleFile;

	titleFile.open("MainMenu.txt");
	string Art = getFileContents(titleFile);
	cout << Art << endl;
	titleFile.close();

	return (_getch() - 48);
}
/*
=================================================================
FUNCTION.   : Union
PARAMETERS  : Set1, Set2
RETURN TYPE : VOID
DESCRIPTION : calculates the union between two sets and places
the result in a new set
=================================================================
*/
void setUnion(vector<Set>& cardSet)
{
	Card* tmpPtr;
	Card* tmpPtr2;
	Set*  newSet = new Set;
	Set   s1;
	Set   s2;

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		tmpPtr = s1.getHashTable(i);
		while (tmpPtr != nullptr)
		{
			for (Card* tmpPtr2(s2.getHashTable(i)); tmpPtr2 != nullptr; tmpPtr2 = tmpPtr2->getNextPtr())
			{
				if (tmpPtr == tmpPtr2)
				{
					Card* tmpPtrNext(tmpPtr->getNextPtr());
					//if (tmpPtr->getPrevPtr() != nullptr) { tmpPtrNext->setPrevPtr(tmpPtr->getPrevPtr()); }
					//else tmpPtrNext->setPrevPtr(nullptr);
					delete tmpPtr;
					tmpPtr = tmpPtrNext;
				}
			}
			if (tmpPtr != nullptr)
			{
				Card* tmpCard = new Card(tmpPtr);
				newSet->addCard(tmpCard);
				tmpPtr = tmpPtr->getNextPtr();
			}
		}
	}
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		tmpPtr2 = s2.getHashTable(i);
		if (tmpPtr2 != nullptr)
		{
			for (Card* tmpPtr2(s2.getHashTable(i)); tmpPtr2 != nullptr; tmpPtr2 = tmpPtr2->getNextPtr())
			{
				Card* tmpCard = new Card(tmpPtr2);
				newSet->addCard(tmpCard);
			}
		}
	}
	newSet->printHash();
}

void setImport(vector<Set>& cardSet)
{
	ifstream       deckInputFile;
	ifstream       inFile;
	ifstream       artFile;
	ofstream       deckOutputFile;
	string         face;
	string         suit;
	string         file;
	string         Art_import;
	const char*    systemCall;
	int            fileStringInput;
	//char           confirmResponse;
	int            i = 1;
	auto		   initialSize = cardSet.size();
	Set*           tmpSet = new Set;
	vector<string> setFiles;

	//cout << "\n(enter '=' skip and to continue to file import)\n";
	system("cls");
	artFile.open("Import.txt");
	Art_import = getFileContents(artFile);
	cout << Art_import << endl;
	artFile.close();
	//systemCall[0] = "cd C:\\Users\\Adam\\source\\repos\\ConsoleApplication5\\ConsoleApplication5";
	systemCall = "dir | gawk '/[ ][H^][.]*(\\.txt)$/' | gawk '{ print $5 }' > gawkFileList.txt";
	//systemCall[2] = "echo gawktestList.txt | gawk '/H^.*(\.txt)$/'";
	//systemCall[2] = "gawk '{ if ($5 ~ /[H]^.*\\.txt?/) printf(\"%s\\n\",$5) }' gawktestList.txt > gawkFileList.txt";

	system(systemCall);
	//system(systemCall[1]);
	//system(systemCall[2]);

	inFile.open("gawkFileList.txt");

	cout << "\nAvailable .txt files\n\n";
	do
	{
		string file;
		inFile >> file;
		if (!inFile.eof())
		{
			setFiles.push_back(file);
			cout << "\t" << i << " - " << file << endl;
			i++;
		}
	} while (!inFile.eof());

	cout << "\n\nPress <1-" << setFiles.size() << "> to tag for import...\nPress <ENTER> to import\n\n";
	cout << "\nFiles to import:\n\n";

	auto j = initialSize;
	while (((fileStringInput = _getch()) != 13) && (fileStringInput != 27))
	{
		cout << "\t" << setFiles[fileStringInput - 49] << endl;
		tmpSet->setFileName(setFiles[fileStringInput - 49]);
		cardSet.push_back(*tmpSet);
		cardSet[j].setFileName(setFiles[fileStringInput - 49]);
		j++;
	}
	/*
	do
	{

	file = "";
	cout << "\nEnter the name Card hand to import : ";

	while(((fileStringInput = _getch()) != 27) && (fileStringInput != 13) && (fileStringInput != 61))
	{
	cout << fileStringInput;
	string tmp = "";
	cin >> tmp;
	file = fileStringInput + tmp;
	cout << "\nfileStringInput: " << static_cast<int>(fileStringInput);
	cout << "\ntmp: " << tmp;
	cout << "\nfile: " << file;
	cout << "\n";
	}

	if (fileStringInput == 27)
	{
	system("cls");
	return;
	}
	if (fileStringInput == 61)
	{
	cout << "\nlittle loop";
	break;
	}

	if (fileStringInput == 13)
	{
	cout << "Confirm filename: " << file << " (Y/N): ";
	cin >> confirmResponse;
	if ((confirmResponse == 'Y') || (confirmResponse == 'y'))
	{
	tmpSet->setFileName(file);
	cardSet.push_back(*tmpSet);
	}
	}

	} while (fileStringInput != 61);*/
	int currentSize = cardSet.size();

	for (int i = initialSize; i < currentSize; i++)
	{
		deckInputFile.open(cardSet[i].getFileName());
		if (deckInputFile.fail()) cout << "File failed to open" << endl;

		cout << "\n\n>> Deck file successfully opened\n\n";

		while (!deckInputFile.eof())
		{
			deckInputFile >> face >> suit;
			cout << "face: " << face << endl;
			cout << "suit: " << suit << endl;
			//tempCard.setFace(face);
			//tempCard.setSuit(suit);
			Card* tempCard = new Card;

			tempCard->setFace(face);
			tempCard->setSuit(suit);
			tempCard->printCard();

			cardSet[i].addCard(tempCard);
			cout << "\nCard added";
		}
		cout << "\n\n...Closing Deck file" << i + 1 << endl;
		deckInputFile.close();
		//cout << "\n>>Contents of Set " << i+1 << ": \n" << endl;
		//cardSet[i].printSetHash();
	}
	system("cls");
}

void setIntersection(vector<Set>& cardSet)
{
	return;
}

void setDifference(vector<Set>& cardSet)
{
	return;
}

void searchCards(vector<Set>& cardSet)
{
	return;
}

void printSetHash(vector<Set>& cardSet)
{
	char userResponse;
	string printSetArt;
	ifstream artImport;

	system("cls");
	artImport.open("PrintSet.txt");
	printSetArt = getFileContents(artImport);
	cout << printSetArt << endl;

	do
	{
		cout << "\n\tPress <1-" << cardSet.size() << "> to print set: ";
		cout << "\n\tPress <ESC> for Main Menu\n";

		for (int i; i < cardSet.size(); i++)
			cout << "\n\t\t" << i + 1 << " - " << cardSet[i].getFileName();

		userResponse = _getch();
	
		auto range = (1, cardSet.size());
		if (userResponse != '\x1b')
			cardSet[userResponse - 49].printHash();
		else if (userResponse == '\x1b')
		{
			system("cls");
			return;
		}

	} while (userResponse != '\x1b');

	system("cls");
	return;
}

void printSetList(vector<Set>& cardSet)
{
	ifstream setListFile;
	ifstream setFile;
	vector<string> setList;
	char userInput;
	const char* systemCall;

	systemCall = "dir | gawk '/Set^[.]*\\.(txt)$/ | gawk '{ print $5 } > setList.txt";
	system(systemCall);

	setListFile.open("setList.txt");
	while (!setListFile.eof())
	{
		static int i = 1;
		string set;
		setListFile >> set;
		cout << "\t" << i << " - " << set << endl;
		setList.push_back(set);
		i++;
	}
	setListFile.close();

	while ((userInput = _getch()) != '\x1b')
	{
		setFile.open(setList[userInput - 48]);
		while (!setFile.eof())
		{
			string face, suit;
			setFile >> face >> suit;
			cout << face << " " << suit << endl;
		}
		setFile.close();
	}

	


	


}

void outputSet(vector<Set>& cardSet)
{
	ofstream  outputFile;
	ifstream  artImport;
	string    artFile;
	int       fileNum;
	Card*     tmpPtr;
	string    outputFileName;

	system("cls");
	artImport.open("ExportSet.txt");
	artFile = getFileContents(artImport);
	cout << "\t" << artFile << endl;
	artImport.close();


	for (int i = 0; i < cardSet.size(); i++)
		cout << "\n" << i + 1 << " - " << cardSet[i].getFileName();

	cout << "\nEnter file to output (1-" << cardSet.size() << "): "; cin >> fileNum;
	outputFileName = "Set" + cardSet[fileNum - 1].getFileName();
	outputFile.open(outputFileName);

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		tmpPtr = cardSet[fileNum - 1].getHashTable(i);
		while (tmpPtr != nullptr)
		{
			outputFile << tmpPtr->getFace() << " " << tmpPtr->getSuit() << endl;
			tmpPtr = tmpPtr->getNextPtr();
		}
	}


	system("cls");
	system("dir");
	//system(sys += out);
	return;
}
