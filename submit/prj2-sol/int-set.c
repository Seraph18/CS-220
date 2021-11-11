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
        return 0;
    }
    Node *currNode = tempHead->dummyNode.next;

    for (int i = 0; i < tempHead->nElements; ++i)
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
    // Return 0 is element is not in set
    return 0;
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
        newNode->data = element; // add data to new node

        // Check if set is empty
        if (tempHead->nElements == 0)
        { // Add Node to header
            tempHead->dummyNode.next = newNode;
            tempHead->nElements = tempHead->nElements + 1;
            return tempHead->nElements;
        }
        else
        { // Add Node to end
            Node *currNode = tempHead->dummyNode.next;
            Node *prevNode = tempHead->dummyNode.next;
            int firstRun = 1;

            if (firstRun == 1 && newNode->data < currNode->data)
            {
                firstRun = 0;
                newNode->next = tempHead->dummyNode.next;
                tempHead->dummyNode.next = newNode;
            }
            else
            {
                // Find Where to put the newNode
                for (int i = 0; i < tempHead->nElements - 1; ++i)
                {
                    // Looking to put node in correct order
                    if (currNode->data < newNode->data)
                    { // Check if the current node is smaller than the new node; if the current node is smaller, keep looking
                        if (firstRun == 1)
                        { // Don't advance prevNode on the first run through
                            firstRun = 0;
                            currNode = currNode->next; // go to next node
                        }
                        else
                        {
                            currNode = currNode->next;
                            prevNode = prevNode->next;
                        }
                    }
                    else
                    {
                        // Found spot to put it before the very end
                        if (firstRun == 1)
                        { // First runthrough, and therefore will be attached to the head
                            firstRun = 0;
                            newNode->next = tempHead->dummyNode.next;
                            tempHead->dummyNode.next = newNode;
                        }
                        else
                        {
                            newNode->next = currNode;
                            prevNode->next = newNode;
                        }

                        // Finished run
                        tempHead->nElements = tempHead->nElements + 1;
                        return tempHead->nElements;
                    }
                }

                // Do last check seperatly
                if (newNode->data < currNode->data)
                {
                    newNode->next = currNode;
                    prevNode->next = newNode;
                }
                else
                {
                    currNode->next = newNode;
                }
            }
            tempHead->nElements = tempHead->nElements + 1;
        }
    }

    return tempHead->nElements;
}

/** Change intSet by adding all elements in array elements[nElements] to
 *  it.  Returns # of elements in intSet after addition.  Returns
 *  < 0 on error with errno set.
 */
int addMultipleIntSet(void *intSet, const int elements[], int nElements)
{
    Header *newSetHeader = (Header *)intSet;

    if (intSet == NULL)
    {
        return -1;
    }
    for (int i = 0; i < nElements; ++i)
    {
        addIntSet(intSet, elements[i]);
    }
    return newSetHeader->nElements;
}

/** Set intSetA to the union of intSetA and intSetB.  Return # of
 *  elements in the updated intSetA.  Returns < 0 on error.
 */
int unionIntSet(void *intSetA, void *intSetB)
{
    Header *tempHeadA = (Header *)intSetA;
    Header *tempHeadB = (Header *)intSetB;
    if (tempHeadB == NULL || tempHeadA == NULL)
    { // Check if either set is invalid
        return -1;
    }
    int nElementsInB = nElementsIntSet(intSetB);

    if (nElementsInB == 0)
    { // Check if second set is empty, if so, nothing to be done but return nElements of the first set
        return nElementsIntSet(intSetA);
    }

    // Combing second set into the first one
    Node *currNodeInB = tempHeadB->dummyNode.next;
    for (int i = 0; i < nElementsInB; ++i)
    {
        if (addIntSet(intSetA, currNodeInB->data) < 0)
        {
            return -1;
        }
        else
        {
            if (currNodeInB->next != NULL)
            {
                currNodeInB = currNodeInB->next;
            }
        }
    }

    return tempHeadA->nElements;
}

/** Set intSetA to the intersection of intSetA and intSetB.  Return #
 *  of elements in the updated intSetA.  Returns < 0 on error.
 */
