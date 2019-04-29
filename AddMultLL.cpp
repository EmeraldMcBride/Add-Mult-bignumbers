
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

struct node{
  double data;
  node* next;
  bool lessThan;
};

node* reverseList(node * list, node * acc){
  if(list == NULL)
    return acc;
  else{
    node * nodeCopy = new node();
    nodeCopy->data = list->data;
    if(acc == NULL)
      nodeCopy->next = NULL;
    else
      nodeCopy->next = acc;
    list = list->next;
    return reverseList(list, nodeCopy);
  }
}

node* addNode(node * list, double data, bool lessThan){
  node * temp = new node;
  temp->data = data;
  temp->lessThan = lessThan;
  temp->next = list;
  list = temp;
  return list;
}

node* buildList(node* list, string fileName){
  ifstream inFile;
  inFile.open(fileName.c_str());
  double data = 0;
  bool lessThan = false;
  while(inFile >> data){
    if(data < 1 && data > 0){
      lessThan = true;
      list = addNode(list,data*10,lessThan);
    }else{
      list = addNode(list,data,lessThan);
    }
  }
  inFile.close();
  return list;
}

void print(node * list, string fileName){

  ofstream outFile;
  outFile.open(fileName.c_str());
  while(list){
    if(list->data < 1 && list->data > 0){
      outFile << ".";
      double temp = list->data *10;
      int temp2 = (int)temp;
      outFile << temp2 << endl;
      list = list->next;
      continue;
    }
    outFile << list->data << endl;
    list = list->next;
  }
}
//Counts the elements that are after the decimal.
int lessThan(node * list){
  int counter = 0;
  while(list){
    if(list->lessThan)
      counter++;
    list = list->next;
  }
  return counter;
}
//Pads zeros on to addition to add decimals
//Pads zeros on to lists for multi of decimals
void padZeros(node *&list, int zeros){
  for(int i = 0; i < zeros; i++)
    list = addNode(list, 0, true);
}
//Helper function for adding numbers, returns carry and adds
//number to list.
double addNumberReturnCarry(node *& list, double dataOne, \
                        double dataTwo,double carry, bool lessThan){
  list = addNode(list, fmod((dataOne+dataTwo+carry), 10), lessThan);
  int returnMe = (int) (dataOne+dataTwo+carry);
  return returnMe/10;
}
//Function to add numbers with use of a healper function
//found above
node* addNumbers(node * listOne, node * listTwo){
  node * returnMe = NULL;
  double carry = 0;

  while(listOne || listTwo){

    if(!listOne){
      carry = addNumberReturnCarry(returnMe, 0, listTwo->data, carry, \
                                   listTwo->lessThan);
      listTwo = listTwo->next;
      continue;
    }

    if(!listTwo){
      carry = addNumberReturnCarry(returnMe, listOne->data, 0, carry, \
                                   listOne->lessThan);
      listOne = listOne->next;
      continue;
    }

    if(listOne && listTwo){
      carry = addNumberReturnCarry(returnMe, listOne->data, listTwo->data, \
                                   carry, listOne->lessThan);
      listOne = listOne->next;
      listTwo = listTwo->next;
      continue;
    }
  }
  if(carry > 0)
    returnMe = addNode(returnMe, carry, false);
  return returnMe;
}
//Multiplys numbers together, padding with zeros
// and reversing the list when needed.
node* multiplyNumbers(node* listOne, node* listTwo){
  node* returnMe = NULL;
  int zeros = 0;
  while(listOne){
    node * multiMe = NULL;
    for(int i = 0; i < listOne->data; i++){//*tens
      multiMe = addNumbers(listTwo, multiMe);
      multiMe = reverseList(multiMe, NULL);
    }
    padZeros(multiMe, zeros);
    zeros++;
    listOne = listOne->next;
    returnMe = addNumbers(multiMe, returnMe);
    returnMe = reverseList(returnMe,NULL);
  }
  return returnMe;
}
//Adds the decimal to my answers
node* addDecimals(node * list, int decimals){
  list = reverseList(list,NULL);
  node* temp = list;
  for(int i = 1; i < decimals; i++)
    temp = temp->next;
  temp->data = temp->data/10;
  list = reverseList(list,NULL);
  return list;
}

int main(int argc, char* argv[]){
  if(argc < 5){
    cout << "Please use like this:" << endl;
    cout << "a.out [add/mult] inFile1 inFile2 outFile" << endl;
    return 1;
  }

  node* listOne;
  node* listTwo;
  node* ansList;
  int listOneDecimals;
  int listTwoDecimals;
  int decimals = 0;
  int choice = -1;
  int diff = 0;
  listOne = buildList(listOne, argv[2]);
  listTwo = buildList(listTwo, argv[3]);

  listOneDecimals = lessThan(listOne);
  listTwoDecimals = lessThan(listTwo);

  if(!strcmp(argv[1], "add")){
    diff = abs(listOneDecimals - listTwoDecimals);
    if(listOneDecimals > listTwoDecimals){
      padZeros(listTwo,diff);
      decimals = listOneDecimals;
    }else{
      padZeros(listOne,diff);
      decimals = listTwoDecimals;
    }
    ansList = addNumbers(listOne, listTwo);
  }else if(!strcmp(argv[1], "mult")){
    ansList = multiplyNumbers(listOne, listTwo);
    decimals = listOneDecimals+listTwoDecimals;
    ansList = reverseList(ansList ,NULL);
  }


  if(decimals > 0)
    ansList = addDecimals(ansList, decimals);

  print(ansList, argv[4]);

  return 0;
}