# Requirements
- Ubuntu 16.04 (Virtual machine or PC, 4Gb RAM, NN Gb disk space)

TODO:
1. shell script to install dependencies and build the project
2. vagrant image for developers

# Install dependencies

```
sudo apt install build-essential cmake git pkg-config libboost-all-dev libssl-dev libunwind8-dev liblzma-dev libldns-dev libexpat1-dev doxygen graphviz
```
# Clone the repository

```
cd $HOME
git clone https://github.com/graft-project/GraftNetwork.git
```
# Build

```
cd GraftNetwork
make -j4
```
in order to build standalone binaries (i.e. to upload it to the server where no dev packages installed), invoke

```
make -j4 release-static
```

for "-j" param select approriate number of CPU cores available on your system

# Copy the binaries to the appropriate directory

```
sudo cp build/release/bin/* /usr/local/bin/
```
(system wide)

or 

```
mkdir $HOME/bin
cp build/release/bin/* $HOME/bin
```
(user wide)

For developers, it make sense to create symlinks instead of copying binaries

```
ln -s $(pwd)/build/release/bin/* $HOME/bin
```


# Start private testnet

for private testnet we'll be running three nodes

## Start node #1
TODO

## Start node #2
TODO

## Start node #3
TODO

## Create wallets




# Connect to public testnet
TODO

# Connect to mainnet
TODO
