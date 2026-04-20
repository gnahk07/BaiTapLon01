# TODO: Code Review Fixes for Apartment Management System

## Plan Status: Pending User Approval

### 1. Fix Immediate Bugs [ ]
- Rename floor dirs to Floor_%d consistently (floor.c/room.c)
- Fix room.c paths, stat usage, addRoom logic
- Implement displayHeader in Main.c
- Fix menu.c recursion, add full menu
- Complete resident input loop

### 2. Add Core Features [ ]
- Linked list for residents
- Binary serialization per room
- Full CRUD/search

### 3. Refactor [ ]
- Dynamic memory
- Error handling

## Commands to Test
```bash
gcc *.c -o main.exe
./main.exe
```

**Next**: Await approval to implement step-by-step.

