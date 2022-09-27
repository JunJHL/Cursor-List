#include "cursor_list.hpp"

#include <iostream>

#define NUM_SLOTS 30
static bool slots_open = true;

struct Slot
{
    int data;
    unsigned next;  // index of next slot
};

static struct Slot SLOTS[NUM_SLOTS];

unsigned slotAlloc()
{
    // TODO: Implement.
    if(slots_open == true) {
        for(unsigned i = 0; i < NUM_SLOTS-1; ++i) {
            SLOTS[i].next = i+1;    // initialized the free list
        }
        SLOTS[NUM_SLOTS-1].next = 0;
        slots_open = false;
    }

    if(SLOTS[0].next == 0) {
        throw std::runtime_error("Slots Not Available");
    }
    unsigned slot = SLOTS[0].next;  // get next slot indicated by the head of the free list
    SLOTS[0].next = SLOTS[slot].next;   // assigned next slot of the current index to the head of the free list

    return slot;
}

void slotFree(unsigned index)
{
    // TODO: Implement.
    unsigned temp = SLOTS[0].next;
    SLOTS[0].next = index;
    SLOTS[index].next = temp;
}

void printSlots(unsigned start, unsigned end)
{
    for (unsigned i = start; i < end; ++i)
        std::cout << "SLOTS[" << i << "]: " << SLOTS[i].data
            << ' ' << SLOTS[i].next << '\n';
}

unsigned clCreate()
{
    // TODO: Implement.
    unsigned head = slotAlloc();
    SLOTS[head].next = 0;
    return head;
}

void clDestroy(unsigned head)
{
    // TODO: Implement.
    unsigned index = head;
    unsigned temp = 0;
    while(index != 0) {
        temp = index;
        index = SLOTS[index].next;    // update index
        slotFree(temp);    // free index before update
    }
}

void clAppend(unsigned node, int val)
{
    // TODO: Implement.
    unsigned index = node;
    while(SLOTS[index].next != 0) { // iterate until end of list
        index = SLOTS[index].next;
    }
    SLOTS[index].next = clCreate(); // new node
    SLOTS[SLOTS[index].next].data = val;
}

void clInsert(unsigned node, int val)
{
    // TODO: Implement.
    unsigned next = SLOTS[node].next;
    unsigned temp = next;  // next node of the current node
    if(next == 0) {     // if node is the @end, clCreate to new node, and next index of new node to 0
        next = clCreate();
        SLOTS[node].next = next;
        SLOTS[next].data = val;
    }else {
        next = slotAlloc(); // set current node's next node to new node
        SLOTS[node].next = next;
        SLOTS[next].next = temp;    // set new node's next node to the old next node of current node
        SLOTS[next].data = val;
    }
}

unsigned clFind(unsigned head, int val)
{
    // TODO: Implement.
    unsigned index = head;
    while(SLOTS[index].next != 0) {
        if(SLOTS[index].data == val) {
            return index;
        }
        index = SLOTS[index].next;
    }
    return 0;
}

void clPrint(unsigned head)
{
    // TODO: Implement.
    std::cout << "SLOTS[" << head << "]: (header)" << std::endl;

    unsigned index = SLOTS[head].next;
    while(index != 0) {
        std::cout << "SLOTS[" << index << "]: " << SLOTS[index].data << std::endl;
        index = SLOTS[index].next;
    }
}

bool clDelete(unsigned head, int val)
{
    // TODO: Implement.
    unsigned index = clFind(head, val);
    unsigned previous = head;
    if(index != 0) {
        while(SLOTS[previous].next != index) {
            previous = SLOTS[previous].next;
        }
        if(SLOTS[index].next == 0) {
            SLOTS[previous].next = 0;
            SLOTS[index].data = 0;
            slotFree(index);
        }else {
            SLOTS[previous].next = SLOTS[index].next;
            SLOTS[index].data = 0;
            slotFree(index);
        }
        return true;
    }else {
        return false;
    }
}

bool clIsEmpty(unsigned head)
{
    // TODO: Implement.
    if(SLOTS[head].next == 0) {
        return true;
    }
    return false;
}

unsigned clLength(unsigned head)
{
    // TODO: Implement.
    unsigned length = 0;
    unsigned index = SLOTS[head].next;
    while(index != 0) {
        ++length;
        index = SLOTS[index].next;
    }
    return length;  // when index == 0
}

1 - 3 - 9 - 5 - 0
unsigned clDeleteAll(unsigned head, int val) // delete all matching value
{
    // TODO: Implement.
    unsigned num_del = 0;
    unsigned index = head;
    unsigned previous = index;
    bool deleted = false;

    while(index != 0) {
        deleted = clDelete(previous, val);
        if(deleted) {
            ++num_del;
            index = clFind(previous, val);
            while(SLOTS[previous].next != index) {
                previous = SLOTS[previous].next;
            }
        }else {
            break;
        }
    }
    return num_del;
}

unsigned clCopy(unsigned head)
{
    // TODO: Implement.
    unsigned cp_head = clCreate();
    unsigned cp_index = cp_head;
    unsigned index = SLOTS[head].next;
    if(clLength(head) > clLength(0)) {
        throw std::runtime_error("Not Enough Slots Available to Copy");
    }
    while(index != 0) {
        clAppend(cp_index, SLOTS[index].data);
        index = SLOTS[index].next;
        cp_index = SLOTS[cp_index].next;
    }
    return cp_head;
}

void clReverse(unsigned head)
{
    // TODO: Implement.
   unsigned previous = head;
   unsigned index = SLOTS[head].next;
   unsigned next = head;
   while(index != 0) {
       next = SLOTS[index].next;
       SLOTS[index].next = previous;
       previous = index;
       index  = next;
   }
   SLOTS[SLOTS[head].next].next = 0;
   SLOTS[head].next = previous;
}
