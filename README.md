
ERPC C/C++ Source files at the moment are not generated by the makefile, they have to 
be manually generated prior to running `make` with the following command:

```bash
./Linux_x64/erpcgen -o build/generated/ src/example.erpc
```
