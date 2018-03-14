#include<iostream>
#include<string>
#include<stdio.h>
//#include<termios.h>
#include<array>
#include<memory>
#include<fstream>
#include<vector>
#include<iomanip>
#include<cctype>
#include<iterator>
#include<exception>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

using namespace std;

const int TABLE_SIZE = 20;
/*
====================================================
CARD CLASS
====================================================
*/
class Card {
private:
	std::string  	   face;
	std::string  	   suit;
	unique_ptr<Card>   nextCard;
	unique_ptr<Card>   prevCard;
public:
	Card();
	Card(std::string(face), std::string(suit));
	Card(Card&);
	Card(unique_ptr<Card>);
	~Card();
	bool              operator==(unique_ptr<Card>);
	unique_ptr<Card>  operator=(unique_ptr<Card>);
	Card              operator=(Card&);
	unique_ptr<Card>  getNextPtr()const;
	unique_ptr<Card>  getPrevPtr()const;
	std::string            getFace()const;
	std::string            getSuit()const;
	void              printCard()const;
	void              setFace(std::string);
	void              setSuit(std::string);
	void              setNextPtr(unique_ptr<Card>);
	void              setPrevPtr(unique_ptr<Card>);
	void              setPrevPtr(int);
	void              eraseCard();
	int               getKey();
};
/*
====================================================
SET CLASS
====================================================
*/
class Set {
private:
	std::string       fileName;
	array<unique_ptr<Card>, TABLE_SIZE> hashTable;
public:
	Set();
	int     hashFunction(int)   const;
	unique_ptr<Card>   getHashTable(int)   const;
	std::string  bucketEnd(unique_ptr<Card>) const;
	std::string  bucketFront(unique_ptr<Card>) const;
	std::string  getFileName();
	void    setFileName(std::string);
	void    printHash(std::string);
	void    addCard(unique_ptr<Card>);
};

/*
====================================================
FUNCTION DEFINITIONS
====================================================
*/
int  titleScreen();
void setUnion(vector<Set>&);
void setImport(vector<Set>&);
void setIntersection(vector<Set>&);
void setDifference(vector<Set>&);
void searchCards(vector<Set>&);
void printSetHash(vector<Set>&);
void printSetList(vector<Set>&);
void outputSet(vector<Set>&);
void loadArtwork(std::string);


static struct termios old, new1;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
	tcgetattr(0, &old); //grab old terminal i/o settings
	new1 = old; //make new settings same as old settings
	new1.c_lflag &= ~ICANON; //disable buffered i/o
	new1.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
	tcsetattr(0, TCSANOW, &new1); //apply terminal io settings
}

/* Restore old terminal i/o settings */
/*void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

char getch(void)
{
	return getch_(0);
}*/


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
		case   1: setImport(cardSet); break;
		case   2: setUnion(cardSet); break;
		case   3: setIntersection(cardSet); break;
		case   4: setDifference(cardSet); break;
		case   5: searchCards(cardSet); break;
		case   6: printSetHash(cardSet); break;
		case   7: printSetList(cardSet); break;
		case   8: outputSet(cardSet); break;
		case -21: system("cls"); break;
		}
	}
	return 0;
}
//===========================================================================
//===========================================================================
//===========================================================================



Card::Card() : face(), suit(), nextCard(nullptr), prevCard(nullptr) {}
Card::Card(std::string(face), std::string(suit)) : nextCard(nullptr), prevCard(nullptr) {}
/*
=================================================================
COPY CONSTRUCTORS
=================================================================
*/
Card::Card(Card& c1) : face(c1.getFace()), suit(c1.getSuit()), nextCard(nullptr), prevCard(nullptr) {}
Card::Card(unique_ptr<Card> c1) : face(c1->getFace()), suit(c1->getSuit()), nextCard(nullptr), prevCard(nullptr) {}
/*
=================================================================
DESTRUCTORS
=================================================================
*/
Card::~Card() { std::cout << "\nDestroying Card... (" << face << " of " << suit << ")"; }
/*
=================================================================
OPERATOR OVERLOADS
=================================================================
*/
bool  Card::operator== (unique_ptr<Card> c2)
{
	if (c2 == this)
	{
		std::cout << "\nThis is the same memory address for " << face << " of " << suit;
	}
	return ((this->face == c2->getFace()) && (this->suit == c2->getSuit()));
}
unique_ptr<Card> Card::operator= (unique_ptr<Card> rhs) { return rhs; }
Card  Card::operator= (Card& rhs)
{
	this->face = rhs.getFace();
	this->suit = rhs.getSuit();
	this->nextCard = rhs.getNextPtr();
	this->prevCard = rhs.getPrevPtr();
	return *this;
}
/*
=================================================================
GETTERS
=================================================================
*/
std::string Card::getFace()        const { return face; }
std::string Card::getSuit()        const { return suit; }
unique_ptr<Card>  Card::getNextPtr() const { return nextCard; }
unique_ptr<Card>  Card::getPrevPtr() const { return prevCard; }
/*
=================================================================
SETTERS
=================================================================
*/
void  Card::setFace(std::string f) { face = f; }
void  Card::setSuit(std::string s) { suit = s; }
void  Card::setNextPtr(unique_ptr<Card> tmpNextCard) { nextCard = tmpNextCard; }
void  Card::setPrevPtr(unique_ptr<Card> tmpPrevCard) { prevCard = tmpPrevCard; }
void  Card::setPrevPtr(int i) { prevCard = nullptr; }

