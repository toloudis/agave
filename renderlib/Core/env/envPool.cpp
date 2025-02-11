/****************************************************************************\
**  envPool.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/env/envPool.hpp"

//----------------------------------------------------------------------------
//  parcel_block(envPool::Block* iBlock)
//----------------------------------------------------------------------------
envPool::ListElement*
envPool::parcel_block(envPool::Block* iBlock)
{
  char* block = (char*)iBlock;

  block += 4;

  int i;
  ListElement* prev_head = m_ListHead;

  for (i = 0; i < m_ElementsPerBlock; i++) {
    ListElement* cur_head = (ListElement*)block;
    cur_head->m_Next = prev_head;
    prev_head = cur_head;
    block += m_ElementSize;
  }

  return prev_head;
}

//--------------------------------------------------------------------
//  The constructor creates a pool to handle elements of the given
//	size (usually sizeof(Class)), with the given number of
//	elements per allocation block.
//--------------------------------------------------------------------
envPool::envPool(int iElementSize, int iElementsPerBlock)
  : m_ElementsPerBlock(iElementsPerBlock)
  , m_ElementSize(iElementSize)
  , m_BlockSize(iElementSize * iElementsPerBlock + 4)
  , m_AllocLimit(0x7fffffff)
  , m_BytesAllocated(0)
  , m_ListHead(NULL)
  , m_BlockHead(NULL)
{
}

//--------------------------------------------------------------------
//  ~envPool() deallocates all memory, so make sure that
//	elements have been deleted before the pool is destroyed.
//--------------------------------------------------------------------
envPool::~envPool()
{
  this->DeallocateAll();
}

//--------------------------------------------------------------------
//  DeallocateAll() deallocates all memory, so again
//	make sure that all elements have been deleted and are not
//	using the memory.
//--------------------------------------------------------------------
void
envPool::DeallocateAll()
{
  Block* cur = m_BlockHead;

  while (cur) {
    Block* next = cur->m_Next;
    ::operator delete(cur);
    cur = next;
  }

  m_BlockHead = NULL;
  m_ListHead = NULL;
  m_BytesAllocated = 0;
}

//--------------------------------------------------------------------
//  SetAllocationLimit() prevents the pool from allocating
//	more than iBytes total.  If the pool can't allocate memory,
//	Allocate() will return NULL.
//--------------------------------------------------------------------
void
envPool::SetAllocationLimit(int iBytes)
{
  m_AllocLimit = iBytes;
}

//--------------------------------------------------------------------
//	test_valid_ptr tests to make sure the given i_Ptr comes from our
//	block.
//--------------------------------------------------------------------
void
envPool::test_valid_ptr(void* i_Ptr)
{
  //	debug; test to make sure i_Ptr really comes from this pool
  bool found = false;
  Block* cur_block = m_BlockHead;
  while (cur_block) {
    if (envType::UIntPtr(i_Ptr) > envType::UIntPtr(cur_block)) {
      if ((envType::UIntPtr(i_Ptr) - envType::UIntPtr(cur_block)) < envType::UIntPtr(m_BlockSize)) {
        found = true;
        break;
      }
    }

    cur_block = cur_block->m_Next;
  }

  DBG_ASSERT(found, "pointer not found in pool");
}

//--------------------------------------------------------------------
//	test_list_valid tests all pointers in the freelist to make sure
//	they are valid
//--------------------------------------------------------------------
void
envPool::test_list_valid()
{
  ListElement* cur = m_ListHead;
  while (cur) {
    test_valid_ptr(cur);
    cur = cur->m_Next;
  }
}

//--------------------------------------------------------------------
//	test_already_freed finds out if the given pointer is already on
//	the freelist (and so shouldn't be deleted again).
//--------------------------------------------------------------------
void
envPool::test_already_freed(void* i_Ptr)
{
  ListElement* cur = m_ListHead;
  while (cur) {
    DBG_ASSERT(cur != i_Ptr, "Pointer already freed");
    cur = cur->m_Next;
  }
}
