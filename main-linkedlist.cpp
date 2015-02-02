#include "LinkedList.h"
#include "PropertyList.h"

int main() {


  //  LinkedList ll;


  cout << "New Linked List test\n";
  LinkedList ll;
  LinkedList lb;

  ll.insert("A", "a");
  ll.insert("B", "b");
  ll.print();

  PropertyList<FixedString, FixedString> pp;
  pp.set("A", "a");
  pp.set("B", "b");
  cout << "pp size: " << pp.size() << endl;
  pp.print();

  lb = ll;

  cout << "Print lb\n";
  lb.print();

  // Note that you should only call one of them.
  // Pointers are moved not deleted when doing assignments.
  lb.deleteList();

  FixedString fs("Hello");
  FixedString fsa;

  cout << "Test fixed string\n";

  fsa = fs;

  cout << "fsa: " << fsa << endl;
  return 0;
}

