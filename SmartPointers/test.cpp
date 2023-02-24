/*#include "smart_pointers.hpp";

int main()
{
    // Test the unique pointer

    emb::unique_ptr<int> a(new int(20)); // Create pointer with constructor
    emb::unique_ptr<int> b; // Create null pointer.

    b.move_to(a); // shift data around.

    size_t sizeA = sizeof(a); // Get the size of the pointers.
    size_t sizeB = sizeof(b);

    // Testing shared pointers.
    emb::shared_ptr<int> shared_a = emb::shared_ptr<int>(new int(20)); // allocate an integer
    emb::shared_ptr<int> shared_b = shared_a; // These should all be the same.
    {
        emb::shared_ptr<int> shared_c = shared_b;
        emb::shared_ptr<int> shared_d = shared_a;
    }
    emb::shared_ptr<int> shared_e = emb::shared_ptr<int>(new int(30)); // new heap allocated integer

    shared_e = shared_a; // Free the second integer.

    shared_a.erase(); // Remove A

    size_t sizeE = sizeof(shared_e);

    return 0;
}*/