# odbc-ipfs
![MSBuild](https://github.com/odbc-ipfs/odbc-ipfs/workflows/MSBuild/badge.svg)

Open Database Connectivity (ODBC) driver for IPFS

This driver uses libp2p's pubsub to send databse queries. IPFS nodes then parse the request and return a response and synchronize among themselves.

# WARNING: All database transactions are currently public using libp2p pubsub. NOTHING is encrypted.

### Note: There is no built-in installer at the moment. You must manually edit the registry and create a .dsn file for now.

### Creating File DSN File
1. Create new file named "odbc-ipfs.dsn"
2. Insert 
     ```
     [ODBC]
     DRIVER=odbc-ipfs
     ```
### Installing the driver in Windows Registry

1. Open Windows Registry Editor

2. Go to "Computer\HKEY_LOCAL_MACHINE\SOFTWARE\ODBC\ODBCINST.INI\ODBC Drivers"
     - Add new "String Value" and set the name to "odbc-ipfs" (name is not important, it just must be consistent)

3. Go to "Computer\HKEY_LOCAL_MACHINE\SOFTWARE\ODBC\ODBCINST.INI"
     - Add new "Key" and set the name to "odbc-ipfs"

4. Go to "Computer\HKEY_LOCAL_MACHINE\SOFTWARE\ODBC\ODBCINST.INI\odbc-ipfs"
     - Add new "String Value" and set the name to "Driver"
     - Right click "Driver", click "Modify..." and set "Value data:" equal to the .dll path (including the odbc-ipfs.dll, not just the folder)


     - Add new "String Value" and set the name to "Setup"
     - Right click "Setup", click "Modify..." and set "Value data:" equal to the .dll path (including the odbc-ipfs.dll, not just the folder)


     - Add new "DWORD" and set the name to "UsageCount"
     - Right click "UsageCount", click "Modify..." and set "Value data:" equal 1 (Base: hexadecimal)