int intersectionIntSet(void *intSetA, void *intSetB)
{
    Header *headerA = (Header *)intSetA;
    Header *headerB = (Header *)intSetB;

    // Some Edge case tests

    // Check if sets are null
    if (headerA == NULL || headerB == NULL)
    {
        return -1;
    }

    if ((headerA->nElements || headerB->nElements) == 0)
    {
        return 0; // No elements in common
    }

    void *setA;
    setA = orderAndRemoveDuplicatesFromSet(intSetA);
    void *setB;
    setB = orderAndRemoveDuplicatesFromSet(intSetB);

    headerA = (Header *)setA;
    headerB = (Header *)setB;

    void *intersectionSet = newIntSet();

    for (const void *iterA = newIntSetIterator(setA); iterA != NULL;
         iterA = stepIntSetIterator(iterA))
    {

        for (const void *iterB = newIntSetIterator(setB); iterB != NULL;
             iterB = stepIntSetIterator(iterB))
        {
            int valueA = intSetIteratorElement(iterA);
            int valueB = intSetIteratorElement(iterB);

            if (valueA == valueB)
            {
                addIntSet(intersectionSet, valueA);
            }
        }
    }
    // reset intSetA
    freeIntSetButKeepHead(intSetA);
    headerA = (Header *)intSetA;


    // Make deep copy of intersectionSet
    for (const void *iterIntersection = newIntSetIterator(intersectionSet); iterIntersection != NULL;
         iterIntersection = stepIntSetIterator(iterIntersection))
    {
        addIntSet(intSetA, intSetIteratorElement(iterIntersection));
    }
    freeIntSet(intersectionSet);
    freeIntSet(setA);
    freeIntSet(setB);
    return nElementsIntSet(intSetA);
}

// Helper Method for intersection; returns a set in the form of a void pointer
void *
orderAndRemoveDuplicatesFromSet(const void *intSet)
{

    Header *originalHeader = (Header *)intSet;

    void *newSet = newIntSet();

    Node *currNode = originalHeader->dummyNode.next;

    // Add elements from orignal set to newSet
    for (int i = 0; i < originalHeader->nElements; ++i)
    {
        addIntSet(newSet, currNode->data);
        if (currNode->next != NULL)
        {
            currNode = currNode->next;
        }
    }
    // Return cleaned up set
    return newSet;
}

/** Free all resources used by previously created intSet. */
void freeIntSet(void *intSet)
{
    Header *newSetHeader = (Header *)intSet;
    int numberOfElements = newSetHeader->nElements;

    Node *nodeArr[numberOfElements];

    Node *currNode = &newSetHeader->dummyNode;
    int currNodeIndex = 0;
    for (int i = 0; i < newSetHeader->nElements; ++i)
    {
        currNode = currNode->next;
        nodeArr[i] = currNode;
        ++currNodeIndex;
    }

    for (int i = 0; i < currNodeIndex; ++i)
    {
        free(nodeArr[i]);
    }

    free((void *)newSetHeader);
}

void freeIntSetButKeepHead(void *intSet){
    Header *newSetHeader = (Header *)intSet;
    int numberOfElements = newSetHeader->nElements;

    Node *nodeArr[numberOfElements];

    Node *currNode = &newSetHeader->dummyNode;
    int currNodeIndex = 0;
    for (int i = 0; i < newSetHeader->nElements; ++i)
    {
        currNode = currNode->next;
        nodeArr[i] = currNode;
        ++currNodeIndex;
    }

    for (int i = 0; i < currNodeIndex; ++i)
    {
        free(nodeArr[i]);
    }
    newSetHeader->nElements = 0;
    newSetHeader->dummyNode.next = NULL;
}

/** Return a new iterator for intSet.  Returns NULL if intSet
 *  is empty.
 */
const void *newIntSetIterator(const void *intSet)
{
    const Header *tempHead = (Header *)intSet;
    Node *n = tempHead->dummyNode.next;
    return n;
}

/** Return current element for intSetIterator. */
int intSetIteratorElement(const void *intSetIterator)
{

    const Node *currNode = (Node *)intSetIterator;
    return currNode->data;
}

/** Step intSetIterator and return stepped iterator.  Return
 *  NULL if no more iterations are possible.
 */
const void *stepIntSetIterator(const void *intSetIterator)
{

    const Node *currNode = (Node *)intSetIterator;
    return currNode->next;
}

/**Tool I used for debugging. */
/*
void printIntSet(const void *intSet, int nElements)
{
    Header *tempHead = (Header *)intSet;
    Node *currNode = tempHead->dummyNode.next;
    printf("\n{");
    for (int i = 0; i < nElements - 1; ++i)
    {
        printf("%d, ", currNode->data);
        currNode = currNode->next;
    }
    printf("%d }\n", currNode->data);
}
*/