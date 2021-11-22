# Operators

In afro operators are wrappers commands to support undo/ redo. However operators can't be executed directly they should be enqueued into the operator_queue in the main context. Since operators can't call other operators amid execution they should contain as small code as possible in order to have better code reuse.
