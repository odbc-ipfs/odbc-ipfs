# odbc-ipfs
![MSBuild](https://github.com/odbc-ipfs/odbc-ipfs/workflows/MSBuild/badge.svg)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs?ref=badge_shield)

Open Database Connectivity (ODBC) driver for IPFS

This driver uses libp2p's pubsub to send SQL queries. IPFS nodes then parse the request and return a response and synchronize among themselves.

# WARNING: All database transactions are currently public using libp2p pubsub. NOTHING is encrypted yet.

### Creating File DSN File
1. Create new file named "odbc-ipfs.dsn"
2. Insert 
     ```
     [ODBC]
     DRIVER=odbc-ipfs
     ```
### Debugging
- All ODBC functions in this driver have debug statements embedded in them.
- To listen to an active driver's debug messages we use [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)

### Installing the driver in Windows Registry

1. Run Installer.exe as Administrator

### Uninstalling the driver in Windows Registry

1. Run Uninstaller.exe as Administrator



## License
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fodbc-ipfs%2Fodbc-ipfs?ref=badge_large)
=======


