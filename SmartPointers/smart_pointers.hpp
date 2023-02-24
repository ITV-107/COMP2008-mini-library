#ifndef SMART_POINTER_HPP // Header guard to prevent the test program from doing shenanigans.
#define SMART_POINTER_HPP

/*
/ ============================================================================
| ======================== COMP2008  Embedded Systems ========================
< ======================== Unique and Shared Pointers ========================
| ============================== ISAAC VASSALLO ==============================
\ ============================================================================
*/

// These should hopefully help with memory deallocation of "new" objects in 
// embedded systems without bloating the chip's memory with the standard library

namespace emb {

    /// @brief A class with unique ownership of the data it carries.
    /// @tparam datatype The type that this pointer holds. 
    template <typename datatype>
    class unique_ptr
    {
    private:
        datatype* ptr; // Address to look at in memory.
    public:
        /// @brief Fills in data from smart pointer. Leave it blank for nullptr
        /// @param _ptr The object to copy over.
        unique_ptr(datatype* _ptr = nullptr) : ptr(_ptr) {
            
        };

        // This automatically deletes a unique pointer's information
        ~unique_ptr() {
            // Don't attempt to free up 0x0000000000000, might be vital memory.
            if (ptr != nullptr)
            {
                delete ptr;
            }
        };

        // This prevents duplicating a unique pointer by copying it.
        unique_ptr(const unique_ptr&) = delete;

        // This prevents duplicating a unique pointer by assigning it.
        unique_ptr& operator = (const unique_ptr&) = delete;

        /// @brief Transfers ownership from input pointer to this.
        /// @param in The unique pointer to transfer ownership from.
        /// @return This pointer with the address of in.
        unique_ptr& move_to(unique_ptr& in) {

            // Don't overwrite if self-assignment occurs.
            if (this != &in)
            {
                ptr = in.ptr; // Transfer information to this ptr.
                in.ptr = nullptr; // Null the other pointer
            }
            return *this;
        }
        
        /// @brief Deferencing operation
        /// @return Item stored in ptr
        datatype& operator *() {
            return *ptr;
        }

        /// @brief Indirection operation
        /// @return Member or function of pointer
        datatype* operator->() {
            return ptr;
        }
    };


    /// @brief This counts the number of pointers in a given set bound to the same memory block.
    class reference_count
    {
    public:
        int refs;
    public:
        /// @brief Increment reference count
        void add() {
            refs++;
        }
        /// @brief Decrement reference count
        int release() {
            return --refs;
        }
    };


    
    /// @brief A pointer class that uses a heap-based reference counter.
    /// @tparam datatype The type that this pointer holds. 
    template <typename datatype>
    class shared_ptr
    {
    private:
        datatype* ptr; // Data pointer
        reference_count* rp; // Reference pointer

    public:
        /// @brief Add an entirely new shared pointer set.
        /// @param _ptr The heap memory to point to.
        shared_ptr(datatype* _ptr = nullptr) : ptr(_ptr), rp(0)
        {
            rp = new reference_count(); // Increment the copied reference count.
            rp->add();
        }

        /// @brief Clone the pointer from another shared pointer
        /// @param _ptr The shared pointer to clone from.
        shared_ptr(const shared_ptr& _ptr) : ptr(_ptr.ptr), rp(_ptr.rp)
        {
            // Increment the copied reference count.
            rp->add();
        }

        /// @brief Clone the pointer by assigning a shared pointer.
        /// @param _ptr The shared pointer that is assigned to it.
        shared_ptr& operator = (const shared_ptr& _ptr) {

            // Edge case: A shared pointer overwrites the only pointer to an object.
            if (rp->release()) {
                // Then erase the old reference-count and pointer to that memory.
                delete rp;
                if (ptr != nullptr)
                    delete ptr;
            }

            // Then overwrite the details of the memory with the new pointer's information.
            ptr = _ptr.ptr;
            rp = _ptr.rp;
            rp->add();
            return *this;
        }

        // This automatically deletes a shared pointer's information
        ~shared_ptr()
        {
            // Decrement the reference count and remove the info if there is no reference.
            if (rp->release() == 0) 
            {
                // Remove the reference count and data
                delete rp;
                if (ptr != nullptr)
                    delete ptr;
            }
        }

        /// @brief Erase the shared pointer
        void erase() {
            this->~shared_ptr();
        }

        /// @brief Deferencing operation
        /// @return Item stored in ptr
        datatype& operator *() {
            return *ptr;
        }

        /// @brief Indirection operation
        /// @return Member or function of pointer
        datatype* operator->() {
            return ptr;
        }
    };
}
#endif