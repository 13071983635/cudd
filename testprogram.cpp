#include<stdio.h>
#include<iostream>
#include"cudd.h"
#include"util.h"
using namespace std;
DdNode **createHalfAdderBDD(DdManager *manager)
{
  DdNode *x0 = Cudd_bddIthVar(manager, 0);
  DdNode *x1 = Cudd_bddIthVar(manager, 1);

  DdNode *and1 = Cudd_bddAnd(manager, x0, Cudd_Not(x1));
  Cudd_Ref(and1);

  DdNode *and2 = Cudd_bddAnd(manager, x1, Cudd_Not(x0));
  Cudd_Ref(and2);

  DdNode *sum = Cudd_bddOr(manager, and1, and2);
  Cudd_Ref(sum);
  Cudd_RecursiveDeref(manager, and1);
  Cudd_RecursiveDeref(manager, and2);

  DdNode *carry = Cudd_bddAnd(manager, x0, x1);
  Cudd_Ref(carry);
  
  //杩斿洖涓や釜BDD鏍?  DdNode **outputs = new DdNode*[2];
  outputs[0] = sum;
  outputs[1] = carry;

  return outputs;
}
/*void toDot(DdManager *manager, DdNode **outputs)
{
  char **inputNames = new char*[2];//鏍囪涓や釜杈撳叆鑺傜偣
  inputNames[0] = new char[3];
  inputNames[1] = new char[3];
  inputNames[0] = "x1";
  inputNames[1] = "x2";
  char **outputNames = new char*[2];//鏍囪涓や釜杈撳嚭鑺傜偣
  outputNames[0] = new char[4];
  outputNames[1] = new char[6];
  strcpy(outputNames[0], "sum");
  strcpy(outputNames[1], "carry");
  FILE *f = fopen("./HalfAdder.dot", "w");
  //manager锛欳UDD绠＄悊鍣?
  //2锛氳緭鍑虹殑鏁伴噺
  //outputs锛氳緭鍑鸿妭鐐圭殑鏁扮粍
  Cudd_DumpDot(manager, 2, outputs, inputNames, outputNames, f); 
}*/
DdNode *Cudd_bddRestrict(
  DdManager *manager, //DD绠＄悊鍣?  DdNode *BDD,        //鐢ㄦ潵闄愬埗鐨凚DD 
  DdNode *restrictBy  //琚檺鍒剁殑BDD
 );
void test(DdManager *manager, DdNode **node)
{
  DdNode *x0 = Cudd_bddIthVar(manager, 0);
  DdNode *x1 = Cudd_bddIthVar(manager, 1);
  const int SIZE = 4;
  DdNode *restrictBy[SIZE];
  DdNode *testSum[SIZE];
  DdNode *testCarry[SIZE];
  //閫氳繃濡備笅鏂瑰紡鏉ラ檺鍒?  restrictBy[0] = Cudd_bddAnd(manager, Cudd_Not(x0), Cudd_Not(x1));
  //x1=0 and x2=0
  restrictBy[1] = Cudd_bddAnd(manager, Cudd_Not(x0), x1);//x1=0/and//x2=1
  restrictBy[2]= Cudd_bddAnd(manager,x0,Cudd_Not(x1));//x1=1//and//x2=0
  restrictBy[3] = Cudd_bddAnd(manager, x0, x1);
  //x1=1 and x2=1
  for(int i=0; i<SIZE; i++)
  {
    Cudd_Ref(restrictBy[i]);//闄愬埗寮曠敤
    //閫氳繃涓€涓柊鍑芥暟闄愬埗
    testSum[i] = Cudd_bddRestrict(manager, node[0], restrictBy[i]);
    testCarry[i] = Cudd_bddRestrict(manager, node[1], restrictBy[i]);
    Cudd_RecursiveDeref(manager, restrictBy[i]);//娓呯悊闄愬埗
  }
  cout << "(x1 = 0, x2 = 0): sum = " << 1 - Cudd_IsComplement(testSum[0])<<' '
  << "Cary =" << 1 - Cudd_IsComplement(testCarry[0]) << endl
  << "(x1 = 0,x2 = 1)锛歴um = "<< 1-Cudd_IsComplement(testSum[1])<<' '
  << "Carry = "<< 1 - Cudd_IsComplement(testCarry[1]) << endl
  << "(x1 = 1,x2 = 0)锛歴um = "<< 1-Cudd_IsComplement(testSum[2])<<' '
  << "Carry = "<< 1 - Cudd_IsComplement(testCarry[2]) << endl
  << "(x1 = 1,x2 = 1)锛歴um = "<< 1-Cudd_IsComplement(testSum[3])<<' '
  << "Carry = " << 1 - Cudd_IsComplement(testCarry[3]) << endl;
  for(int i=0; i<SIZE; i++)
  {
    Cudd_RecursiveDeref(manager,testSum[i]);
    Cudd_RecursiveDeref(manager,testCarry[i]);
  }
}
void toDot(DdManager *manager, DdNode **outputs)
 {
   char **inputNames = new char*[2];//鏍囪涓や釜杈撳叆鑺傜偣
   inputNames[0] = new char[3];
    inputNames[1] = new char[3];
    inputNames[0] = "x1";
    inputNames[1] = "x2";
   char **outputNames = new char*[2];//鏍囪涓や釜杈撳嚭鑺傜偣
   outputNames[0] = new char[4];
    outputNames[1] = new char[6];
    strcpy(outputNames[0], "sum");
   strcpy(outputNames[1], "carry");
    FILE *f = fopen("./HalfAdder.dot", "w");
    //manager锛欳UDD绠＄悊鍣?
    //2锛氳緭鍑虹殑鏁伴噺
    //outputs锛氳緭鍑鸿妭鐐圭殑鏁扮粍
   Cudd_DumpDot(manager, 2, outputs, inputNames, outputNames, f);
  }

int main()
{
  //                    鍒濆鍖栫鐞嗗櫒            
  //0                   鍒濆鍖朾dd鍙橀噺鐨勬暟鐩?  //0                   鍒濆鍖杬dd鍙橀噺鐨勬暟鐩?
  //CUDD_UNIQUE_SLOTS锛?鍒濆鍖栧敮涓€琛ㄧ殑澶у皬锛?56锛?  //CUDD_CACHE_SLOTS锛? 鍒濆鍖栫紦瀛樺ぇ灏忥紙262144锛?  //0                   鏈€澶х洰鏍囧唴瀛橈紙0浠ｈ〃娌℃湁闄愬埗锛?  DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
  //鍒涘缓BDD
  DdNode **outputs = createHalfAdderBDD(manager);
  //鍐欏叆BDD鍒颁竴涓偣鏂囦欢
  toDot(manager, outputs);
  test(manager, outputs);
  return 0;
}

