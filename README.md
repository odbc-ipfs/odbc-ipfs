# odbc-ipfs
![MSBuild](https://github.com/odbc-ipfs/odbc-ipfs/workflows/MSBuild/badge.svg)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs?ref=badge_shield)
[![Build status](https://ci.appveyor.com/api/projects/status/xywxqqk5oalur4r5?svg=true)](https://ci.appveyor.com/project/tctromp/odbc-ipfs)


Open Database Connectivity (ODBC) driver for IPFS

This driver uses libp2p's pubsub to send SQL queries. IPFS nodes then parse the request and return a response and synchronize among themselves.

#### Project Status: Communication between the driver and a go node in IPFS works, but go nodes do not communicate between themselves yet.

# WARNING: All database transactions are currently public using libp2p pubsub. NOTHING is encrypted yet.

### [Docker Hub](https://hub.docker.com/repository/docker/tctromp/odbc-ipfs-node)
- Run node (assumes IPFS is running)
     ```
     docker run tctromp/odbc-ipfs-node:latest
     ```
- Run node and IPFS
- Copy docker-compose.yml from the odbc-ipfs-go-node folder into a folder
- NOTE: For linux you must change "127.0.0.1:4001:4001" to "4001:4001"
     ```     
     docker-compose up
     ```

     
### Building
- Build odbc-ipfs.dll, Installer.exe, Uninstaller.exe
- Binaries in /x64/Release
     ```
     msbuild odbc-ipfs.sln
     ```
- Build odbc-ipfs-go-node.exe
     ```
     cd odbc-ipfs-go-node-main
     go build
     ```
- Build cgo-connector.dll, cgo-connector.h
     ```
     cd odbc-ipfs-cgo-ipfs
     go build -buildmode=c-shared -o cgo-connector.dll
     ```
     - Note: .lib file must be generated from the cgo-connector.dll
     - This is because MSVC does not support directly embedding cgo
    
### Structure
- core.cpp
     - Contains the core ODBC functions
- ipfsConnector.cpp
     - Contains 4 functions called from core.cpp
     - Functions defined in connector.h
          ```
          - SQL_API execute(SQLHSTMT sqlstmt, SQLWCHAR* StatementText)
          - SQL_API fetch(SQLHSTMT sqlstmt)
          - SQL_API connect(SQLHENV EnvironmentHandle, SQLHDBC* ConnectionHandle)
          - SQL_API disconnect(SQLHDBC ConnectionHandle)
          ```
- odbc-ipfs-go-node/main.go
     - Conatains the logic for talking to odbc-ipfs-cgo-ipfs
     - Stores and queries the sqlite databse
   
- odbc-ipfs-cgo-ipfs/pubsub.go
     - Builds into cgo-connector.dll
     - cgo bridge from the driver to golang to use libp2p pubsub
     - NOTE: Currently requires a full IPFS node to be running. Embedded libp2p is planned.

### Golang Documentation
- https://pkg.go.dev/github.com/odbc-ipfs/odbc-ipfs

### Debugging
- All ODBC functions in this driver have debug statements embedded in them.
- To listen to an active driver's debug messages we use [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)

### Installing the driver in Windows Registry

1. Run Installer.exe as Administrator
2. Copy odbc-ipfs.dsn to C:/Program Files/Common Files/Odbc/Data Sources/ 

### Uninstalling the driver in Windows Registry

1. Run Uninstaller.exe as Administrator
2. Remove odbc-ipfs.dsn from C:/Program Files/Common Files/Odbc/Data Sources/

## License
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs?ref=badge_large)
=======


