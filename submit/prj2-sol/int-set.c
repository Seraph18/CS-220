#include "int-set.h"

typedef struct NodeStruct
{ // Node for linked list
    int data;
    struct NodeStruct *next;
} Node;

typedef struct
{ // Header for linked list
    int nElements;
    Node dummyNode;
} Header;

/** Abstract data type for set of int's.  Note that sets do not allow
 *  duplicates.
 */

/** Return a new empty int-set.  Returns NULL on error with errno set.
 */
void *
newIntSet()
{
    return calloc(1, sizeof(Header));
}

/** Return # of elements in intSet */
int nElementsIntSet(void *intSet)
{
    Header *tempHead = (Header *)intSet;
    return tempHead->nElements;
}

/** Return non-zero iff intSet contains element. */
int isInIntSet(void *intSet, int element)
{

    Header *tempHead = (Header *)intSet;

    if (tempHead->nElements == 0)
    {
        return -1;
    }
    Node *currNode = tempHead->dummyNode.next;

    for (int i = 1; i < tempHead->nElements; ++i)
    {
        if (currNode->data == element)
        {
            // Returns 1 if element is in set
            return 1;
        }
        if (i + 1 < tempHead->nElements)
        {
            currNode = currNode->next;
        }
    }
    // Return -1 is element is not in set
    return -1;
}

/** Change intSet by adding element to it.  Returns # of elements
 *  in intSet after addition.  Returns < 0 on error with errno
 *  set.
 */
int addIntSet(void *intSet, int element)
{

    if (intSet == NULL)
    { // Check for no set
        return -1;
    }

    Header *tempHead = (Header *)intSet;

    // Check if element already exists
    if (isInIntSet(intSet, element) == 1)
    {
        // Do nothing if it is already in the set
    }
    else
    {
        Node *newNode = calloc(1, sizeof *newNode);
        newNode->data = element; //add data to new node

        // Check if set is empty
        if (tempHead->nElements == 0)
        { // Add Node to header
            tempHead->dummyNode.next = newNode;
        }
        else
        { // Add Node to end
            Node *lastNode = tempHead->dummyNode.next;

            for (int i = 0; i < tempHead->nElements - 1; ++i){
                lastNode = lastNode->next;
            }
            lastNode->next = newNode;
        }
        tempHead->nElements = tempHead->nElements + 1;
    }

    return tempHead->nElements;
}

/** Change intSet by adding all elements in array elements[nElements] to
 *  it.  Returns # of elements in intSet after addition.  Returns
 *  < 0 on error with errno set.
 */
int addMultipleIntSet(void *intSet, const int elements[], int nElements)
{
    Header *tempHeader = (Header *)intSet;

    if (intSet == NULL)
    {
        return -1;
    }
    for (int i = 0; i < nElements; ++i)
    {
        addIntSet(intSet, elements[i]);
    }
    return tempHeader->nElements;
}

/** Set intSetA to the union of intSetA and intSetB.  Return # of
 *  elements in the updated intSetA.  Returns < 0 on error.
 */
int unionIntSet(void *intSetA, void *intSetB)
{
    // TODO
    return 0;
}

/** Set intSetA to the intersection of intSetA and intSetB.  Return #
 *  of elements in the updated intSetA.  Returns < 0 on error.
 */
int intersectionIntSet(void *intSetA, void *intSetB)
{
    // TODO
    return 0;
}

/** Free all resources used by previously created intSet. */
void freeIntSet(void *intSet)
{
    Header *tempHeader = (Header *)intSet;
    int numberOfElements = tempHeader->nElements;

    Node *nodeArr[numberOfElements];

    Node *currNode = &tempHeader->dummyNode;
    int currNodeIndex = 0;
    for (int i = 0; i < tempHeader->nElements; ++i)
    {
        currNode = currNode->next;
        nodeArr[i] = currNode;
        ++currNodeIndex;
    }

    for (int i = 0; i < currNodeIndex; ++i)
    {
        free(nodeArr[i]);
    }

    free((void *)tempHeader);
    
}

/** Return a new iterator for intSet.  Returns NULL if intSet
 *  is empty.
 */
const void *newIntSetIterator(const void *intSet)
{
    // TODO
    return 0;
}

/** Return current element for intSetIterator. */
int intSetIteratorElement(const void *intSetIterator)
{
    // TODO
    return 0;
}

/** Step intSetIterator and return stepped iterator.  Return
 *  NULL if no more iterations are possible.
 */
const void *stepIntSetIterator(const void *intSetIterator)
{
    // TODO
    return 0;
}
