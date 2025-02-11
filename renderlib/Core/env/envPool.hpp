/****************************************************************************\
**  envPool.hpp
**  A class which assists in using pool allocation schemes.  Pool-style
**  allocaiton provides speedier allocation and deallocation of fixed
**	size memory areas.
**
**
**
\****************************************************************************/

#ifdef ENV_POOL_HPP
#error envPool.hpp multiple included
#endif
#define ENV_POOL_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif
#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif
#ifndef DBG_MSG_HPP
#include "Core/dbg/dbgMsg.hpp"
#endif

//============================================================================
//	Make this 1 to enable some extra integrity testing stuff.
//============================================================================
#define ENV_POOL_PARANOID 0
// #define ENV_POOL_PARANOID 1

//============================================================================
//	envPool
//============================================================================
class envPool
{
public:
  //--------------------------------------------------------------------
  //  The constructor creates a pool to handle elements of the given
  //	size (usually sizeof(Class)), with the given number of
  //	elements per allocation block.
  //--------------------------------------------------------------------
  envPool(int iElementSize, int iElementsPerBlock = 256);

  //--------------------------------------------------------------------
  //  ~envPool() deallocates all memory, so make sure that
  //	elements have been deleted before the pool is destroyed.
  //--------------------------------------------------------------------
  ~envPool();

  //--------------------------------------------------------------------
  //  Allocate() allocates memory space for a single element
  //	of the size given in the constructor.  This could return NULL.
  //--------------------------------------------------------------------
  void* Allocate();

  //--------------------------------------------------------------------
  //  Deallocate() deallocates the memory for some element.
  //	Of course, this must be a pointer previously returned by Allocate().
  //--------------------------------------------------------------------
  void Deallocate(void* i_Ptr);

  //--------------------------------------------------------------------
  //  DeallocateAll() deallocates all memory, so again
  //	make sure that all elements have been deleted and are not
  //	using the memory.
  //--------------------------------------------------------------------
  void DeallocateAll();

  //--------------------------------------------------------------------
  //  SetAllocationLimit() prevents the pool from allocating
  //	more than i_Bytes total.  If the pool can't allocate memory,
  //	Allocate() will return NULL.
  //--------------------------------------------------------------------
  void SetAllocationLimit(int i_Bytes);

private:
  //--------------------------------------------------------------------
  //	test_valid_ptr tests to make sure the given i_Ptr comes from our
  //	block.
  //--------------------------------------------------------------------
  void test_valid_ptr(void* i_Ptr);

  //--------------------------------------------------------------------
  //	test_list_valid tests all pointers in the freelist to make sure
  //	they are valid
  //--------------------------------------------------------------------
  void test_list_valid();

  //--------------------------------------------------------------------
  //	test_already_freed finds out if the given pointer is already on
  //	the freelist (and so shouldn't be deleted again).
  //--------------------------------------------------------------------
  void test_already_freed(void* i_Ptr);

  struct Block
  {
    Block* m_Next;
  };

  struct ListElement
  {
    ListElement* m_Next;
  };

  ListElement* parcel_block(Block* iBlock);

  void operator=(const envPool&); // disallow assignment

  const int m_ElementsPerBlock;
  const int m_ElementSize;
  const int m_BlockSize;

  int m_AllocLimit;
  int m_BytesAllocated;

  Block* m_BlockHead;
  ListElement* m_ListHead;
};

//----------------------------------------------------------------------------
//  Allocate() allocates memory space for a single element
//	of the size given in the constructor.  This could return NULL.
//----------------------------------------------------------------------------
inline void*
envPool::Allocate()
{
#if ENV_POOL_PARANOID
  test_list_valid();
#endif

  // we may need a new block
  if ((m_ListHead == NULL) || (m_ListHead->m_Next == NULL)) {
    if (m_BytesAllocated + m_BlockSize > m_AllocLimit)
      return NULL;

    Block* new_block = (Block*)::operator new(m_BlockSize);
    m_BytesAllocated += m_BlockSize;
    new_block->m_Next = m_BlockHead;
    m_BlockHead = new_block;
    m_ListHead = parcel_block(new_block);
  }

  // return the top of our list, and move the head
  // to the next element
  ListElement* ret_val = m_ListHead;
  m_ListHead = m_ListHead->m_Next;

#if ENV_POOL_PARANOID
  test_list_valid();
#endif

  return ret_val;
}

//----------------------------------------------------------------------------
//  Deallocate() deallocates the memory for some element.
//	Of course, this must be a pointer previously returned by Allocate().
//----------------------------------------------------------------------------
inline void
envPool::Deallocate(void* i_Ptr)
{
#if ENV_POOL_PARANOID
  test_valid_ptr(i_Ptr);
  test_list_valid();
  test_already_freed(i_Ptr);
#endif

  // push i_Ptr on our free list
  ((ListElement*)i_Ptr)->m_Next = m_ListHead;
  m_ListHead = (ListElement*)i_Ptr;

#if ENV_POOL_PARANOID
  test_list_valid();
#endif
}