void Card::printCard() const { std::cout << face << " of " << suit << " "; return; }
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
	int key = 0;
	for (int i : face) key += i;
	for (int i : suit) key += i;
	return key;
}
/*
=================================================================
FUNCTION.   : eraseCard
PARAMETERS  : n/a
RETURN TYPE : void
DESCRIPTION : erase card from linked list
=================================================================
*/
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
/*
=================================================================
CONSTRUCTORS
=================================================================
*/
Set::Set() : hashTable() {}
/*
=================================================================
SETTERS
=================================================================
*/
std::string Set::getFileName() { return fileName; }
/*
=================================================================
GETTERS
=================================================================
*/
unique_ptr<Card> Set::getHashTable(int i) const { return hashTable[i]; }
void             Set::setFileName(std::string file) { fileName = file; }
/*
=================================================================
FUNCTION.   : bucketEnd
PARAMETERS  : CARD POINTER (tail bucket link)
RETURN TYPE : STRING
DESCRIPTION : recursive function that takes the tail bucket link,
and prints out links from head to tail
=================================================================
*/
std::string Set::bucketEnd(Card> endLink) const
{
	std::string s;

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
std::string Set::bucketFront(unique_ptr<Card> frontLink) const
{
	std::string s = frontLink->getFace() + " of " + frontLink->getSuit() + " -> ";
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
void Set::printHash(std::string graphic)
{
	unique_ptr<Card> tmpPtr;

	//std::cout << "\n-------------------------------------------------------------\n";
	std::cout << "\n\n";
	std::cout << graphic;
	//std::cout << "\n\t>>Printing Hash Table (created from " << fileName << ": \n";
	//std::cout << "\n\t[] - hash index, \n\t-> - bucket links\n";
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		tmpPtr = hashTable[i];
		if (tmpPtr == nullptr) std::cout << "\n\t " << "[" << i << "] -> NULL";
		else
		{
			while (tmpPtr->getNextPtr() != nullptr)
			{
				tmpPtr = tmpPtr->getNextPtr();
			}
			std::cout << "\n\t [" << i << "] -> " << bucketEnd(tmpPtr);
		}
	}
	//std::cout << "\n\n-------------------------------------------------------------\n";
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
void Set::addCard(unique_ptr<Card> newCard)
{
	int   hashValue = hashFunction(newCard->getKey());
	unique_ptr<Card> arrayPtr = hashTable[hashValue];


	std::cout << "\n\n\t> Attempting to insert "; newCard->printCard(); std::cout << "at [" << hashValue << "]:";

	if (arrayPtr == nullptr)
	{
		std::cout << "\n\t> hash slot is empty... ";
		hashTable[hashValue] = new Card(newCard);
		arrayPtr = hashTable[hashValue];
		std::cout << "\n\t    ==========================================================";
		std::cout << "\n\t    || UPDATED: hashTable[" << hashValue << "] -> " << bucketFront(arrayPtr);
		std::cout << "\n\t    ==========================================================";
		return;
	}
	std::cout << "\n\t> hashTable[" << hashValue << "] -> " << bucketFront(arrayPtr);

	while (arrayPtr != nullptr)
	{
		if (*arrayPtr == newCard)
		{
			std::cout << "\n     \n\t\tduplicate detected, deleting new card...("; arrayPtr->printCard(); std::cout << ")";
			std::cout << "\n\t       -----------------------------------------------------------";
			newCard->eraseCard();
			return;
		}
		else if (arrayPtr->getNextPtr() == nullptr)
		{
			std::cout << "\n\t> linking "; newCard->printCard();
			std::cout << "to "; arrayPtr->printCard();
			unique_ptr<Card> arrayPtrNext = arrayPtr->getNextPtr();

			arrayPtrNext = new Card(newCard);
			arrayPtr->setNextPtr(arrayPtrNext);
			arrayPtrNext->setPrevPtr(arrayPtr);
			arrayPtrNext->setNextPtr(nullptr);
			std::cout << "\n\t    ==========================================================";
			std::cout << "\n\t    || UPDATED: hashTable[" << hashValue << "]: " << bucketEnd(arrayPtrNext);
			std::cout << "\n\t    ==========================================================";
			return;
		}
		else if (arrayPtr->getNextPtr() != nullptr)
		{
			arrayPtr = arrayPtr->getNextPtr();
		}
	}
}
/*
=================================================================
FUNCTION.   : getFileContents
PARAMETERS  : ifstream
RETURN TYPE : std::string
DESCRIPTION : read file and convert it's contents to a single
std::string, then return that std::string
=================================================================
*/
std::string getFileContents(ifstream& File)
{
	std::string   Lines = "";

	if (File.good())
	{
		while (File.good())
		{
			std::string TempLine;
			getline(File, TempLine);
			TempLine += "\n";
			Lines += TempLine;
		} return Lines;
	}
	else { return "File did not open\n"; }
}

/*
=================================================================
FUNCTION.   : addNumberGraphic
PARAMETERS  : std::string, std::string
RETURN TYPE : std::string header, std::string num
DESCRIPTION : concatenate header graphic with number graphic
to display various graphics headers (e.g. "Set1",
"Set2", "Set3")
=================================================================
*/
std::string addNumberGraphic(std::string header, std::string num)
{
	std::string          Lines = "";
	ifstream        numFile;
	ifstream        headFile;
	std::string          blanks;
	vector<std::string>  tempFold;
	vector<std::string>  tempFold2;
	std::string          TempLine;
	std::string          TempLine1;

	headFile.open("Graphics\\Set.txt");
	if (headFile.fail()) std::cout << "Failed to open";
	while (!headFile.eof())
	{
		getline(headFile, TempLine);
		if (TempLine.length() == 1)
		{
			TempLine.pop_back();
		}
		tempFold.push_back(TempLine);
	}
	headFile.close();

	//for (int i = 0; i < tempFold.size(); i++)


	numFile.open("Graphics\\" + num + ".txt");
	if (numFile.fail()) std::cout << "numFile failed to open";

	while (!numFile.eof())
	{
		getline(numFile, TempLine1);
		if (TempLine1.length() == 1)
		{
			TempLine1.pop_back();
		}
		tempFold2.push_back(TempLine1);
	}

	for (int i = 0; i < tempFold.size(); i++)
	{
		Lines += tempFold[i] + tempFold2[i] + "\n";
	}

	numFile.close();

	return Lines;
}
/*
=================================================================
FUNCTION.   : titleScreen
PARAMETERS  : n/a
RETURN TYPE : int
DESCRIPTION : load Menu graphic and return user-entered menu option
=================================================================
*/
int titleScreen()
{
	loadArtwork("MainMenu.txt");
	return (getch() - 48);
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
	unique_ptr<Card> tmpPtr;
	unique_ptr<Card> tmpPtr2;
	Set*  newSet = new Set;
	Set   s1;
	Set   s2;

	loadArtwork("SetUnion.txt");

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		tmpPtr = s1.getHashTable(i);
		while (tmpPtr != nullptr)
		{
			for (unique_ptr<Card> tmpPtr2(s2.getHashTable(i)); tmpPtr2 != nullptr; tmpPtr2 = tmpPtr2->getNextPtr())
			{
				if (tmpPtr == tmpPtr2)
				{
					unique_ptr<Card> tmpPtrNext(tmpPtr->getNextPtr());
					delete tmpPtr;
					tmpPtr = tmpPtrNext;
				}
			}
			if (tmpPtr != nullptr)
			{
				unique_ptr<Card> tmpCard = new Card(tmpPtr);
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
			for (unique_ptr<Card> tmpPtr2(s2.getHashTable(i)); tmpPtr2 != nullptr; tmpPtr2 = tmpPtr2->getNextPtr())
			{
				unique_ptr<Card> tmpCard = new Card(tmpPtr2);
				newSet->addCard(tmpCard);
			}
		}
	} newSet->printHash("Graphics\\Set.txt");
}
/*
=================================================================
FUNCTION.   : setImport
PARAMETERS  : vector<Set>
RETURN TYPE : VOID
DESCRIPTION : get list of available "hand.txt" files from current
directory, display to user, capture user-input,
and load files into program
=================================================================
*/
void setImport(vector<Set>& cardSet)
{
	ifstream        deckInputFile;
	ifstream        inFile;
	ifstream        artFile;
	ofstream        deckOutputFile;
	std::string          face;
	std::string          suit;
	std::string          file;
	std::string          Art_import;
	const char*     systemCall;
	int             fileStringInput;
	int             i = 1;
	int		        initialSize = cardSet.size();
	Set*            tmpSet = new Set;
	vector<std::string>  setFiles;


	loadArtwork("Import.txt");

	systemCall = "dir | gawk 'BEGIN { RS = \"\\n| \" }; {if ($1 ~ /^H.*(txt)$/) { printf(\"%s\\n\", $1)}}' > gawkFileList.txt";
	system(systemCall);
	inFile.open("gawkFileList.txt");

	std::cout << "\nAvailable .txt files: \n\n";
	do
	{
		inFile >> file;
		if (!inFile.eof())
		{
			setFiles.push_back(file);
			std::cout << "\t" << i << " - " << file << endl;
			i++;
		}
	} while (!inFile.eof());

	std::cout << "\n\nPress <1-" << setFiles.size() << "> to tag for import...\nPress <ENTER> to import\n\n";
	std::cout << "\nFiles to import:\n\n";

	int j = initialSize;
	while (((fileStringInput = getch()) != 13) && (fileStringInput != 27))
	{
		std::cout << "\t" << setFiles[fileStringInput - 49] << endl;
		tmpSet->setFileName(setFiles[fileStringInput - 49]);
		cardSet.push_back(*tmpSet);
		cardSet[j].setFileName(setFiles[fileStringInput - 49]);
		j++;
	}
	int currentSize = cardSet.size();

	for (int i = initialSize; i < currentSize; i++)
	{
		deckInputFile.open(cardSet[i].getFileName());
		if (deckInputFile.fail()) std::cout << "File failed to open" << endl;

		std::cout << "\n\n>> Deck file successfully opened\n\n";

		while (!deckInputFile.eof())
		{
			deckInputFile >> face >> suit;
			std::cout << "face: " << face << endl;
			std::cout << "suit: " << suit << endl;
			unique_ptr<Card> tempCard = new Card;

			tempCard->setFace(face);
			tempCard->setSuit(suit);
			tempCard->printCard();

			cardSet[i].addCard(tempCard);
			std::cout << "\nCard added";
		}
		std::cout << "\n\n...Closing Deck file" << i + 1 << endl;
		deckInputFile.close();
	}
	system("cls");
}
/*
=================================================================
FUNCTION.   : printSetHash
PARAMETERS  : vector<Set>&
RETURN TYPE : VOID
DESCRIPTION : prints the entire Set hashTable with bucket links
=================================================================
*/
void setIntersection(vector<Set>& cardSet)
{
	return;
}
/*
=================================================================
FUNCTION.   : printSetHash
PARAMETERS  : vector<Set>&
RETURN TYPE : VOID
DESCRIPTION : prints the entire Set hashTable with bucket links
=================================================================
*/
void setDifference(vector<Set>& cardSet)
{
	return;
}
/*
=================================================================
FUNCTION.   : printSetHash
PARAMETERS  : vector<Set>&
RETURN TYPE : VOID
DESCRIPTION : prints the entire Set hashTable with bucket links
=================================================================
*/
void searchCards(vector<Set>& cardSet)
{
	return;
}
/*
=================================================================
FUNCTION.   : printSetHash
PARAMETERS  : vector<Set>&
RETURN TYPE : VOID
DESCRIPTION : prints the entire Set hashTable with bucket links
=================================================================
*/
void printSetHash(vector<Set>& cardSet)
{
	char      userResponse;
	std::string    printSetArt;
	ifstream  artImport;
	std::string    userR;


	loadArtwork("PrintHash.txt");

	do
	{
		std::cout << "\n\n\t=================================================================";
		std::cout << "\n\n\tPress <1-" << cardSet.size() << "> to print set: ";
		std::cout << "\n\tPress <ESC> for Main Menu\n";

		for (int i = 0; i < cardSet.size(); i++) { std::cout << "\n\t\t" << (i + 1) << " - " << cardSet[i].getFileName(); }

		userR.erase(0);
		userResponse = getch();
		userR.push_back(userResponse);

		if (userResponse != '\x1b')
		{
			std::string graphic = addNumberGraphic("Graphics\\Set.txt", userR);
			cardSet[userResponse - 49].printHash(graphic);

		}
		else if (userResponse == '\x1b') { system("cls"); return; }

	} while (userResponse != '\x1b');

	system("cls");
	return;
}
/*
=================================================================
FUNCTION.   : printSetList
PARAMETERS  : vector<Set>&
RETURN TYPE : VOID
DESCRIPTION : prints user-selected set as a list of cards
=================================================================
*/
void printSetList(vector<Set>& cardSet)
{
	std::string          set;
	std::string          face;
	std::string          suit;
	std::string          userR;
	ifstream        setListFile;
	ifstream        setFile;
	vector<std::string>  setList;
	int             userInput;
	const char*     systemCall;

	//find set files in the current directory and output to setList.txt
	systemCall = "dir | gawk '/[ ][(Set)*].*/.(txt)$/ | gawk '{ print $5 }' > setList.txt";
	system(systemCall);

	loadArtwork("PrintSet.txt");

	//Input each line of setList.txt to std::string vector
	setListFile.open("setList.txt");
	while (!setListFile.eof())
	{
		setListFile >> set;
		if (!setListFile.eof())
		{
			setList.push_back(set);
		}
	} setListFile.close();

	//output user-selected set lists until ESC is pressed
	do
	{
		//display list of set files to user
		std::cout << endl;
		for (auto i = setList.begin(); i < setList.end(); i++)
		{
			std::cout << setw(7) << " ";
			std::cout << (distance(setList.begin(), i) + 1) << " - " << *i << "\n";
		}

		//capture user input and open corresponding set file
		userInput = getch();
		if (userInput == '\x1b') return;
		userR.push_back(userInput);
		std::cout << addNumberGraphic("Set.txt", userR);
		userR.pop_back();
		setFile.open(setList[userInput - 49]);
		std::cout << "\n     ==================\n";
		//input file contents and output to console
		while (!setFile.eof())
		{
			setFile >> face;
			setFile >> suit;
			if (!setFile.eof())
			{
				std::cout << "\t" << face << " " << suit << endl;
			}
		} setFile.close();
		std::cout << "     ==================";
	} while (userInput != '\x1b');
}
/*
=================================================================
FUNCTION.   : outputSet
PARAMETERS  : vector<String>&
RETURN TYPE : VOID
DESCRIPTION : creates file and outputs set to that file as a list
=================================================================
*/
void outputSet(vector<Set>& cardSet)
{
	ofstream  outputFile;
	ifstream  artImport;
	std::string    artFile;
	int       fileNum;
	unique_ptr<Card>     tmpPtr;
	std::string    outputFileName;


	loadArtwork("ExportSet.txt");

	//display hash Set names
	for (int i = 0; i < cardSet.size(); i++)
	{
		std::cout << "\n" << i + 1 << " - " << cardSet[i].getFileName();
	}

	//prompt user to enter file number
	std::cout << "\nEnter file to output (1-" << cardSet.size() << "): ";
	cin >> fileNum;

	//create set file name and open corresponding output file
	outputFileName = "Set" + cardSet[fileNum - 1].getFileName();
	outputFile.open(outputFileName);

	//output each element of hash set to output file as a list
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
/*
=================================================================
FUNCTION.   : loadArtwork
PARAMETERS  : std::string
RETURN TYPE : VOID
DESCRIPTION : opens graphic files and outputs to console
=================================================================
*/
void loadArtwork(std::string artFileName)
{
	ifstream     artImport;
	std::string       artFile;

	system("cls");
	artImport.open("Graphics//" + artFileName);
	artFile = getFileContents(artImport);
	std::cout << "\t" << artFile << endl;
	artImport.close();
	return;
}
