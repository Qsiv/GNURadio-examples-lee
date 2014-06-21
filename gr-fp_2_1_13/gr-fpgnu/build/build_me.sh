#!/usr/bin/sh
cmake ../
sudo make
sudo make test
sudo ctest -VV
sudo make install



