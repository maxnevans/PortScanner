# Parallel Port Scanner

This console Windows application could be used to scan for open ports.

## Used technologies:
- multithreading with new threadpool winAPI;
- WSA to establish connection;
- critical sections;
- barriers to sync all worker threads (could be done with mutex or critical section but practice with barriers needed);
- memory management with smart pointers (unique_ptr, shared_ptr);
- RAII.
